OBJS = time.o
CXXFLAGS = -Wall -std=c++11
CFLAGS = -Wall -std=gnu99

%.o: %.cc
	$(CXX) $(CXXFLAGS) -I./lib -c -o $@ $<

all: time 

time: $(OBJS) lib/libtc.a
	$(LD) $(LDFLAGS) -o $@ $(OBJS) -L./lib -ltc $(LDLIBS)
# space: $(OBJS) space.o
# 	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) space.o
# v_0: v_0.o
# 	$(CC) -o $@ $(CFLAGS) v_0.o
# v_s: v_s.o
# 	$(CC) -o $@ $(CFLAGS) v_s.o

clean:
	$(RM) $(OBJS) time.o space.o v_0.o v_s.o

clean.all:
	$(RM) $(OBJS) time.o space.o v_0.o v_s.o
	$(RM) time space v_0 v_s

