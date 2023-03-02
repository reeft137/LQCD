CXXFLAGS = -Wall -std=c++17

TARGET = time space KSpot

all: $(TARGET)

$(TARGET): %: %.o
	$(CXX) $(LDFLAGS) -o $@ $< -L./lib -llqcd $(LDLIBS)
%.o: %.cc
	$(CXX) $(CXXFLAGS) -I./lib -c -o $@ $<

clean: 
	$(RM) *.o 

clean.all:
	$(RM) *.o
	$(RM) $(TARGET)

