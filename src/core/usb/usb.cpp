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
#include "../ringbuffer.h"
#include "../timer.h"
#include "descriptors.h"

// Console can be used with stdio functions.
FILE console;

namespace usb {

// Current idle period in ms. This is set by the host via a Set Idle HID class
// request to silence the device's reports for either the entire idle
// duration, or until the report status changes (e.g. the user presses a key).
// HID Specification 1.11:
// "The recommended default idle rate (rate when the device is initialized) is
// 500 milliseconds for keyboards (delay before first repeat rate)"
static u16 idle_time = 500;
static u16 idle_time_remaining = 0;

static const uint kConsoleSendBufferSize = 256;
static u8 console_send_rb_data[kConsoleSendBufferSize];
static RingBuffer<u8> console_send_rb(console_send_rb_data,
                                      kConsoleSendBufferSize);

// Writes single byte to console endpoint.
static int ConsoleWrite(char byte, FILE *unused) {
  return !console_send_rb.Write(byte);
}

void Init() {
  // Setup a FILE to use stdio functions for the console.
  fdev_setup_stream(&console, ConsoleWrite, nullptr, _FDEV_SETUP_WRITE);
  USB_Init();
}

// Gets current report data and writes it to the keyboard endpoint.
static void WriteKeyboardEndpoint() {
  Endpoint_SelectEndpoint(KEYBOARD_EPADDR);
  if (!Endpoint_IsINReady())
    return;

  // Store last sent report statically to get to know if data has changed.
  static u8 prev_data[report::kDataSize];
  void *curr_data = report::data;
  bool send_data = false;

  // Send report to host if either report has changed...
  if (memcmp(prev_data, curr_data, report::kDataSize)) {
    // Get new report.
    memcpy(prev_data, curr_data, report::kDataSize);
    send_data = true;
  // ... or idle time is over.
  } else if (idle_time_remaining == 0) {
    // Just send the same report again.
    send_data = true;
  }

  if (send_data) {
    Endpoint_Write_Stream_LE(curr_data, report::kDataSize, NULL);
    idle_time_remaining = idle_time;
  }

  // Finalize stream.
  Endpoint_ClearIN();
}

// Writes console IN endpoint. hid_listen tool needs a full IN endpoint all the
// time.
static void WriteConsoleEndpoint() {
  Endpoint_SelectEndpoint(CONSOLE_IN_EPADDR);
  if (!Endpoint_IsINReady())
    return;

  // Fill the bank with...
  while(Endpoint_IsReadWriteAllowed()) {
    if (!console_send_rb.Empty()) {
      // ...either the current buffered bytes or with...
      Endpoint_Write_8(console_send_rb.Read());
    } else {
      // ...zeros keep the connection to hid_listen program alive.
      Endpoint_Write_8(0);
    }
  }

  Endpoint_ClearIN();
}

void Task() {
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  WriteKeyboardEndpoint();
  WriteConsoleEndpoint();
}

// Registers all endpoints and enables SOF interrupt.
void EVENT_USB_Device_ConfigurationChanged(void) {
  Endpoint_ConfigureEndpoint(KEYBOARD_EPADDR, EP_TYPE_INTERRUPT,
                             KEYBOARD_EPSIZE, 1);
  Endpoint_ConfigureEndpoint(CONSOLE_IN_EPADDR, EP_TYPE_INTERRUPT,
                             CONSOLE_EPSIZE, 2);
  Endpoint_ConfigureEndpoint(CONSOLE_OUT_EPADDR, EP_TYPE_INTERRUPT,
                             CONSOLE_EPSIZE, 1);
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
        Endpoint_ClearSETUP();

        // Write the report data to the control endpoint
        // The functions waits for the host to enter the status stage
        Endpoint_Write_Control_Stream_LE(report::data, report::kDataSize);

        // Manually clear the status stage
        Endpoint_ClearOUT();

        LOG_DEBUG("HID Request: GetReport");
      }
      break;

    case HID_REQ_SetIdle:
      if (USB_ControlRequest.bmRequestType ==
          (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) {
        Endpoint_ClearSETUP();
        Endpoint_ClearStatusStage();

        // Idle period in MSB. HID Specification sends idle time in a numbler
        // multiple of 4ms.
        idle_time = (USB_ControlRequest.wValue >> 8) * 4;

        LOG_DEBUG("HID Request: SetIdle - %ums", idle_time);
      }
      break;

    case HID_REQ_GetIdle:
      if (USB_ControlRequest.bmRequestType ==
          (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE)) {
        Endpoint_ClearSETUP();

        // HID Specification sends idle time in a numbler multiple of 4ms.
        Endpoint_Write_8(idle_time / 4);

        Endpoint_ClearIN();
        Endpoint_ClearStatusStage();

        LOG_DEBUG("HID Request: GetIdle");
      }
      break;
  }
}

void EVENT_USB_Device_StartOfFrame(void) {
  if (idle_time_remaining)
    --idle_time_remaining;

  Timer::Update();
  matrix::Update();
}

}  // namespace usb
