
SOURCES = $(shell ls *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

CXX = g++
CXXFLAGS = -Wall -Werror -Winline -ansi -pedantic -fPIC -O3

AR = ar
ARFLAGS = rc

ARLIB = libsmpp.a
SHLIB = libsmpp.so

.PHONY: all
all: $(ARLIB) $(SHLIB)

$(ARLIB): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

$(SHLIB): $(OBJECTS)
	$(CXX) -shared -o $@ $(CCCOPTS) $(OBJECTS) -fPIC

.PHONY: clean
clean:
	rm -f core *.o *.d *~ $(ARLIB) $(SHLIB)

