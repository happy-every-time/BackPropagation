CXX := g++
CXXFLAGS := -Wall -Os

%.o: %.cpp
    $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@