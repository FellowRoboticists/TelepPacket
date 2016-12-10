#define MAX_PACKET_SIZE 100

class CommandPacket {
  public:

    /**
     * Constructs a new CommandPacket for use with the command
     * byte. The commandLength is the total number of bytes when
     * you include the command byte and all the bytes of the
     * value(s).
     */
    CommandPacket(uint8_t commandByte, uint8_t commandLength);

    /**
     * Clears out anything in the buffer and resets the counter
     * to 0.
     */
    void reset();

    /**
     * Appends a byte to the buffer.
     */
    void append(uint8_t b);

    /**
     * Returns the length of the buffer.
     */
    uint8_t length();

    /**
     * Returns true if the packet is valid.
     */
    boolean valid();

    /**
     * Returns the next value in the buffer of the specified length.
     */
    uint16_t nextValue(uint8_t valueLength);

    boolean readCompleted();

    /**
     * Reads the next full command packet from the specified stream.
     */
    static boolean read(Stream &s, CommandPacket &packet);

  private:

    // The byte index as bytes are appended
    uint8_t mCurrentIndex;
    uint8_t mCommandLength;
    // The byte index as values are pulled out
    uint8_t mValueIndex;
    // The buffer to hold the command packet
    uint8_t mBuffer[MAX_PACKET_SIZE];
    uint8_t mCommandByte;
};
