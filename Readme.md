Hardware
--------

punchtape.dxf has the basic mechanical design of the read head. The head is bolted together with M3 hardware.

boards/5mm-phototransistors contains Eagle CAD files for the LED emitter and phototransistor boards.

Software
--------

The reader is implemented as an Arduino sketch in punchtape-reader/punchtape-reader.ino. I'm using a Teensy++ 2.0
at present but it should work with any ordinary Arduino variant. You'll have to alter the pin assignments at the top
of the file, though.
