#include <pthread.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <exception>
#include <stdexcept>
#include <memory>
#include <iostream>

#include "worker.h"

using std::exception;
using std::vector;
using std::shared_ptr;
using std::cin;

void* UseThread(void* arg)
{
  return (ThreadWorker*)arg->Handle(); 
}

void ThreadMaster::TakeRequests() override 
{
  std::string str;
  cin >> str;
  BaseRequest req(Requests); 
}
/*
            std::cout << "Available requests list:\n";
            std::cout << "START   : Initiates the game\n";
            std::cout << "RUN X   : Runs X iterations of stopped game\n";
            std::cout << "STATUS  : Shows field status\n";
            std::cout << "STOP    : Stops game calculating\n";
            std::cout << "QUIT    : Closes the program\n";
            */
