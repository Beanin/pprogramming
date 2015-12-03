#include <pthread.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <exception>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <fcntl.h>

#include "worker.h"

const unsigned ALIVE_CELL_PROBABILITY = 30; 

using std::exception;
using std::vector;
using std::shared_ptr;
using std::cin;

char Stream[100000];
const char* Cur = Stream;
char* StreamEnd = Stream;

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

void LocalMaster::TakeRequests() 
{
  StreamEnd+=read(STDIN_FILENO, Stream, 20);
  if (!strncmp(Cur, "HELP", 4) 
    PrintHelpMessage();
  else if (!strncmp(Cur, "RUN", 3)) {
    Cur+=3;
    int ic; 
    sscanf(Cur, "%d", ic);
    ++Cur;
    Requests.push_back(BaseRequest("RUN",ic));
  }  
  else if (!strncmp(Cur, "STATUS", 6))
  {
    Requests.push_back(BaseRequest("STATUS"));
  }
  else if (!strncmp(Cur, "QUIT", 4))
  {
    Requests.push_back(BaseRequest("QUIT"));
  }
  else if (!strncmp(Cur, "STOP", 4))
  {
    Requests.push_back(BaseRequest("STOP"));
  }
  else if (StreamEnd - Cur > 6) {
    Cur++;
  } 
}

void LocalMaster::GetStartRequest() 
{
  StreamEnd+=read(STDIN_FILENO, Stream, 20);
  if (!strncmp(Cur, "START", 5))
  {
    State = STARTED;
    Cur+=5;
  }
  if (!strncmp(Cur, "HELP", 4)) 
  {
    PrintHelpMessage();
    Cur+=4; 
  }
  else if (StreamEnd - Cur > 5) {
    Cur++;
  } 
}

