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

#ifndef CFKW_SRC_USB_USB_H_
#define CFKW_SRC_USB_USB_H_

#include <LUFA/Drivers/USB/USB.h>

#include "../common.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initializes all internal usb resources.
void UsbInit(void);

// Reads and writes endpoint data.
void UsbTask(void);

// Sends current report to master if it has changed. If sync is true this
// function blocks until the report can be sent (max 1ms). If sync is false
// the report is only sent if the endpoint can be written and the funtion
// returns immediately.
void UsbSendReport(bool sync);

// LUFA event handlers.
void EVENT_USB_Device_ConfigurationChanged(void);
void EVENT_USB_Device_ControlRequest(void);
void EVENT_USB_Device_StartOfFrame(void);

#ifdef __cplusplus
}
#endif

#endif  // CFKW_SRC_USB_USB_H_
