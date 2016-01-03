#pragma once
#include "request.h"
#include "worker.h"
#include <memory>

class BaseMaster: public BaseWorker
{
public:
  virtual void SendRequest(BaseRequest req) = 0;
  virtual void SendFinalReport() = 0;
  virtual void WorkersSync() = 0;
  
  virtual void SendCalculations() override {}
  virtual void ReceiveCalculations() override {}
  virtual void CollabSync() override {}
  virtual void SyncWithMaster() override {}
  virtual void Calculate() override {}

  virtual void HandleRequest() override;

  virtual void HandleOtherRequests() {Requests.pop_front();}
  virtual void InitWorkers() {}
  virtual void GetField() = 0;

  unsigned WorkersCount;
};

class LocalMaster: public BaseMaster
{
public:
  virtual void SendFinalReport() override;
  virtual void TakeRequests() override; 
  virtual void SendRequest(BaseRequest req) override;
  virtual void GetField() override;
  void GetRandomField();
  vector<std::shared_ptr<LocalWorker>> Slaves;
  pthread_barrier_t WorkersBarrier;
  pthread_barrier_t MasterBarrier;
  vector<vector<vector<int>>> FieldsToSend;
  vector<BaseRequest> RequestsToSend;

  int Read();  
};

class ThreadMaster: public LocalMaster 
{
public:
  ThreadMaster();
  ~ThreadMaster();
  virtual void WorkersSync() override; 
  virtual void InitWorkers() override;
};