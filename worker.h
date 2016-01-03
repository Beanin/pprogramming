#pragma once
#include <vector>
#include <pthread.h>
#include <memory>
#include <deque>

#include "request.h"

typedef enum {WAITING, RUNNING, STOPPED, ENDED} TState;

using std::vector;
using std::shared_ptr;
using std::deque;

class BaseWorker {
public:
  virtual void* Handle();
  void ClearRequests() {Requests.clear();}
  virtual void HandleRequest();  
  virtual void TakeRequests() = 0;
  virtual void SendCalculations() = 0;
  virtual void ReceiveCalculations() = 0;
  virtual void SendFinalReport() = 0;
  virtual void Calculate() = 0;
  virtual void CollabSync() = 0; 
  virtual void SyncWithMaster() = 0;
 
  virtual void Report() {SyncWithMaster();}
  virtual ~BaseWorker() = default;

  vector<vector<int>> Field;
  vector<vector<int>> OldField;
  size_t IterNumber;  
  size_t IterCount;
  unsigned Height, Width;
  unsigned Id;
  TState State;
  deque<BaseRequest> Requests;
};

struct ThreadWorkerDataCommon  
{
  pthread_barrier_t* WorkersBarrier;
  pthread_barrier_t* MasterBarrier;
};

struct LocalWorkerData
{
  const vector<BaseRequest>* RequestsFromMaster;
  vector<vector<int>>* SrcField;
  vector<int>* SendFieldTop;
  vector<int>* SendFieldBottom;
  vector<int>* ReceiveFieldTop;
  vector<int>* ReceiveFieldBottom;
  unsigned RequestQueuePosition;
};

class LocalWorker : public BaseWorker, public LocalWorkerData
{
public:
  LocalWorker(unsigned number, LocalWorkerData localData);
  virtual void Calculate() override;
  virtual void SendCalculations() override;
  virtual void ReceiveCalculations() override;
  virtual void TakeRequests() override;
  virtual void SendFinalReport() override;


  size_t NeighboursCount(size_t x, size_t y);
};

class ThreadWorker : public LocalWorker, public ThreadWorkerDataCommon
{
public:
  ThreadWorker(unsigned number, LocalWorkerData localData, ThreadWorkerDataCommon threadCommon);
  virtual ~ThreadWorker() {
  }

  virtual void CollabSync() override;
  virtual void SyncWithMaster() override;


  static bool UpdatingQueue;
  static unsigned int Updated; 
  size_t RequestQueuePosition;
  pthread_t pid;
};

