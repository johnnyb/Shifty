#ifndef Shifty_h
#define Shifty_h

#include <Arduino.h>

class Shifty {
  public:

  Shifty();

  void setBitCount(int bitCount);
  void setPins(int dataPin, int clockPin, int latchPin, int readPin);
  void setPins(int dataPin, int clockPin, int latchPin);
  void setBitMode(int bitnum, bool mode);
  bool getBitMode(int bitnum);
  void batchWriteBegin();
  void batchWriteEnd();
  void batchReadBegin();
  void batchReadEnd();
  void writeBit(int bitnum, bool value);
  bool readBit(int bitnum);

  private:

  int dataPin;
  int clockPin;
  int readPin;
  int latchPin;
  
  int bitCount;
  int byteCount;
  byte writeBuffer[16];
  byte dataModes[16];
  byte readBuffer[16];
  bool batchWriteMode;
  bool batchReadMode;

  void writeAllBits();
  void readAllBits();
  void writeBitHard(int bitnum, bool value);
  void writeBitSoft(int bitnum, bool value);
  bool readBitHard(int bitnum);
  bool readBitSoft(int bitnum);
};

#endif
