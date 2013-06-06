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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include "matrix.h"
#include "report.h"
#include "usb/usb.h"

__attribute__((weak))
void Init() {}

__attribute__((weak))
void Tick() {}

#include "keycodes.h"

int main(void)
{
  // Disable watchdog if enabled by bootloader/fuses
  BIT_CLR(MCUSR, WDRF);
  wdt_disable();

  // Disable clock division
  clock_prescale_set(clock_div_1);

  Usb::Init();
  Matrix::Init();
  Init();

  // Enable interrupts. Interrupts are required for USB.
  sei();

  for (;;) {
    if (Usb::start_of_frame()) {
      Matrix::Scan();
      Tick();
      Usb::SendReport();
    }

    sleep_mode();
  }
}
