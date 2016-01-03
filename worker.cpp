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
  if (!Requests.empty() && Requests.front() == "RUN" && State == STOPPED)
  {
    IterNumber = 0;
    IterCount = Requests.front().GetIterCount();
    State = RUNNING;
    Requests.pop_front();
  }
  if (Requests.empty() && State == RUNNING)
  {
    Calculate();
    CollabSync();
    SendCalculations();
    ReceiveCalculations();
    IterNumber++;
    if (IterNumber == IterCount)
    {
      Requests.push_front(BaseRequest("STOP"));
    }
  }

  if (Requests.empty())
    return;

  if (Requests.front() == "STOP" && State == RUNNING)
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
    fprintf(stderr, "Request handling error! Request: %s\n", Requests.front().GetType().c_str());
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
      if (!(i==0 && j==0))
        Cnt+= OldField[(i + OldField.size() + y) % OldField.size()][(j + OldField[0].size() + x) % OldField[0].size()];
    }
  }
  return Cnt;
}

void LocalWorker::SendCalculations()
{
  for (size_t i = 1; i < OldField[0].size() - 1; ++i) 
  {
    (*SendFieldTop)[i - 1] = OldField[0][i]; 
    (*SendFieldBottom)[i - 1] = OldField[OldField.size() - 1][i];  
  }
}

void LocalWorker::ReceiveCalculations() 
{
  for (size_t i = 1; i < OldField[0].size() - 1; ++i) 
  {
    OldField[0][i] = (*ReceiveFieldTop)[i];
    OldField[OldField.size() - 1][i] = (*ReceiveFieldBottom)[i];
  }
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
    for (size_t x = 1; x < OldField[0].size() - 1; ++x)
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
  Field = *SrcField;
  OldField = Field;
  Height = Field.size() - 2;
  Width = Field[0].size();
  State = WAITING;
}

void LocalWorker::SendFinalReport() 
{
  for (size_t i = 0; i < SrcField->size() - 1; ++i) 
  {
    for (size_t j = 0; j < (*SrcField)[0].size();++j) 
    {
      (*SrcField)[i][j] = OldField[i + 1][j + 1];
    }
  } 
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