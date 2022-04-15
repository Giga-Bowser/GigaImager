# do you actually read makefiles? lol. i dont even know how this stuff should look
CXX = g++
CXXFLAGS = -std=c++17
CXXFLAGS = -O3

all: main.cpp xxhash/xxhash.c
	$(CXX) $(CXXFLAGS) $? -o build/gigaImager