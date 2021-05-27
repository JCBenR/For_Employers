INCS = ConcurrentQueue.h ConcurrentQueueTests.h SerialQueue.hpp SerialQueueTests.h

OBJS = main.o ConcurrentQueueTests.o SerialQueueTests.o

CXXFLAGS = --std=c++14 -O2

threadQueue: $(OBJS)
	$(CXX) $(CXXFLAGS) -o threadQueue $(OBJS)



main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

ConcurrentQueueTests.o: ConcurrentQueueTests.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

SerialQueueTests.o: SerialQueueTests.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

