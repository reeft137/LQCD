LIB=.

OBJS = jknife.o dataio.o misc.o
CXXFLAGS = -Wall -std=c++11 -I$(LIB)

all: $(LIB)/libtc.a

$(LIB)/libtc.a: $(OBJS)
	ar rcs $(LIB)/libtc.a $?

clean:
	$(RM) $(OBJS)

clean.all:
	$(RM) $(OBJS)
	$(RM) $(LIB)/libtc.a
