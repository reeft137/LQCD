all: jackknife_sampling jackknife_average

jackknife_sampling: jackknife_sampling.o data_io.o
	$(CXX) -o $@ $(CXXFLAGS) jackknife_sampling.o data_io.o
jackknife_average: jackknife_average.o data_io.o
	$(CXX) -o $@ $(CXXFLAGS) jackknife_average.o data_io.o


jackknife_sampling.o: data_io.h
jackknife_average.o: data_io.h
data_io.o: data_io.h

clean: 
	rm -f *.o
