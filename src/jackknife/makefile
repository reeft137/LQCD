all: jackknife_sampling jackknife_average

jackknife_sampling: jackknife_sampling.o valarray_io.o
	$(CXX) -o $@ $(CXXFLAGS) jackknife_sampling.o valarray_io.o
jackknife_average: jackknife_average.o valarray_io.o
	$(CXX) -o $@ $(CXXFLAGS) jackknife_average.o valarray_io.o


jackknife_sampling.o: valarray_io.h
jackknife_average.o: valarray_io.h
valarray_io.o: valarray_io.h

clean: 
	rm -f *.o
