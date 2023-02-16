%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I./lib -c -o $@ $<

BRIDGE_INSTALL_PATH = ../bridge161/build/
include $(BRIDGE_INSTALL_PATH)/make.inc


OBJS=         \
SOLVER_MAIN.o \
SOLVER_SUB.o  \
parameters.o  \
anti_quark.o  \
qqbar.o   



all: SOLVER_MAIN



SOLVER_MAIN: $(OBJS) lib/librcnp.a
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -L./lib -lrcnp $(LDLIBS)


# archive
src/librcnp.a: Force
	cd lib; make all



parameters.o: parameters.h


cleanjob: 
	rm -f *.job.*

clean: Force
	rm -f $(OBJS)
	rm -f debug_*
	rm -f *~
	cd lib; make clean
	rm -f job.solver.*
	rm -f core.*
	rm -f LOG/*

doxygen: Force
	cd ../..; make doxygen




Force:
