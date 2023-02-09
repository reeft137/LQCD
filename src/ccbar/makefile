LIBOFILE=   \
jackknife.o \
data_io.o   \
misc.o

all: space_corr

space_corr: $(LIBOFILE) space_corr.o
	$(CXX) -o $@ $(CXXFLAGS) $(LIBOFILE) space_corr.o
time_corr: $(LIBOFILE) time_corr.o
	$(CXX) -o $@ $(CXXFLAGS) $(LIBOFILE) time_corr.o

clean:
	rm -f $(LIBOFILE)

