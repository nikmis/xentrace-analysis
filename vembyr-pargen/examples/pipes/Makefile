CC=g++ 
CXX=g++
RM=rm -f
CPPFLAGS=
LDFLAGS=
LDLIBS=

SRCS=main.cpp pipes.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: pipes

pipes: $(OBJS)
	    g++ $(LDFLAGS) -o pipes $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	    rm -f ./.depend
	        $(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	    $(RM) $(OBJS)

dist-clean: clean
	    $(RM) *~ .depend pipes

include .depend
