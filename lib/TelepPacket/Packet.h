#define MAX_PACKET_SIZE 100
#define PKT_LENGTH_BYTE 1
#define PKT_START_BYTE 0x013

class Packet {
  public:

    /**
     * Constructs a new packet with the start byte set and the
     * current index set to 1.
     */
    Packet();

    /**
     * Clears the current buffer adds the start byte and resets 
     * the current index to 1.
     */
    void reset();

    /**
     * Attempts to find a start byte within the current invalid
     * buffer and shifts the buffer to the left. If the buffer
     * doesn't contain a start byte, the buffer will simply be
     * cleared.
     */
    void resetToStartByte();

    /**
     * Appends the specified byte to the buffer, incrementing the
     * current index.
     */
    void append(uint8_t b);

    /**
     * Returns the current length of the packet.
     */
    uint16_t length();

    /**
     * Calculates the checksum for the packet and appends it to the 
     * end of the buffer, incrementing the current index.
     */
    void complete();

    /**
     * Verify the validity of the specified packet. It will check that
     * it starts with the correct byte value, has the correct length
     * byte and the checksum matches.
     */
    boolean valid();

    /**
     * Calculates and returns the checksum for the current buffer
     * contents;
     */
    uint8_t calculateChecksum();

    /**
     * Returns true if the packet buffer is semantically complete. That is
     * the length of the buffer matches the length byte specified.
     */
    boolean readCompleted();

    /**
     * Reads the next full package from the specified stream. It will 
     * continue reading until a complete packet is read, or it will 
     * continue to read a packet if interrupted from a previous
     * read attempt.
     *
     * Returns true if a complete and valid packet was read, false
     * otherwise.
     */
    static boolean read(Stream &s, Packet& packet);
  private:

    uint16_t mCurrentIndex;
    uint8_t mBuffer[MAX_PACKET_SIZE];
};
