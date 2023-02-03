FC=gfortran
FFLAGS=-Wall -g -O3

all: eff_m_exp eff_m_hyper

eff_m_exp: eff_m_exp.o data_io.o
	$(CXX) -o $@ $(CXXFLAGS) eff_m_exp.o data_io.o
eff_m_hyper: eff_m_hyper.o data_io.o DRTBIS.o
	$(CXX) -o $@ $(CXXFLAGS) eff_m_hyper.o data_io.o DRTBIS.o -lgfortran
	
eff_m_exp.o: data_io.h
eff_m_hyper.o: data_io.h
data_io.o: data_io.h

clean: 
	rm -f *.o
