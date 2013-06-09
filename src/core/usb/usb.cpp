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
#include "descriptors.h"

FILE console;

volatile bool Usb::start_of_frame_ = false;

// HID Specification 1.11:
// "The recommended default idle rate (rate when the device is initialized) is
// 500 milliseconds for keyboards (delay before first repeat rate)"
u16 Usb::idle_time_ = 500;
u16 Usb::idle_time_remaining_ = 0;

u8 Usb::prev_data_[] = { 0 };

RingBuffer_t Usb::console_send_rb_ = { 0 };
u8 Usb::console_send_rb_data_[] = { 0 };

void Usb::UpdateEndpoints() {
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  Endpoint_SelectEndpoint(KEYBOARD_EPADDR);
  if (Endpoint_IsINReady())
    WriteKeyboardEndpoint();

  Endpoint_SelectEndpoint(CONSOLE_IN_EPADDR);
  if (Endpoint_IsINReady())
    WriteConsoleEndpoint();
}

int Usb::ConsoleWrite(char byte, FILE *unused) {
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return 1;

  Endpoint_SelectEndpoint(CONSOLE_IN_EPADDR);

  // Try to send all queued bytes.
  while (Endpoint_IsReadWriteAllowed() &&
         !RingBuffer_IsEmpty(&console_send_rb_)) {
    Endpoint_Write_8(RingBuffer_Remove(&console_send_rb_));
  }

  // If number of bytes in queue is greater then enpoint size clear IN.
  if (!Endpoint_IsReadWriteAllowed())
    Endpoint_ClearIN();

  if (Endpoint_IsReadWriteAllowed()) {
    // If possible write byte to bank directly.
    Endpoint_Write_8(byte);
  } else {
    Endpoint_ClearIN();
    if (RingBuffer_IsFull(&console_send_rb_))
      return 1;
    else
      RingBuffer_Insert(&console_send_rb_, byte);
  }

  return 0;
}

void Usb::WriteKeyboardEndpoint() {
  Endpoint_SelectEndpoint(KEYBOARD_EPADDR);

  while (!Endpoint_IsReadWriteAllowed());

  void *curr_data = Report::data();
  bool send_data = false;
  // Send report to host if either report has changed...
  if (memcmp(prev_data_, curr_data, Report::kDataSize)) {
    // Get new report.
    memcpy(prev_data_, curr_data, Report::kDataSize);
    send_data = true;
  // ... or idle time is over.
  } else if (idle_time_remaining_ == 0) {
    // Just send the same report again.
    send_data = true;
  }

  if (send_data) {
    Endpoint_Write_Stream_LE(curr_data, Report::kDataSize, NULL);
    idle_time_remaining_ = idle_time_;
  }

  // Finalize stream.
  Endpoint_ClearIN();
}

void Usb::WriteConsoleEndpoint() {
  Endpoint_SelectEndpoint(CONSOLE_IN_EPADDR);

  // Try to send all queued bytes.
  while (Endpoint_IsReadWriteAllowed() &&
         !RingBuffer_IsEmpty(&console_send_rb_)) {
    Endpoint_Write_8(RingBuffer_Remove(&console_send_rb_));
  }

  // If bank is not filled yet, fill it with zeros to prevent hid_listen
  // from disconnecting.
  for (u8 i = Endpoint_BytesInEndpoint(); i < CONSOLE_EPSIZE; i++) {
    Endpoint_Write_8(0);
  }

  Endpoint_ClearIN();
}

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
        Endpoint_Write_Control_Stream_LE(Report::data(), Report::kDataSize);

        // Manually clear the status stage
        Endpoint_ClearOUT();
      }
      break;

    case HID_REQ_SetIdle:
      if (USB_ControlRequest.bmRequestType ==
          (REQDIR_HOSTTODEVICE | REQTYPE_CLASS | REQREC_INTERFACE)) {
        Endpoint_ClearSETUP();
        Endpoint_ClearStatusStage();

        // Get idle period in MSB.
        Usb::set_idle_time(USB_ControlRequest.wValue >> 8);
      }
      break;

    case HID_REQ_GetIdle:
      if (USB_ControlRequest.bmRequestType ==
          (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE)) {
        Endpoint_ClearSETUP();

        Endpoint_Write_8(Usb::idle_time());

        Endpoint_ClearIN();
        Endpoint_ClearStatusStage();
      }
      break;
  }
}

void EVENT_USB_Device_StartOfFrame(void) {
  Usb::Tick();
}
