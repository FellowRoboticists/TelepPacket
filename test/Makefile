CXX = g++
INCLUDES = -I . -I ../lib/TelepPacket
CXXFLAGS = -g $(INCLUDES) -std=gnu++11
OBJM = Packet.o CommandPacket.o
LINKFLAGS = -lcppunit

testpacket: TestPacket.cpp $(OBJM)
	$(CXX) $(CXXFLAGS) -o $@ TestPacket.cpp $(OBJM) $(LINKFLAGS) $(LINKFLAGSLOG4) $(LIBLOG)

test: testpacket
	./testpacket


Packet.o: ../lib/TelepPacket/Packet.cpp ../lib/TelepPacket/Packet.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

CommandPacket.o: ../lib/TelepPacket/CommandPacket.cpp ../lib/TelepPacket/CommandPacket.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Default Compile
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
