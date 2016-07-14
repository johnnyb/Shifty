#ifndef Shifty_h
#define Shifty_h

#include <Arduino.h>

class Shifty {
  public:


  // NOTE - bitCount MUST be a multiple of 8
  Shifty(int bitCount);
  Shifty();
  ~Shifty();

  void setBitCount(int bitCount);
  void setPins(int dataPin, int clockPin, int latchPin, int readPin);
  void setPins(int dataPin, int clockPin, int latchPin);
  void bitMode(int bitnum, bool mode);
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
  byte *writeBuffer = 0;
  byte *dataModes = 0;
  byte *readBuffer = 0;
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
