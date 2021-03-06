#include <Arduino.h>
#include <iostream>
#include <string>
#include <list>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <netinet/in.h>

#include "Packet.h"
#include "CommandPacket.h"

using namespace CppUnit;
using namespace std;

class TestCommandPacket : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestCommandPacket);
  CPPUNIT_TEST(testConstructors);
  CPPUNIT_TEST(testReadCompleted);
  CPPUNIT_TEST(testValid);
  CPPUNIT_TEST(testRead);
  CPPUNIT_TEST(testNextValue);
  CPPUNIT_TEST_SUITE_END();

  public:

    void setUp(void);
    void tearDown(void);

  protected:

    void testConstructors(void);
    void testReadCompleted(void);
    void testValid(void);
    void testRead(void);
    void testNextValue(void);

  private:

    CommandPacket *mPacket;

};

void TestCommandPacket::setUp(void) {
  mPacket = new CommandPacket(0x49, 5);
}

void TestCommandPacket::tearDown(void) {
  delete mPacket;
}

void TestCommandPacket::testConstructors(void) {

}

void TestCommandPacket::testReadCompleted(void) {
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  mPacket->append(0x49);
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  mPacket->append(0x01);
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  mPacket->append(0x02);
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  mPacket->append(0x03);
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  mPacket->append(0x04);
  CPPUNIT_ASSERT(mPacket->readCompleted());
};

void TestCommandPacket::testValid(void) {
  // Valid packet first
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x49);
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x01);
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x02);
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x03);
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x04);
  CPPUNIT_ASSERT(mPacket->valid());

  mPacket->reset();

  // Now, an inalid packet
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x22);
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x01);
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x02);
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x03);
  CPPUNIT_ASSERT(! mPacket->valid());
  mPacket->append(0x04);
  CPPUNIT_ASSERT(! mPacket->valid());
}

void TestCommandPacket::testRead(void) {
  // Stream contains exactly the packet we want
  Stream s;
  s.append(0x49);
  s.append(0x02);
  s.append(0xf2);
  s.append(0xf3);
  s.append(0x06);

  CPPUNIT_ASSERT(CommandPacket::read(s, *mPacket));
  CPPUNIT_ASSERT(mPacket->readCompleted());
  CPPUNIT_ASSERT(mPacket->valid());

  // Stream contains more than we want
  s.reset();
  s.append(0x49);
  s.append(0x02);
  s.append(0xf2);
  s.append(0xf3);
  s.append(0x06);
  s.append(0x13);
  
  mPacket->reset();
  CPPUNIT_ASSERT(CommandPacket::read(s, *mPacket));
  CPPUNIT_ASSERT(mPacket->readCompleted());
  CPPUNIT_ASSERT(mPacket->valid());

  // Stream contains just short of what we need
  s.reset();
  s.append(0x49);
  s.append(0x02);
  s.append(0xf2);
  s.append(0xf3);

  mPacket->reset();
  CPPUNIT_ASSERT(! CommandPacket::read(s, *mPacket));
  CPPUNIT_ASSERT(! mPacket->readCompleted());

  // We complete the packet using a replenished stream
  s.reset();
  s.append(0x06);
  s.append(0x13);

  // Don't reset the packet
  CPPUNIT_ASSERT(CommandPacket::read(s, *mPacket));
  CPPUNIT_ASSERT(mPacket->readCompleted());
  CPPUNIT_ASSERT(mPacket->valid());
}

void TestCommandPacket::testNextValue(void) {
  mPacket->append(0x49);
  mPacket->append(0x01);
  mPacket->append(0x02);
  mPacket->append(0x03);
  mPacket->append(0x04);
  CPPUNIT_ASSERT(mPacket->valid());

  CPPUNIT_ASSERT(258 == mPacket->nextValue(2));
  CPPUNIT_ASSERT(772 == mPacket->nextValue(2));
}

// ###################################################################
// ###################################################################

class TestPacket : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TestPacket);
  CPPUNIT_TEST(testConstructors);
  CPPUNIT_TEST(testReset);
  CPPUNIT_TEST(testAppend);
  CPPUNIT_TEST(testLength);
  CPPUNIT_TEST(testCalculateChecksum);
  CPPUNIT_TEST(testComplete);
  CPPUNIT_TEST(testValid);
  CPPUNIT_TEST(testReadCompleted);
  CPPUNIT_TEST(testRead);
  CPPUNIT_TEST(testValueAt);
  CPPUNIT_TEST(testNextValue);
  CPPUNIT_TEST(testNextValue1);
  CPPUNIT_TEST_SUITE_END();

  public:

    void setUp(void);
    void tearDown(void);

  protected:
    void testConstructors(void);
    void testLength(void);
    void testAppend(void);
    void testReset(void);
    void testCalculateChecksum(void);
    void testComplete(void);
    void testValid(void);
    void testReadCompleted(void);
    void testRead(void);
    void testValueAt(void);
    void testNextValue(void);
    void testNextValue1(void);

  private:

    Packet *mPacket;

};

