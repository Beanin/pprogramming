main : main.cpp worker.cpp worker.h request.h master.cpp master.h
	g++ -o ThreadLife.exe -std=c++11 -pthread master.cpp worker.cpp main.cpp 
