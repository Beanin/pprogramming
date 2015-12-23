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
char* Cur = Stream;
char* StreamEnd = Stream;


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
    GenerateRandomField(); 
    InitWorkers();
  } 
  else 
    HandleOtherRequests();
  
}

void BaseMaster::GenerateRandomField()
{
  srand(time(0));
  OldField.resize(Height, vector<int>(Width + 2));
  Field.resize(Height, vector<int>(Width + 2));
  for (size_t i = 1; i < OldField.size() - 1; ++i)
    for (size_t j = 1; j < OldField[0].size() - 1; ++j)
      OldField[i][j] = rand()%100 < ALIVE_CELL_PROBABILITY ? 1 : 0;
  Field[0] = Field[Field.size() - 2];
  Field[Field.size() - 1] = Field[1];
} 

void LocalMaster::TakeRequests() 
{

  StreamEnd+=read(STDIN_FILENO, Stream, 20);
  if (!strncmp(Cur, "HELP", 4) 
    PrintHelpMessage();
  else if (!strncmp(Cur, "RUN", 3)) {
    Cur+=3;
    int ic; 
    Cur+=sscanf(Cur, "%d", &ic);
    Requests.push_back(BaseRequest("RUN",ic));
  }  
  else if (!strncmp(Cur, "STATUS", 6))
  {
    Cur += 6;
    Requests.push_back(BaseRequest("STATUS"));
  }
  else if (!strncmp(Cur, "QUIT", 4))
  {
    Cur += 4;
    Requests.push_back(BaseRequest("QUIT"));
  }
  else if (!strncmp(Cur, "STOP", 4))
  {
    Cur += 4;
    Requests.push_back(BaseRequest("STOP"));
  }
  else if (!strncmp(Cur, "START", 5))
  { 
    Cur+=5;
    if (!Height || !Width) { 
      Cur+=sscanf(Cur, "%u%u%u", &WorkersCount &Height, &Width);
      Slaves.resize(WorkersCount);
      FieldsToSend.resize(WorkersCount, vector<vector<int>> (Height + 2));
      Requests.push_back(BaseRequest("START"));
    }
  }
  else if (StreamEnd - Cur > 6) {
    Cur++;
  } 
}

void ThreadMaster::InitWorkers() {
  ThreadWorkerDataCommon threadCommon;
  threadCommon.Barrier = &WorkersBarrier;
  threadCommon.SleepLock = &WorkersSleepLock;
  threadCommon.WorkersSleepCV = &WorkersSleepCV;
  threadCommon.MasterSleepCV = &MasterSleepCV;
  for (size_t i = 0; i < WorkersCount; ++i)
  {
    size_t sz = ((Height - 1 / WorkersCount) + 1);
    size_t y0 = sz * i;
    FieldsToSend[i].resize(min(sz, Height + 2 - y0), vector<int>(Width));
    for (size_t y = 0; y < min(sz + 2, Height + 2 - y0); ++y)
    {
      FieldsToSend[i][y] = Field[i][y0 + y]; 
    }
    LocalWorkerData localData;
    localData.SrcField = &FieldsToSend[i];
    localData.SendFieldBottom = &FieldsToSend[(i + 1) % WorkersCount][0]; 
    localData.SendFieldTop = &FieldsToSend[(i + WorkersCount - 1) % WorkersCount].back();
    localData.ReceiveFieldBottom = &FieldsToSend[i].back();
    localData.ReceiveFieldTop = &FieldsToSend[i][0];
    localData.RequestsFromMaster = &RequestsToSend;
    Slaves.push_back(std::shared_ptr<LocalWorker>(new ThreadWorker(i, localData, threadCommon)));

  }
}

ThreadMaster::ThreadMaster(unsigned i)
{
  pthread_barrier_init(&WorkersBarrier, nullptr, WorkersCount);
  pthread_mutex_init(&WorkersSleepLock, nullptr);
  pthread_cond_init(&WorkersSleepCV, nullptr);
  pthread_cond_init(&MasterSleepCV, nullptr);
  pthread_mutex_init(&RequestLock)
}

ThreadMaster::~ThreadMaster() 
{
  set_fl(STDIN_FILENO, O_NONBLOCK); 
  pthread_barrier_destroy(&WorkersBarrier);
  pthread_mutex_destroy(&WorkersSleepLock);
  pthread_cond_destroy(&WorkersSleepCV);
  pthread_cond_destory(&MasterSleepCV);
  pthread_mutex_destroy(&RequestLock);
}