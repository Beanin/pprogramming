#include <iostream>
#include <ctime>
#include <string>
#include <exception>
#include <stdexcept>
#include <memory>

#include "worker.h"



void* BaseWorker::Handle() 
{
  while (State != ENDED) 
  {
    LockSleep();
    if (State != RUNNING && Requests.empty()) 
    {
      Sleep();
    }
    UnlockSleep();
    TakeRequest();
    HandleRequest();
    Report();
    CollabSync();
  }    
}

void BaseWorker::MasterSync()
{
  CollabSync();
  WakeUpMaster();
}

void BaseWorker::HandleRequest()
{
  if (Requests.front() == "RUN" && State == STOPPED)
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
  if (Requests.front() == "STOP" && State == RUNNING))
  {
    SendFinalReport();
    State = STOPPED;
    Requests.pop_front();
  }
  else if (Requests.front() == "QUIT" && State == STOPPED)
  {
    State = ENDED;
  }  
  else 
    Requests.pop_front();

}

void ThreadWorker::LockSleep() 
{
  pthread_mutex_lock(SleepLock);
}

void LocalWorker::Unlock()
{
  pthread_mutex_unlock(SleepLock);
}

size_t ThreadWorker::NeighboursCount(size_t x, size_t y) 
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

void ThreadWorker::TakeRequests()
{
  pthread_mutex_lock(RequestLock);
  if (!UpdatingQueue)
    UpdatingQueue = true;
  for (; RequestQueuePosition < RequestsFromMaster.size(); ++RequestQueuePosition)
  {
    Requests.push_back((*RequestsFromMaster)[RequestQueuePosition]);
  } 
  Updated++;
  if (Updated == WorkersCount)
  {
    UpdatingQueue = false;
    Updated = 0;
  }
  pthread_mutex_unlock(RequestLock);
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

void ThreadWorker::CollabSync()
{
  pthread_barrier_wait(Barrier);
}

void ThreadWorker::WakeUpMaster()
{
  if (Id == 0) 
  { 
    pthread_cond_signal(MasterSleepCV);
  }
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
} 

void ThreadWorker::Sleep()
{
  pthread_cond_wait(WorkersSleepCV, WorkersSleepLock);
}

size_t LocalWorker::NeighboursCount(size_t x, size_t y) 
{
  size_t Cnt = 0;
  for (int i = -1; i < 2; ++i) 
  {
    for (int j = -1; j < 2; ++j) 
    {
      if (!(i==0 && j==0))
        Cnt+= OldField[i + y][j + x];
    }
  }
  return Cnt;
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