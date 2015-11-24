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
    if (State != RUNNING && Requests.empty()) 
    {
      Sleep();
    }
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
    HandleOtherRequests();
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



/*void TThreadWorker::HandleRequest() 
{ 
  if (State == RUNNING) 
  {
    IterNumber++;
    for (size_t y = 1; y < Field.size() - 1; ++y) 
    {
      for (size_t x = 0; x < Field[0].size(); ++x) 
      {
        int c = NeighboursCount(x, y);
        if (OldField[y][x] == 1) {
          if (c < 2 || c > 3)
            Field[y][x] = 0;

        }
        else if (c == 3)
          Field[y][x] = 1;
      }
    }
    OldField.swap(Field);
    int waitRes = pthread_barrier_wait(Barrier);
    if (waitRes != PTHREAD_BARRIER_SERIAL_THREAD && waitRes != 0) 
    {
      perror("pthread_barrier_wait");
    }

  }  
}*/

void TThreadWorker::SendCalculations() {
  if (State == RUNNING) 
  {
    for (size_t i = 1; i < OldField[0].size() - 1; ++i) 
    {
      (*SendT)[i - 1] = OldField[0][i]; 
      (*SendB)[i - 1] = OldField[Field.size() - 1][i];  
    }
  }
}

void TThreadWorker::ReceiveCalculations() 
{
  if (State == RUNNING) 
  {
    for (size_t i = 1; i < Field[0].size() - 1; ++i) 
    {
      Field[0][i] = (*recT)[i];
      Field[Field.size() - 1][i] = (*recB)[i];
    }
  }
}

void TThreadWorker::ReceiveRequest() 
{
  pthread_mutex_lock(ReqLock);  
  *Updating = true;
  if (id == 0)
    pthread_cond_signal(ReqCV);
  pthread_mutex_unlock(ReqLock);
  if (Request == "STOP") 
  {
    State = STOPPED;
  } 
  else if (Request == "END") 
  {
    if (State == STOPPED)
      State = ENDED;
  }  
  else if (Request == "RUN") 
  {
    IterCount = req->IterCount();
    if (Reported)
      Reported = false;
  }
  int waitRes = pthread_barrier_wait(Barrier);
  if (waitRes != PTHREAD_BARRIER_SERIAL_THREAD && waitRes != 0) 
  {
     perror("pthread_barrier_wait");
  }
  else if (waitRes == PTHREAD_BARRIER_SERIAL_THREAD) 
  {
    pthread_mutex_lock(ReqLock);
    pthread_cond_signal(ReqCV, ReqLock);
    *Updating = false;
    pthread_mutex_unlock(ReqLock);
  }     
}





void ThreadWorker::StopWait() 
{
  int waitRes = pthread_barrier_wait(Barrier);
  if (waitRes != PTHREAD_BARRIER_SERIAL_THREAD && waitRes != 0) 
  {
    perror("pthread_barrier_wait");
  }
  if (!Id) 
    pthread_cond_signal(StopCV); 
}

void ThreadWorker::SendCommonReport() 
{
  for (size_t i = 1; i < OldField[0].size() - 1; ++i) 
  {
    (*SendField)[0][i-1] = OldField[i]; 
    (*SendField)[SendField->size() - 1][i-1] = OldField[OldField.size() - 1][i];
  } 
}

void ThreadWorker::SendFinalReport() 
{
  for (size_t i = 0; i < SendField->size() - 1; ++i) 
  {
    for (size_t j = 0; j < (*SendField)[0].size();++j) 
    {
      (*SendField)[i][j] = OldField[i + 1][j + 1];
    }
  }  
} 

size_t ThreadWorker::NeighboursCount(size_t x, size_t y) 
{
  size_t Cnt = 0;
  for (int i = -1; i < 2; ++i) 
  {
    for (int j = -1; j < 2; ++j) 
    {
      if (!(i==0 && j==0))
        Cnt+= OldField[i+y][j + x];
    }
  }
  return Cnt;
} 