void TestPacket::setUp(void) {
  mPacket = new Packet();
}

void TestPacket::tearDown(void) {
  delete mPacket;
}

void TestPacket::testConstructors(void) {
  uint8_t readyBuffer[] ={ 0x02, 0x00, 0x00 };
  uint8_t fullBuffer[] = { 0x02, 0xf2, 0xf3, 0x06 };

  Packet readyPacket(readyBuffer, 3);
  CPPUNIT_ASSERT(readyPacket.valid());
  Packet fullPacket(fullBuffer, 4, false);
  CPPUNIT_ASSERT(fullPacket.valid());
}

void TestPacket::testLength(void) {
  CPPUNIT_ASSERT(1 == mPacket->length());
  mPacket->append(0x01);
  CPPUNIT_ASSERT(2 == mPacket->length());
}

void TestPacket::testAppend(void) {
  // Make sure we don't blow the buffer
  for (int i=0; i<=MAX_PACKET_SIZE; i++) {
    mPacket->append(i);
  }

  CPPUNIT_ASSERT(MAX_PACKET_SIZE == mPacket->length());
}

void TestPacket::testReset(void) {
  mPacket->append(0x01);
  mPacket->append(0x02);
  mPacket->append(0x03);
  CPPUNIT_ASSERT(4 == mPacket->length());
  mPacket->reset();
  CPPUNIT_ASSERT(1 == mPacket->length());
}

void TestPacket::testCalculateChecksum(void) {
  mPacket->append(0x02);
  mPacket->append(0xf2);
  mPacket->append(0xf3);
  CPPUNIT_ASSERT(((0xfa + mPacket->calculateChecksum()) & 0x00ff) == 0);
}

void TestPacket::testComplete(void) {
  mPacket->append(0x02);
  mPacket->append(0xf2);
  mPacket->append(0xf3);
  CPPUNIT_ASSERT(4 == mPacket->length());
  mPacket->complete();
  CPPUNIT_ASSERT(5 == mPacket->length());
}

void TestPacket::testValid(void) {
  mPacket->append(0x02);
  mPacket->append(0xf2);
  mPacket->append(0xf3);
  CPPUNIT_ASSERT(4 == mPacket->length());
  mPacket->complete();
  CPPUNIT_ASSERT(5 == mPacket->length());
  CPPUNIT_ASSERT(mPacket->valid());
}

void TestPacket::testReadCompleted(void) {
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  mPacket->append(0x02);
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  mPacket->append(0xf2);
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  mPacket->append(0xf3);
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  mPacket->complete();
  CPPUNIT_ASSERT(mPacket->readCompleted());
}

void TestPacket::testValueAt(void) {
  mPacket->append(0x02);
  mPacket->append(0xf2);
  mPacket->append(0xf3);
  mPacket->complete();

  CPPUNIT_ASSERT(0x13 == mPacket->valueAt(0));
  CPPUNIT_ASSERT(0x02 == mPacket->valueAt(1));
  CPPUNIT_ASSERT(0xf2 == mPacket->valueAt(2));
  CPPUNIT_ASSERT(0xf3 == mPacket->valueAt(3));
  CPPUNIT_ASSERT(0x06 == mPacket->valueAt(4));
}

void TestPacket::testNextValue(void) {
  mPacket->append(0x05);
  mPacket->append(0x01);
  mPacket->append(0x0e);
  mPacket->append(0x02);
  mPacket->append(0x00);
  mPacket->append(0x0f);
  mPacket->complete();

  CPPUNIT_ASSERT(0x01 == mPacket->nextValue(1)); // Type 1
  CPPUNIT_ASSERT(0x0e == mPacket->nextValue(1)); // Value 1
  CPPUNIT_ASSERT(0x02 == mPacket->nextValue(1)); // Type 2
  CPPUNIT_ASSERT(0x0f == mPacket->nextValue(2)); // Value 2
  uint16_t value = mPacket->nextValue(1);
  // cout << "End of Buffer = " << (int)value << endl;
  CPPUNIT_ASSERT(0xffff == value); // Past end of buffer

}

void TestPacket::testNextValue1(void) {
  uint16_t value1 = 285;
  uint16_t value2 = 391;

  mPacket->append(0x05);
  mPacket->append(0x01);
  mPacket->append(value1 >> 8);
  mPacket->append(value1 & 0x00ff);
  mPacket->append(value2 >> 8);
  mPacket->append(value2 & 0x00ff);
  mPacket->complete();

  CPPUNIT_ASSERT(0x01 == mPacket->nextValue(1)); // Type 1
  CPPUNIT_ASSERT(285 == mPacket->nextValue(2)); // Value 1
  CPPUNIT_ASSERT(391 == mPacket->nextValue(2)); // Value 2
  uint16_t value = mPacket->nextValue(1);
  // cout << "End of Buffer = " << (int)value << endl;
  CPPUNIT_ASSERT(0xffff == value); // Past end of buffer

}

