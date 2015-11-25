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

const unsigned ALIVE_CELL_PROBABILITY = 30; 

using std::exception;
using std::vector;
using std::shared_ptr;
using std::cin;

void* UseThread(void* arg)
{
  return (ThreadWorker*)arg->Handle(); 
}



void BaseMaster::HandleRequest()
{
  if (Requests.front() == "RUN" && State == STOPPED)
  {
    IterNumber = 0;
    IterCount = Requests.front().GetIterCount();
    State = RUNNING;
    SendRequest(Requests.front());
    WakeUpSlaves();
    Requests.pop_front();
  }
  if (Requests.empty() && State == RUNNING)
  {
    UpdateRequest();
    CollabSync();
    SendCalculations();
    ReceiveCalculations();
    IterNumber++;
    if (IterNumber == IterCount)
    {
      Requests.push_front(BaseRequest("STOP"));
    }
  }
  if (Requests.front() == "STOP" && State == RUNNING))
  { 
    SendRequest(Requests.front());
    Sleep();
    SendFinalReport();
    State = STOPPED;
  }
  else if (Requests.front() == "QUIT" && State == STOPPED)
  {
    State = ENDED;
  } 
  else if (State == WAITING && Requests.front() == "START")
  {
    GetStartRequest();
    if (!Field.size())
      GenerateRandomField(); 
    for (int i = 0; i < WorkersCount; ++i)
    {
      InitWorker(i);
    }
  } 
  else 
    HandleOtherRequests();
  
}

void BaseMaster::GenerateRandomField()
{
  srand(time(0));
  OldField.resize(Height, vector<int>(Width));
  Field.resize(Height, vector<int>(Width));
  for (size_t i = 1; i < OldField.size() - 1; ++i)
    for (size_t j = 1; j < OldField[0].size() - 1; ++j)
      OldField[i][j] = rand()%100 < ALIVE_CELL_PROBABILITY ? 1 : 0;
} 

void LocalMaster::GetStartRequest() 
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
