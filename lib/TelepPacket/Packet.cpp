#include <Arduino.h>
#include "Packet.h"
//#include <iostream>
//using namespace std;

Packet::Packet() {
  reset();
}

Packet::Packet(uint8_t *buffer, uint16_t length) : Packet(buffer, length, true) {
}

Packet::Packet(uint8_t *buffer, uint16_t length, boolean completePacket) {
  reset();
  for (int i=0; i<length; i++) {
    append(buffer[i]);
  }
  if (completePacket) {
    complete();
  }
}

void Packet::reset() {
  mBuffer[0] = PKT_START_BYTE;
  mCurrentIndex = 1;
}

void Packet::resetToStartByte() {
  uint16_t startByte = 0;
  for (int i=1; i<mCurrentIndex && startByte == 0; i++) {
    if (PKT_START_BYTE == mBuffer[i]) {
      startByte = i;
    }
  }
  if (startByte > 0) {
    uint16_t pos = 1;
    for (int i=startByte + 1; i<mCurrentIndex; i++) {
      mBuffer[pos++] = mBuffer[i];
    }
    mCurrentIndex = mCurrentIndex - startByte;
  } else {
    reset();
  }
}

void Packet::append(uint8_t b) {
  if (mCurrentIndex < MAX_PACKET_SIZE) {
    mBuffer[mCurrentIndex++] = b;
  }
}

uint16_t Packet::length() {
  return mCurrentIndex;
}

void Packet::complete() {
  append(calculateChecksum());
}

boolean Packet::valid() {
  uint8_t sum = 0;

  for (int i=0; i<(mCurrentIndex - 1); i++) {
    sum += mBuffer[i];
  }

  return mBuffer[0] == PKT_START_BYTE && 
    readCompleted() &&
    sum == mBuffer[mCurrentIndex - 1];
}

uint8_t Packet::calculateChecksum() {
  uint8_t sum = 0;
  for (int i=0; i<mCurrentIndex; i++) {
    sum += mBuffer[i];
  }
  return sum;
}

boolean Packet::readCompleted() {
  if (mCurrentIndex <= PKT_LENGTH_BYTE) {
    return false;
  }

  if (mBuffer[PKT_LENGTH_BYTE] + 3 > MAX_PACKET_SIZE) {
    return true;
  }

  return mBuffer[PKT_LENGTH_BYTE] + 3 == mCurrentIndex;
}

void Packet::write(Stream& s) {
  s.write(mBuffer, length());
}

uint8_t Packet::valueAt(uint16_t index) {
  if (index < 0 || index >= mCurrentIndex) {
    return 0x00;
  } else {
    return mBuffer[index];
  }
}
 
/* static */ boolean Packet::read(Stream& s, Packet& packet) {
  boolean started = packet.length() > 1;
  int bytesAvailable = s.available();
  for (int i=0; i<bytesAvailable; i++) {
    uint8_t b = s.read();
    if (started) {
      packet.append(b);
      if (packet.readCompleted()) {
        break; // No need to read more
      }
    } else if (b == PKT_START_BYTE) {
      started = true;
    }
  }
  return packet.readCompleted() && packet.valid();
}
