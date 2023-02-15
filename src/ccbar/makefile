LIBOFILE=   \
jackknife.o \
data_io.o   \
misc.o

all: corr4+lap

corr4+lap: $(LIBOFILE) corr4+lap.o
	$(CXX) -o $@ $(CXXFLAGS) $(LIBOFILE) corr4+lap.o
corr2+effmass: $(LIBOFILE) corr2+effmass.o
	$(CXX) -o $@ $(CXXFLAGS) $(LIBOFILE) corr2+effmass.o

clean:
	rm -f $(LIBOFILE) corr4+lap.o corr2+effmass.o

