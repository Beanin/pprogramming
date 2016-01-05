#pragma once
#include "request.h"
#include "worker.h"
#include <memory>

class BaseMaster: public virtual BaseWorker
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

  virtual void InitWorkers() = 0;
  virtual void GetField() = 0;

  unsigned WorkersCount;
};

class LocalMaster: public virtual BaseMaster
{
public:
  LocalMaster();
  ~LocalMaster();
  virtual void SendFinalReport() override;
  virtual void TakeRequests() override; 
  virtual void SendRequest(BaseRequest req) override;
  virtual void GetField() override;
  void GetRandomField();
  vector<std::shared_ptr<LocalWorker>> Slaves;
  vector<vector<vector<int>>> FieldsToSend;
  vector<BaseRequest> RequestsToSend;
};

