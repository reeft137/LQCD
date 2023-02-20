BRIDGE_INSTALL_PATH=../../../bridge161/build/
include $(BRIDGE_INSTALL_PATH)/make.inc
LIB=..
CXXFLAGS += -I$(LIB)


OBJS=\
fft3d.o

all: $(LIB)/librcnp.a

$(LIB)/librcnp.a: $(OBJS)
	ar rcs $(LIB)/librcnp.a $?

clean: Force
	rm -f $(OBJS)

clean.all: Force
	rm -f $(OBJS)
	rm -f $(LIB)/librcnp.a

Force:

