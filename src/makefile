CXXFLAGS = -std=c++14 -fpermissive
all:		estruturas.o	view.o	my_drive.o
	g++ $(CXXFLAGS) estruturas.o view.o my_drive.o -o my_drive

estruturas.o:	estruturas.hpp view.hpp estruturas.cpp
	g++ -c $(CXXFLAGS) estruturas.cpp
view.o:		view.hpp view.cpp
	g++ -c $(CXXFLAGS) view.cpp