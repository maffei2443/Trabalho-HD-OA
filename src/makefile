CXXFLAGS = -std=c++11
MY_DRIVE.o:		estruturas.o	view.o	my_drive.o
	g++ $(CXXFLAGS) estruturas.o view.o my_drive.o -o my_drive

estruturas.o:	estruturas.hpp estruturas.cpp
	g++ -c $(CXXFLAGS) estruturas.cpp
view.o:		view.hpp view.cpp
	g++ -c $(CXXFLAGS) view.cpp