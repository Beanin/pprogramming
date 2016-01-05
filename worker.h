#pragma once
#include <vector>
#include <memory>
#include <deque>

#include "request.h"

typedef enum {WAITING, RUNNING, STOPPED, ENDED} TState;

using std::vector;
using std::shared_ptr;
using std::deque;

class BaseWorker {
public:
  void* Handle();
  void ClearRequests() {Requests.clear();}
  virtual void HandleRequest();  
  virtual void HandleOtherRequests();
  virtual void TakeRequests() = 0;
  virtual void SendCalculations() = 0;
  virtual void ReceiveCalculations() = 0;
  virtual void SendFinalReport() = 0;
  virtual void Calculate() = 0;
  virtual void CollabSync() = 0; 
  virtual void SyncWithMaster() = 0;
  virtual void Report() {}
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

class LocalWorker : public virtual BaseWorker, public LocalWorkerData
{
public:
  LocalWorker()=default;
  LocalWorker(unsigned number, LocalWorkerData localData);
  virtual void ReceiveCalculations() override;
  virtual void SendCalculations() override;
  virtual void TakeRequests() override;
  virtual void SendFinalReport() override;
  virtual void Calculate() override;

  size_t NeighboursCount(size_t x, size_t y);
};

