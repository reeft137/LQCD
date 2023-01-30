FC=gfortran
FFLAGS=-Wall -g -O3

all: eff_m_exp eff_m_hyper

eff_m_exp: eff_m_exp.o valarray_io.o
	$(CXX) -o $@ $(CXXFLAGS) eff_m_exp.o valarray_io.o
eff_m_hyper: eff_m_hyper.o valarray_io.o DRTBIS.o
	$(CXX) -o $@ $(CXXFLAGS) eff_m_hyper.o valarray_io.o DRTBIS.o -lgfortran
	
eff_m_exp.o: valarray_io.h
eff_m_hyper.o: valarray_io.h
valarray_io.o: valarray_io.h

clean: 
	rm -f *.o
