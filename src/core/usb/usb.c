/*
 * Copyright (c) 2013 somemetricprefix <somemetricprefix+code@gmail.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "usb.h"

#include "../common.h"
#include "../matrix.h"
#include "../timer.h"
#include "descriptors.h"

// Standard file stream for the CDC interface when set up, so that the virtual
// CDC COM port can be used like any regular character stream in the C APIs.
FILE console;

USB_ClassInfo_CDC_Device_t console_interface = {
  .Config = {
    .ControlInterfaceNumber = 0,
    .DataINEndpoint = {
      .Address = CDC_TX_EPADDR,
      .Size    = CDC_TX_EPSIZE,
      .Banks   = 2,
    },
    .DataOUTEndpoint = {
      .Address = CDC_RX_EPADDR,
      .Size    = CDC_RX_EPSIZE,
      .Banks   = 1,
    },
    .NotificationEndpoint = {
      .Address = CDC_NOTIFICATION_EPADDR,
      .Size    = CDC_NOTIFICATION_EPSIZE,
      .Banks   = 1,
    },
  },
};

// Current idle period in ms. This is set by the host via a Set Idle HID class
// request to silence the device's reports for either the entire idle
// duration, or until the report status changes (e.g. the user presses a key).
// HID Specification 1.11:
// "The recommended default idle rate (rate when the device is initialized) is
// 500 milliseconds for keyboards (delay before first repeat rate)"
static u16 idle_time = 500;
static u16 idle_time_remaining = 0;

#define REPORT_SIZE 16
static u8 report[REPORT_SIZE];

void UsbInit(void) {
  // Create a regular character stream for the interface so that it can be used
  // with the stdio.h functions.
  CDC_Device_CreateStream(&console_interface, &console);

  USB_Init();
}

// Resends report when there has been no change for the last idle period.
static void WriteKeyboardEndpoint(void) {
  Endpoint_SelectEndpoint(KEYBOARD_EPADDR);
  if (!Endpoint_IsINReady() || idle_time_remaining > 0)
    return;

  Endpoint_Write_Stream_LE(report, REPORT_SIZE, NULL);
  Endpoint_ClearIN();
}

static void UpdateConsole(void) {
  // Must throw away unused bytes from the host, or it will lock up while
  // waiting for the device.
  CDC_Device_ReceiveByte(&console_interface);
  CDC_Device_USBTask(&console_interface);
}

void UsbTask(void) {
  USB_USBTask();
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  WriteKeyboardEndpoint();
  UpdateConsole();
}

void UsbSendReport(const u8 *report_data) {
  Endpoint_SelectEndpoint(KEYBOARD_EPADDR);

  // Copy report to internal buffer, so that it can be sent again when idle time
  // is over.
  memcpy(report, report_data, REPORT_SIZE);

  Endpoint_Write_Stream_LE(report, REPORT_SIZE, NULL);
  Endpoint_ClearIN();

  idle_time_remaining = idle_time;
}

// Registers all endpoints and enables SOF interrupt.
void EVENT_USB_Device_ConfigurationChanged(void) {
  Endpoint_ConfigureEndpoint(KEYBOARD_EPADDR, EP_TYPE_INTERRUPT,
                             KEYBOARD_EPSIZE, 1);
  CDC_Device_ConfigureEndpoints(&console_interface);

  USB_Device_EnableSOFEvents();
}

// Appendix G: HID Request Support Requirements
// For a Non-Boot Keyboard only GET_REPORT, SET_IDLE, GET_IDLE control requests
// are required to be implemented.
void EVENT_USB_Device_ControlRequest(void) {
  // Handle HID specific requests.
  switch (USB_ControlRequest.bRequest)
  {
    case HID_REQ_GetReport:
      if (USB_ControlRequest.bmRequestType ==
          (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE)) {
        LOG_DEBUG("HID Request: GetReport");

        Endpoint_ClearSETUP();

        // Write report data to the control endpoint. Endpoint is cleared
        // automatically.
        Endpoint_Write_Control_Stream_LE(report, REPORT_SIZE);

        Endpoint_ClearStatusStage();
      }
      break;

    case HID_REQ_SetIdle:
      if (USB_ControlRequest.bmRequestType ==
          (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) {
        LOG_DEBUG("HID Request: SetIdle - %ums", idle_time);

        Endpoint_ClearSETUP();

        // Idle period in MSB. HID Specification sends idle time in a multiple
        // of 4ms.
        idle_time = (USB_ControlRequest.wValue >> 8) * 4;

        Endpoint_ClearStatusStage();
      }
      break;

    case HID_REQ_GetIdle:
      if (USB_ControlRequest.bmRequestType ==
          (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE)) {
        LOG_DEBUG("HID Request: GetIdle");

        Endpoint_ClearSETUP();

        // HID Specification sends idle time in a multiple of 4ms.
        Endpoint_Write_8(idle_time / 4);
        Endpoint_ClearIN();

        Endpoint_ClearStatusStage();
      }
      break;
  }

  CDC_Device_ProcessControlRequest(&console_interface);
}

// Called every milli second by LUFA.
void EVENT_USB_Device_StartOfFrame(void) {
  if (idle_time_remaining)
    --idle_time_remaining;

  TimersUpdate();
  MatrixUpdate();
}
