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

#ifndef CKFW_SRC_IOPORT_H_
#define CKFW_SRC_IOPORT_H_

// If _SFR_ASM_COMPAT is set register names are constants and have to be casted
#define REG_BIT_SET(reg, bit) BIT_SET(*(volatile u8 *)(u16)(reg), bit)
#define REG_BIT_CLR(reg, bit) BIT_CLR(*(volatile u8 *)(u16)(reg), bit)
#define REG_BIT_IS_SET(reg, bit) BIT_IS_SET(*(volatile u8 *)(u16)(reg), bit)

// Macro used to initialize I/O ports.
// p is the letter of the avr I/O port
// b is the nth bit of that port.
#define IO_PORT(p, b) \
{ PORT##p, DDR##p, PIN##p, b }

// Header only class that abstracts the general digital I/O functionality
// of I/O ports.
class IoPort {
 public:
  // All arguments are the adress of the port registers.
  // To get constant adresses from avr-libc _SFR_ASM_COMPAT must be set to 1.
  IoPort(u8 port, u8 ddr, u8 pin, u8 bit)
      : port_(port),
        ddr_(ddr),
        pin_(pin),
        bit_(bit)
    {}

  // Enable the internal pull-up resistors.
  inline void PullUp() const {
    REG_BIT_CLR(ddr_, bit_);
    REG_BIT_SET(port_, bit_);
  }

  // Sets the pin to output mode and sets it to low.
  inline void WriteLow() const {
    REG_BIT_SET(ddr_, bit_);
    REG_BIT_CLR(port_, bit_);
  }

  // Sets the pin to output mode and sets it to high.
  inline void WriteHigh() const {
    REG_BIT_SET(ddr_, bit_);
    REG_BIT_CLR(port_, bit_);
  }

  // Returns true for high level at pin, false for low level at pin.
  inline bool Read() const {
    return REG_BIT_IS_SET(pin_, bit_);
  }

 private:
  const u8 port_;
  const u8 ddr_;
  const u8 pin_;
  const u8 bit_;

  DISALLOW_COPY_AND_ASSIGN(IoPort);
};

#endif // CKFW_SRC_IOPORT_H_
