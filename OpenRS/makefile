CPPSOURCES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(CPPSOURCES))
CXX = g++
CC = $(CXX)
DEBUG = -g -DCPP_DEBUG=1
CFLAGS = -Wall $(DEBUG)
CPPFLAGS = -I../../../boost/boost_1_59_0
LFLAGS = -Wall
LDLIBS =
EXECUTABLE = openrs.exe

all: $(EXECUTABLE)

$(EXECUTABLE): main
	mv main.exe $(EXECUTABLE)

main: $(OBJS)

.PHONY: all clean

clean:
	\rm *.o $(EXECUTABLE)
