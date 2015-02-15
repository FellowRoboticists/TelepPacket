TelepPacket Arduino Library
===========================

This library provides a Packet class to facilitate reading and
writing specially formatted buffers from the Serial interface.
The basic structure of the packet is as follows:

    Byte
       1      2     3     4        n         n+1
    | 0x13 | len | grp | cmd | cmd-values | chksum |

Where:

* Byte 1 - 0x13 - The marker for the start of a packet
* len - The number of bytes used by the grp, cmd and cmd-value
* grp - The identifier for the command group.
* cmd - The identifier of the specific commands
* cmd-values - 0-n bytes representing the data for the specified command. The values
    supported for a command are documented with the command.
* chksum - a 1-byte check sum for the packet. See below for the algorithm used for the checksum.

Command Values
--------------

A command may require one or more values for its implementation. In addition, the values of
the command may consist of one- or two-byte values. If a command requires a 16-bit integer
value, the value will be encoded in the packet in big-endian format. For example, if the
packet has a 16-bit integer starting at position 4 in the packet, you would re-create the
integer values as follows:

    uint16_t value = packet[4] << 8 | packet[5]

To encode a 16-bit value in the packet you would do something like:

    uint16_t value = 823
    packet[4] = value >> 8
    packet[5] = value & 0x0f

How you do this may depend on the language you're using to encode/decode your packets.
Typeless languages tend to make this more fussy out of the box. For example, some 
languages don't have an inherent unsigned type of the correct size, so you have to perform
masking on the values prior to shifting.

Checksum Calculation
--------------------

The checksum used for this packet is a simple and - admittedly not very robust - method. It 
is deemed to be sufficient for our purposes to allow a pretty good confidence that we have
received/sent a valid packet.

It is calculated as follows (assuming an overall packet length of 5 and that the index of
the first byte is 0):

    byte check_sum = 0
    for (int i=0; i<4; i++) check_sum += packet[i]
    check_sum == packet[4]

Note that the loop above does NOT include the checksum byte. Also note that for other language
implementations you may have to make this slightly more complicated depending on whether
the language provides an unsigned byte type (or any types at all for that matter).

Copyright
=========

Copyright (c) 2015 Dave Sieh

See LICENSE.txt for details.

