OBJS=      \
pre.o      \
data_io.o  \
misc.o

all: pre

pre: $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS)

clean: 
	rm -f $(OBJS)

