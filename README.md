ckfw - custom keyboard firmware
===============================

Keyboard firmware for custom made keyboards with the Atmel AVR atmega32u4 micro
processor (the chip on the Teensy 2.0 development board).

Why ckfw?
---------

There are plenty keyboard firmwares around already (see Similar Projects). So
why on eath do we need another Teensy 2.0 compatible firmware? The answer is
simple: We don’t. This project is mainly an excercise for the author to improve
his C / C++ skills.
This firmware is highly unstable and interfaces may still change a lot. For
serios projects *t.m.k Keyboard Firmware* is recommended. It’s a great firmware
with plenty of features and it’s been well adopted by the DIY community.

Code Style
----------

Preferred formattig is described in the [Google C++ Style Guide](/c++-style/).
Filetype should be .cpp instead of .cc, because the LUFA build system only
recognizes .cpp files.

[c++-style]: http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml

Similar Projects
----------------

* [t.m.k. Keyboard Firmware](https://github.com/somemetricprefix/tmk_keyboard)
* [ergodox firmware](https://github.com/benblazak/ergodox-firmware)
* [humble hacker firmware](https://github.com/humblehacker/firmware)

License
-------

Code of this project is licensed under the ISC license. Some files which were
taken or modified from other projects may have other licenses. The used licenses
are documented at the beginning of each file. The libraries in lib/ use their
own license respectively.
