Shifty
======
A Flexible 74HC595 Manager for Arduino
--------------------------------------

The Shifty library for Arduino is a very flexible way to manage 74HC595 shift registers.  It allows you to write to individual outputs just like "digitalWrite", allows you to daisy-chain shift registers together, and, if you wire it according to the instructions in this document, allows you to use your shift register for *both* input *and* output pins with only one additional pin used.  This makes it ideal for usage with an ATTiny, though it does use up a bit of space on the device.

### Inspiration

This is inspired first by the ATTiny85, which is a very awesome, if limiting, chip to work with.  Shift registers are almost required to do anything fancy with the ATTiny85.  However, a shift register eats up almost all of the pins, making input difficult.  Then I stumbled upon Kevin Darrah's video on how to use output shift registers for reading inputs.  The problem was that the code was very obscure.  Therefore, I decided to write a library that encapsulates these ideas into an easy-to-use form.


### Basic Usage

If you have a single shift register with data on pin 11, clock on pin 12, and latch on pin 8, a simple program to blink Q3 on the register would look like this:

    Shifty myreg;

    void setup() {
      myreg.setBitCount(8);
      myreg.setPins(11, 12, 8);
    }

    void loop() {
      myreg.writeBit(3, HIGH);
      delay(500);
      myreg.writeBit(3, LOW);
      delay(500);
    }

### Batch Mode

In order to make reading/writing bits on your shift register simpler, Shifty buffers your output, and then rewrites all your output every time you set a bit.  However, if you wanted to speed this up, you can wrap a number of bit sets into a single batch:

    void loop() {
      myreg.batchWriteBegin();
      myreg.writeBit(3, HIGH);
      myreg.writeBit(4, LOW);
      myreg.writeBit(5, HIGH);
      myreg.writeBit(8, LOW);
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

You can have as many 74HC595's linked together as you wish, barring electrical issues.  Technically, the library limits you to 16, but that is easy enough to change.  However, with more shift registers, the time it takes to push out the results increases.  For every batchWriteEnd() (or every single writeBit if you are not in a batch) it has to write to *every* shift register bit.

### Input Pins

Using an output shift register for input was inspired by this YouTube video by Kevin Darrah:

http://www.youtube.com/watch?v=nXl4fb_LbcI

Note that the input is still under active development and may or may not be functional at the moment.

Here is how the API is setup to use.  First of all, this requires the usage of a single input pin to receive input feedback.  Therefore, setPins() requires an extra argument - the last number will be the pin used for the input channel.  Additionally, each bit that you plan to use for input needs to be marked with bitMode().

    Shifty myreg;
    void setup() {
      myreg.setBitCount(8);
      myreg.setPins(11, 12, 8, 9);
      myreg.bitMode(4, INPUT);
      myreg.bitMode(6, INPUT);
    }

This sets up Q4 and Q6 on the chip to be used as input bits.  Note that the register itself does not support input, instead you have to wire it as mentioned below (or in the video) to get it to be used as an input bit.  In short, what happens is that the "input" bit is set to HIGH, and we then check to see if we can read the HIGH signal on the input pin to see the state of that particular pin.

Reading pins this was is slow, but the ATTiny is fast enough that it usually doesn't matter.

So, to read pins, do:

    void loop() {
      if(myreg.readBit(4) == HIGH) {
        myreg.writeBit(5, HIGH);
      } else { 
        myreg.writeBit(5, LOW);
      }
    }

### Wiring Your Project for Shift Register Inputs

Fixme - need to write this.
