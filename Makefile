main : GameLife.cpp worker.cpp worker.h request.h master.cpp master.h
	g++ -std=c++11 -pthread master.cpp worker.cpp 
