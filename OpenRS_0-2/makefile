# TODO:
# - Make this set DEBUG to include -DCPP_TEST=1 if `make test` is called

CPPSOURCES = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, %.o, $(CPPSOURCES))
CXX = g++
CC = $(CXX)
DEBUG = -g -DCPP_DEBUG=1 -DCPP_DEBUG_BOOST_SIG=1 \
-DCPP_DEBUG_OBS_BOOST=1
CPPFLAGS =	-Wall $(DEBUG) \
		-I../../../boost/boost_1_59_0
LFLAGS = -Wall
LDLIBS =
EXECUTABLE = openrs.exe

all: $(EXECUTABLE)

$(EXECUTABLE): main
	mv main.exe $(EXECUTABLE)

main: $(OBJS)

.PHONY: all clean test

clean:
	\rm *.o $(EXECUTABLE)