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

#include <LUFA/Drivers/Misc/RingBuffer.h>
#include <LUFA/Drivers/USB/USB.h>

#include "../report.h"

class Usb {
 public:
  static inline void Init() {
    RingBuffer_InitBuffer(&console_send_rb_, console_send_rb_data_,
        sizeof(console_send_rb_data_));
    USB_Init();
  }

  static void Update();

  // Gets current report data from Report class and sends it to the host.
  static void SendReport();

  // Write single byte to console endpoint.
  static void ConsoleWrite(u8 byte);

  // Called by every ms by EVENT_USB_Device_StartOfFrame().
  static inline void Tick() {
    if (idle_time_remaining_)
      idle_time_remaining_--;
  }

  static inline bool start_of_frame() {
    if (start_of_frame_) {
      start_of_frame_ = false;
      return true;
    }
    return false;
  }
  static inline void set_start_of_frame(bool start_of_frame) {
    start_of_frame_ = start_of_frame;
  }

  // HID Specification sends idle time in a numbler multiple of 4ms.
  static inline u16 idle_time() { return idle_time_ / 4; }
  static inline void set_idle_time(u8 idle_time) { idle_time_ = idle_time * 4; }

 private:
  static const u8 kConsoleSendRingBufferSize = 128;

  // volatile because it is set by an interrupt.
  static volatile bool start_of_frame_;

  // Current idle period in ms. This is set by the host via a Set Idle HID class
  // request to silence the device's reports for either the entire idle
  // duration, or until the report status changes (e.g. the user presses a key).
  static u16 idle_time_;
  static u16 idle_time_remaining_;

  static u8 prev_data_[Report::kDataSize];

  static RingBuffer_t console_send_rb_;
  static u8 console_send_rb_data_[kConsoleSendRingBufferSize];
};

extern "C" {
  void EVENT_USB_Device_ConfigurationChanged(void);
  void EVENT_USB_Device_ControlRequest(void);
  void EVENT_USB_Device_StartOfFrame(void);
}

#endif // CFKW_SRC_USB_USB_H_
