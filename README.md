Shifty
======
A Flexible 74HC595 Manager for Arduino
--------------------------------------

The Shifty library for Arduino is a very flexible way to manage 74HC595 shift registers.  It allows you to write to individual outputs just like "digitalWrite", allows you to daisy-chain shift registers together, and, if you wire it according to the instructions in this document, allows you to use your shift register for *both* input *and* output pins with only one additional pin used.  This makes it ideal for usage with an ATTiny, though it does use up a bit of space on the device.

### Basic Usage

If you have a single shift register with data on pin 11, clock on pin 12, and latch on pin 8, a simple program to blink Q3 on the register would look like this:

    Shifty myreg;

    void setup() {
      myreg.setBitCount(8);
      myreg.setPins(11, 12, 8);
    }

    void loop() {
      myreg.bitWrite(3, HIGH);
      delay(500);
      myreg.bitWrite(3, LOW);
      delay(500);
    }

### Batch Mode

In order to make reading/writing bits on your shift register simpler, Shifty buffers your output, and then rewrites all your output every time you set a bit.  However, if you wanted to speed this up, you can wrap a number of bit sets into a single batch:

    void loop() {
      myreg.batchWriteBegin();
      myreg.bitWrite(3, HIGH);
      myreg.bitWrite(4, LOW);
      myreg.bitWrite(5, HIGH);
      myreg.bitWrite(8, LOW);
      myreg.batchWriteEnd();
    }

This sets the bits on Q3, Q4, Q5, and Q8, but doesn't push the results out until you give a batchWriteEnd() command.

### Daisy-chaining Shift Registers Together

Additionally, if you daisy-chain two 74HC595's together, you can use the same library, just set it to have 16 bits instead of 8:

    Shifty myreg;

    void setup() {
      myreg.setBitCount(16);
      myreg.setPins(11, 12, 8);
    }

You can have as many 74HC595's linked together as you wish, barring electrical issues.  However, with more shift registers, the time it takes to push out the results increases.  For every batchWriteEnd() (or every single bitWrite if you are not in a batch) it has to write to *every* shift register bit.

### Input Pins

Using an output shift register for input was inspired by this YouTube video by Kevin Darrah:

http://www.youtube.com/watch?v=nXl4fb_LbcI

Note that the input is still under active development and may or may not be functional at the moment.
