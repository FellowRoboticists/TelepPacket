/**
 * Stub Arduino.h
 */

typedef int boolean;

#include <math.h>
#define PI M_PI
#include <stdint.h>

#define SERIAL_MAX_BUFFER 64

class Stream {
  public:
    Stream();
    // -- Standard stream interface
    boolean available();
    uint8_t read();
    void write(uint8_t *b, int l);

    // -- Testing interface
    void reset();
    void append(uint8_t b);

  private:
    uint16_t mCurrentIndex;
    uint16_t mCurrentReadIndex;
    uint8_t mBuffer[SERIAL_MAX_BUFFER];
};
