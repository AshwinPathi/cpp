appname := function

CXX := clang++
CXXFLAGS := -std=c++2a -Wall

$(appname): main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o $(appname).out

clean:
	rm -f $(appname).out
