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

#include <LUFA/Drivers/USB/USB.h>

#include "../common.h"
#include "../report.h"
#include "descriptors.h"

void Usb::Init()
{
  USB_Init();
}

void Usb::SendReport()
{
  if (USB_DeviceState != DEVICE_STATE_Configured)
    return;

  Endpoint_SelectEndpoint(KEYBOARD_EPADDR);

  if (!Endpoint_IsReadWriteAllowed())
    return;

  // Get and write report data
  Endpoint_Write_Stream_LE(Report::get_data(), Report::kDataSize, NULL);

  // Finalize stream
  Endpoint_ClearIN();
}

// Current idle period in ms. This is set by the host via a Set Idle HID class
// request to silence the device's reports for either the entire idle duration,
// or until the report status changes (e.g. the user presses a key).
// For now this is ignored and reports are send whenever possible.
static u16 idle_time = 0;

void EVENT_USB_Device_ConfigurationChanged(void)
{
  Endpoint_ConfigureEndpoint(KEYBOARD_EPADDR, EP_TYPE_INTERRUPT,
                             KEYBOARD_EPSIZE, 1);
}

// Appendix G: HID Request Support Requirements
// For a Non-Boot Keyboard only GET_REPORT, SET_IDLE, GET_IDLE control requests
// are required to be implemented.
void EVENT_USB_Device_ControlRequest(void)
{
  // Handle HID specific requests.
  switch (USB_ControlRequest.bRequest)
  {
    case HID_REQ_GetReport:
      ASSERT(USB_ControlRequest.bmRequestType ==
                 (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE));

      Endpoint_ClearSETUP();

      // Write the report data to the control endpoint
      // The functions waits for the host to enter the status stage
      Endpoint_Write_Control_Stream_LE(Report::get_data(), Report::kDataSize);

      // Manually clear the status stage
      Endpoint_ClearOUT();

      break;

    case HID_REQ_SetIdle:
      ASSERT(USB_ControlRequest.bmRequestType ==
                 (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE));

      Endpoint_ClearSETUP();
      Endpoint_ClearStatusStage();

      // Get idle period in MSB, IdleCount must be multiplied by 4 to get number
      // of milliseconds
      idle_time = ((USB_ControlRequest.wValue & 0xFF00) / 8);

      break;

    case HID_REQ_GetIdle:
      ASSERT(USB_ControlRequest.bmRequestType ==
                 (REQDIR_DEVICETOHOST | REQTYPE_CLASS | REQREC_INTERFACE));

      Endpoint_ClearSETUP();

      // Write the current idle duration to the host, must be divided by 4
      // before sent to host
      Endpoint_Write_8(idle_time / 4);

      Endpoint_ClearIN();
      Endpoint_ClearStatusStage();

      break;
  }
}
