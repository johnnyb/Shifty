#include <Shifty.h>

Shifty::Shifty() {
}

void Shifty::setBitCount(int bitCount) {
  this->bitCount = bitCount;
  this->byteCount = bitCount/8;
  for(int i = 0; i < this->byteCount; i++) {
    this->writeBuffer[i] = 0;
    this->dataModes[i] = 0;
    this->readBuffer[i] = 0;
  }
}  

void Shifty::setPins(int dataPin, int clockPin, int latchPin, int readPin) {
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(readPin, INPUT);
  this->dataPin = dataPin;
  this->clockPin = clockPin;
  this->latchPin = latchPin;
  if(readPin != -1) {
    this->readPin = readPin;
  }
}

void Shifty::setPins(int dataPin, int clockPin, int latchPin) {
  setPins(dataPin, clockPin, latchPin, -1);
}


void Shifty::batchWriteBegin() {
  batchWriteMode = true;
}

void Shifty::writeBit(int bitnum, bool value) {
  if(batchWriteMode) {
    writeBitSoft(bitnum, value);
  } else {
    writeBitHard(bitnum, value);
  }
}

void Shifty::batchWriteEnd() {
  writeAllBits();
  batchWriteMode = false;
}

void Shifty::batchReadBegin() {
  batchReadMode = true;
  readAllBits();
}

bool Shifty::readBit(int bitnum) {
  if(batchReadMode) {
    return readBitSoft(bitnum);
  } else {
    return readBitHard(bitnum);
  }
}

void Shifty::batchReadEnd() {
  batchReadMode = false;
}

void Shifty::setBitMode(int bitnum, bool mode) {
  int bytenum = bitnum / 8;
  int offset = bitnum % 8;
  byte b = this->dataModes[bytenum];
  bitSet(b, offset);
  this->dataModes[bytenum] = b;
}

bool Shifty::getBitMode(int bitnum){ //true == input
  int bytenum = bitnum / 8; // get working byte offset
  int offset = bitnum % 8;  // get working bit offset
  byte b = this->dataModes[bytenum]; //set b to working byte
  return bitRead(this->dataModes[bytenum], offset);
}

void Shifty::writeBitSoft(int bitnum, bool value) {
  int bytenum = bitnum / 8;
  int offset = bitnum % 8;
  byte b = this->writeBuffer[bytenum];
  bitWrite(b, offset, value);
  this->writeBuffer[bytenum] = b;
}

void Shifty::writeBitHard(int bitnum, bool value) {
  writeBitSoft(bitnum, value);
  writeAllBits();
}

void Shifty::writeAllBits() {
  digitalWrite(latchPin, LOW);
  digitalWrite(clockPin, LOW);
  for(int i = 0; i < this->byteCount; i++) {
    shiftOut(dataPin, clockPin, MSBFIRST, this->writeBuffer[i]);
  }
  digitalWrite(latchPin, HIGH);
}

bool Shifty::readBitSoft(int bitnum) {
  int bytenum = bitnum / 8;
  int offset = bitnum % 8;

  return bitRead(this->readBuffer[bytenum], offset);
}

bool Shifty::readBitHard(int bitnum) {
  int bytenum = bitnum / 8;
  int offset = bitnum % 8;

  // To read the bit, set all output pins except the pin we are looking at to 0
  for(int i = 0; i < this->byteCount; i++) {
    byte mask = this->dataModes[i];
    byte outb = this->writeBuffer[i];
    for(int j = 0; j < 8; j++) {
      if(bitRead(mask, j)) {
        if(i == bytenum && j == bitnum) {
          bitSet(outb, j);
        } else {
          bitClear(outb, j);
        }
      }
    }
  }

  // Flush
  writeAllBits();

  // Get our data pin
  bool value = digitalRead(this->readPin);

  // Set the cached value
  byte cacheb = this->readBuffer[bytenum];
  bitWrite(cacheb, offset, value);
  this->readBuffer[bytenum] = cacheb;

  return value;
}

void Shifty::readAllBits() {
  for(int i = 0; i < this->byteCount; i++) {
    byte mask = this->dataModes[i];
    byte outb = this->writeBuffer[i];
    byte inb = 0;
    for(int j = 0; j < 8; j++) {
      if(bitRead(mask, j)) {
        readBitHard(i * 8 + j);
      }
    }
  }
}
