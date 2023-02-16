OBJS = jackknife.o data_io.o misc.o
CXXFLAGS = -Wall
CFLAGS = -Wall -std=gnu99

all: corr-space v_0 v_s

corr-time: $(OBJS) corr-time.o
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) corr-time.o
corr-space: $(OBJS) corr-space.o
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) corr-space.o
v_0: v_0.o
	$(CC) -o $@ $(CFLAGS) v_0.o
v_s: v_s.o
	$(CC) -o $@ $(CFLAGS) v_s.o

clean:
	$(RM) $(OBJS) corr-time.o corr-space.o v_0.o v_s.o

allclean:
	$(RM) $(OBJS) corr-time.o corr-space.o v_0.o v_s.o
	$(RM) corr-time corr-space v_0 v_s

