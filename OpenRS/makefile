CPPSOURCES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(CPPSOURCES))
CXX = g++
CC = $(CXX)
DEBUG = -g
CFLAGS = -Wall $(DEBUG)
CPPFLAGS = -I../../../boost/boost_1_59_0
LFLAGS = -Wall
LDLIBS =

all: openrs

openrs: main
	mv main.exe openrs.exe

main: $(OBJS)

.PHONY: all clean

clean:
	\rm *.o $(EXECUTABLE)
