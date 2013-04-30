CC=g++
CXX=g++
RM=rm -f
CPPFLAGS=
LDFLAGS=
LDLIBS=

SRCS=main.cpp calc.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: calc

calc: $(OBJS)
	    g++ $(LDFLAGS) -o calc $(OBJS) $(LDLIBS) 

depend: .depend

.depend: $(SRCS)
	    rm -f ./.depend
	        $(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
	    $(RM) $(OBJS)

dist-clean: clean
	    $(RM) *~ .depend calc

include .depend
