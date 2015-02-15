#include "Packet.h"

Packet pkt;

void setup()
{
  pkt.reset();
  Packet::read(Serial, pkt);
}

void loop()
{
}
