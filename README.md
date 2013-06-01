ckfw - custom keyboard firmware
===============================

Keyboard firmware for custom made keyboards that use the Atmel AVR atmega32u4
micro processor (the chip on the Teensy 2.0 development board).

Why ckfw?
---------

There are plenty keyboard firmwares around already (see Similar Projects). This
may rise the question why should there be yet another keyboard firmware? The
answer is simple: This project is mainly an excercise for the author to improve
his C++ skills and implement features that are hard to do within the
t.m.k keymap framework.  
The author is happy if you use _ckfw_ for your custom keyboard even though
he recommends using the _t.m.k Keyboard Firmware_ for most projects (for now)
because _ckfw_ is not nearly as feature complete and well tested yet.

Code Style
----------

Preferred formattig is described by the [Google C++ Style Guide](/c++-style/).
Filetype should be .cpp instead of .cc, because the LUFA build system only
recognizes .cpp files.

Similar Projects
----------------

* [t.m.k. Keyboard Firmware](https://github.com/somemetricprefix/tmk_keyboard)
* [ergodox firmware](https://github.com/benblazak/ergodox-firmware)
* [humble hacker firmware](https://github.com/humblehacker/firmware)

License
-------

Code of this project is licensed under the ISC license.
LUFA has its own license.

[c++-style]: http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml

