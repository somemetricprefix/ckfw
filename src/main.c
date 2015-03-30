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
#include <avr/wdt.h>
#include <stdio.h>
#include <util/delay.h>

#include "common.h"
#include "matrix.h"
#include "eventqueue.h"
#include "usb/usb.h"

__attribute__((weak))
void Init(void) {}

__attribute__((weak))
void KeyEvent(Event event) {}

int main(void)
{
  // Disable watchdog if enabled by bootloader/fuses.
  BIT_CLR(MCUSR, WDRF);
  wdt_disable();

  // Disable clock division
  clock_prescale_set(clock_div_1);

  // Setup core modules.
  MatrixInit();
  UsbInit();

  // Setup keyboard model specific things.
  Init();

  // Enable interrupts.
  sei();

  for (;;) {
    // Process all events in queue as fast as possible.
    while (!EventQueueEmpty()) {
      KeyEvent(EventQueueRead());
    }

    // Enter bootloader mode if requested over serieal port.
    // getc() is nonblocking here.
    if (getc(&console) == 'b') {
      USB_Disable();
      cli();
      _delay_ms(100);  // Wait for the USB detachment to register on the host.
      asm volatile("jmp 0x7E00");
    }

    // Send reports and debug messages to host.
    UsbTask();
  }
}
