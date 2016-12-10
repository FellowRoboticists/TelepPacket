#include <Arduino.h>
#include "CommandPacket.h"

//#include <iostream>
//using namespace std;

CommandPacket::CommandPacket(uint8_t commandByte, uint8_t commandLength) {
  mCommandByte = commandByte;
  mCommandLength = commandLength;
  reset();
}

void CommandPacket::reset() {
  mCurrentIndex = 0;
  mValueIndex = 0;
}

void CommandPacket::append(uint8_t b) {
  if (mCurrentIndex < MAX_PACKET_SIZE) {
    mBuffer[mCurrentIndex++] = b;
  }
}

uint8_t CommandPacket::length() {
  return mCurrentIndex;
}

uint16_t CommandPacket::nextValue(uint8_t valueLength) {
  if (mValueIndex == 0) {
    mValueIndex = 1;
  } else if (mValueIndex >= (mCurrentIndex - 1)) {
    mValueIndex = -1;
  }

  uint16_t value = 0xffff;
  if (mValueIndex > 0 && mValueIndex < MAX_PACKET_SIZE) {
    if (valueLength == 1) {
      value = mBuffer[mValueIndex++];
    } else if (valueLength == 2) {
      value = mBuffer[mValueIndex++] << 8;
      value |= mBuffer[mValueIndex++];
    }
  }

  return value;
}

boolean CommandPacket::valid() {
  return (length() == mCommandLength) && (mBuffer[0] == mCommandByte);
}

boolean CommandPacket::readCompleted() {
  return mCurrentIndex == mCommandLength;
}

/* static */ boolean CommandPacket::read(Stream &s, CommandPacket& packet) {
  int bytesAvailable = s.available();
  for (int i=0; i<bytesAvailable; i++) {
    uint8_t b = s.read();
    packet.append(b);
    if (packet.readCompleted()) {
      break; // No need to read more
    }
  }
  return packet.readCompleted() && packet.valid();
}
