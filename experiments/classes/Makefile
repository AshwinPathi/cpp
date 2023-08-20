appname := classes

CXX := clang++
CXXFLAGS := -std=c++17 -Wall

sizes: class_sizes.cpp
	$(CXX) $(CXXFLAGS) class_sizes.cpp -o sizes.out

destructors: destructors.cpp
	$(CXX) $(CXXFLAGS) destructors.cpp -o destructors.out

clean:
	rm -f *.out
