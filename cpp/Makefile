CXX=mpicxx
CXXFLAGS=-std=c++11 -Wall -I$(GPERF_DIR)/include
LDFLAGS=-L$(GPERF_DIR)/lib -lprofiler
TARGET=parallel-opt

all: $(TARGET)

$(TARGET): parallel_computation.cpp
	$(CXX) $(CXXFLAGS) parallel_computation.cpp -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET) *.prof