CXX=ccache g++ -g -std=c++11
CPPFLAGS=-Wall -Wextra -pthread -MD -MF $(patsubst %.o, .%.d, $(notdir $@)) -MT '$(patsubst %.o, .%.d, $(notdir @)) $@'
CXXFLAGS=-ICommon/src -IDoubleBuffer/src
LDFLAGS=-lrt
all: servo terminal

libcommon.a: $(patsubst %.cpp, %.o, $(wildcard Common/src/*.cpp))
	$(AR) $(ARFLAGS) $@ $^

libdb.a: $(patsubst %.cpp, %.o, $(wildcard DoubleBuffer/src/*.cpp))
	$(AR) $(ARFLAGS) $@ $^

servo: $(patsubst %.cpp, %.o, $(wildcard Servo/src/*.cpp)) libcommon.a libdb.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

terminal: $(patsubst %.cpp, %.o, $(wildcard Terminal/src/*.cpp)) libcommon.a libdb.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f */*/*.o

-include .*.d
