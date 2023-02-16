BRIDGE_INSTALL_PATH=../../../bridge161/build/
include $(BRIDGE_INSTALL_PATH)/make.inc
LIB=..
CXXFLAGS += -I$(LIB)


OBJS=\
rcnp.o             \
comm4D_to_comm1D.o \
comm4D_to_comm3D.o \
mpiio.o            \
CommsInit.o        \
                   \
rcnp_gamma.o       \
rcnp_SOLVERS.o     \
misc.o			   \
shift.o


all: $(LIB)/librcnp.a

$(LIB)/librcnp.a: $(OBJS)
	ar rcs $(LIB)/librcnp.a $?

clean: Force
	rm -f $(OBJS)

clean.all: Force
	rm -f $(OBJS)
	rm -f $(LIB)/librcnp.a

Force:

