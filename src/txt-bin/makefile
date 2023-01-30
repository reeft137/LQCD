all: bin2txt txt2bin

bin2txt: bin2txt.o
	$(CXX) -o $@ $(CXXFLAGS) bin2txt.o
txt2bin: txt2bin.o 
	$(CXX) -o $@ $(CXXFLAGS) txt2bin.o

bin2txt.o:
txt2bin.o:

clean: 
	rm -f *.o
