CXXFLAGS = -std=c++17 -g3 -O3 -Wall

memory-balloon: memory-balloon.o
	$(CXX) $(LDFLAGS) -o $@ $<

memory-balloon.o: ArgParser.h
