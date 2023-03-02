LIB=..
CXXFLAGS = -Wall -std=c++11 -I$(LIB)

OBJS = \
jknife.o \
dataio.o \
misc.o

all: $(LIB)/liblqcd.a

$(LIB)/liblqcd.a: $(OBJS) typealias.h
	ar rv $(LIB)/liblqcd.a $?

clean:
	$(RM) $(OBJS)

clean.all:
	$(RM) $(OBJS)
	$(RM) $(LIB)/liblqcd.a
