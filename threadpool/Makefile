appname := threadpool

CXX := clang++
CXXFLAGS := -std=c++17 -Wall

$(appname): main.cpp threadpool.cpp
	$(CXX) $(CXXFLAGS) main.cpp threadpool.cpp -o $(appname).out

clean:
	rm -f $(appname).out
