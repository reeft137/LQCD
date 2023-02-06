OBJS=       \
ccbarmain.o \
jackknife.o \
nbsmisc.o   \
data_io.o   \
misc.o

all: ccbarmain

ccbarmain: $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS)

clean: 
	rm -f $(OBJS)