void TestPacket::testRead(void) {
  // Stream contains exactly the packet we want
  Stream s;
  s.append(0x13);
  s.append(0x02);
  s.append(0xf2);
  s.append(0xf3);
  s.append(0x06);

  CPPUNIT_ASSERT(Packet::read(s, *mPacket));
  CPPUNIT_ASSERT(mPacket->readCompleted());
  CPPUNIT_ASSERT(mPacket->valid());

  // Stream contains more than we want
  s.reset();
  s.append(0x13);
  s.append(0x02);
  s.append(0xf2);
  s.append(0xf3);
  s.append(0x06);
  s.append(0x13);
  
  mPacket->reset();
  CPPUNIT_ASSERT(Packet::read(s, *mPacket));
  CPPUNIT_ASSERT(mPacket->readCompleted());
  CPPUNIT_ASSERT(mPacket->valid());

  // Stream contains just short of what we need
  s.reset();
  s.append(0x13);
  s.append(0x02);
  s.append(0xf2);
  s.append(0xf3);

  mPacket->reset();
  CPPUNIT_ASSERT(! Packet::read(s, *mPacket));
  CPPUNIT_ASSERT(! mPacket->readCompleted());

  // We complete the packet using a replenished stream
  s.reset();
  s.append(0x06);
  s.append(0x13);

  // Don't reset the packet
  CPPUNIT_ASSERT(Packet::read(s, *mPacket));
  CPPUNIT_ASSERT(mPacket->readCompleted());
  CPPUNIT_ASSERT(mPacket->valid());

  // Should pick up a packet even if the stream doesn't
  // start correctly
  // Stream contains exactly the packet we want
  s.reset();
  s.append(0xff); // not the right start byte
  s.append(0x12); // Nor this one
  s.append(0x13);
  s.append(0x02);
  s.append(0xf2);
  s.append(0xf3);
  s.append(0x06);

  mPacket->reset();
  CPPUNIT_ASSERT(Packet::read(s, *mPacket));
  CPPUNIT_ASSERT(mPacket->readCompleted());
  CPPUNIT_ASSERT(mPacket->valid());

  // Going to start with a bogus start byte and see if we recover
  s.reset();
  s.append(0x13); // not the right start byte
  s.append(0x12); // Nor this one
  s.append(0x13);
  s.append(0x02);
  s.append(0xf2);
  s.append(0xf3);
  s.append(0x06);

  mPacket->reset();
  CPPUNIT_ASSERT(! Packet::read(s, *mPacket));
  CPPUNIT_ASSERT(! mPacket->readCompleted());
  CPPUNIT_ASSERT(! mPacket->valid());

  mPacket->resetToStartByte();
  CPPUNIT_ASSERT(Packet::read(s, *mPacket));
  CPPUNIT_ASSERT(mPacket->readCompleted());
  CPPUNIT_ASSERT(mPacket->valid());
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestPacket );
CPPUNIT_TEST_SUITE_REGISTRATION( TestCommandPacket );

// -------------------------------------------------------------
// Serial Method definition
// -------------------------------------------------------------

Stream::Stream() {
  reset();
}

boolean Stream::available() {
  return mCurrentIndex - mCurrentReadIndex;
  // return mCurrentIndex > 0 && mCurrentReadIndex < mCurrentIndex;
}

void Stream::reset() {
  mCurrentIndex = 0;
  mCurrentReadIndex = 0;
}

void Stream::append(uint8_t b) {
  if (mCurrentIndex >= SERIAL_MAX_BUFFER) {
    return;
  }
  mBuffer[mCurrentIndex++] = b;
}

uint8_t Stream::read() {
  if (available()) {
    return mBuffer[mCurrentReadIndex++];
  } else {
    return 0;
  }
}

void Stream::write(uint8_t *b, int l) {
  // Do nothing
}


int main(int argc, char* argv[]) {
  // informs test-listener about testresults
  CPPUNIT_NS::TestResult testresult;

  // register listener for collecting the test-results
  CPPUNIT_NS::TestResultCollector collectedresults;
  testresult.addListener(&collectedresults);

  // register listener for per-test progress output
  CPPUNIT_NS::BriefTestProgressListener progress;
  testresult.addListener(&progress);

  // insert test-suite at test-runner by registry
  CPPUNIT_NS::TestRunner testrunner;
  testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest ());
  testrunner.run(testresult);

  // output results in compiler-format
  CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
  compileroutputter.write ();

  // Output XML for Jenkins CPPunit plugin
  //ofstream xmlFileOut("cppTestBasicMathResults.xml");
  //XmlOutputter xmlOut(&collectedresults, xmlFileOut);
  //xmlOut.write();

  // return 0 if tests were successful
  return collectedresults.wasSuccessful() ? 0 : 1;
}
