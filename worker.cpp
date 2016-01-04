#include <cstdio>
#include <ctime>
#include <string>
#include <exception>
#include <stdexcept>
#include <memory>

#include "worker.h"

static void* UseThread(void* arg)
{
  return ((ThreadWorker*)arg)->Handle(); 
}

void* BaseWorker::Handle() 
{
  while (State != ENDED) 
  {
    TakeRequests();
    HandleRequest();
    Report();
  }    
}

void BaseWorker::HandleRequest()
{
  if (State == RUNNING && Requests.empty())
  {
    Calculate();
    CollabSync();
    SendCalculations();
    CollabSync();
    ReceiveCalculations();
    IterNumber++;
    if (IterNumber == IterCount)
    {
      Requests.push_front(BaseRequest("STOP"));
    }
  }

  if (Requests.empty())
    return;

  if (Requests.front() == "RUN" && State == STOPPED)
  {
    IterNumber = 0;
    IterCount = Requests.front().GetIterCount();
    State = RUNNING;
    Requests.pop_front();
  }
  else if (Requests.front() == "STOP" && State == RUNNING)
  {
    SendFinalReport();
    State = STOPPED;
    Requests.pop_front();
    ClearRequests();
  }
  else if (Requests.front() == "QUIT" && State == STOPPED)
  {
    State = ENDED;
  }  
  else {
    fprintf(stderr, "Request handling error! Request: %s\nId:%u\n", Requests.front().GetType().c_str(), Id);
    Requests.pop_front();
  }
}

size_t LocalWorker::NeighboursCount(size_t x, size_t y) 
{
  size_t Cnt = 0;
  for (int i = -1; i < 2; ++i) 
  {
    for (int j = -1; j < 2; ++j) 
    {
      if (i*i + j*j > 0)
        Cnt+= OldField[(OldField.size() + y + i) % OldField.size()][(OldField[0].size()+ j+ x) % OldField[0].size()];
    }
  }
  return Cnt;
}

void LocalWorker::SendCalculations()
{
  (*SendFieldTop) = OldField[1];
  (*SendFieldBottom) = OldField[OldField.size() - 2]; 
}

void LocalWorker::ReceiveCalculations() 
{

  OldField[0] = (*ReceiveFieldTop);
  OldField.back() = (*ReceiveFieldBottom);
}


void LocalWorker::TakeRequests()
{
  SyncWithMaster();
  SyncWithMaster();
  for (; RequestQueuePosition < RequestsFromMaster->size(); ++RequestQueuePosition)
  {
    Requests.push_back((*RequestsFromMaster)[RequestQueuePosition]);
  }
}


void LocalWorker::Calculate()
{
  for (size_t y = 1; y < OldField.size() - 1; ++y)
  {
    for (size_t x = 0; x < OldField[0].size(); ++x)
    {
      if (OldField[y][x] && (NeighboursCount(x, y) == 3 || NeighboursCount(x, y) == 4))
        Field[y][x] = 1;
      else if (OldField[y][x]==0 && NeighboursCount(x, y) == 3)
        Field[y][x] = 1;
      else Field[y][x] = 0;   
    }
  }
  Field.swap(OldField);
}

LocalWorker::LocalWorker(unsigned number, LocalWorkerData localData):LocalWorkerData(localData)
{

  BaseWorker::Id = number;
  OldField = *SrcField;
  Field = OldField;
  Height = OldField.size() - 2;
  Width = OldField[0].size();
  State = STOPPED;
}

void LocalWorker::SendFinalReport() 
{
  (*SrcField) = OldField; 
  SyncWithMaster(); 
} 

void ThreadWorker::CollabSync()
{
  pthread_barrier_wait(WorkersBarrier);
}

void ThreadWorker::SyncWithMaster()
{
  pthread_barrier_wait(MasterBarrier);
}

ThreadWorker::ThreadWorker(unsigned number, LocalWorkerData localData, ThreadWorkerDataCommon threadCommon):LocalWorker(number, localData),
  ThreadWorkerDataCommon(threadCommon)
{
  pthread_create(&pid, nullptr, UseThread, this);
}  