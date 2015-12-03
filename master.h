#include "request.h"
#include "worker.h"

class BaseMaster: public BaseWorker
{
protected:
  virtual void WakeUpSlaves() = 0; 
  virtual void InitWorker(unsigned number) = 0;
  virtual void SendRequest(BaseRequest req) = 0;
  virtual void UpdateRequest() = 0;
  virtual void WaitForSlaves() = 0;
  
  virtual void Sleep() override {}
  virtual void WakeUpMaster() override {}
  virtual void SendCalculations() override {}
  virtual void ReceiveCalculations() override {}
  virtual void CollabSync() override {};
  virtual void Calculate() override {};
  virtual void HandleOtherRequests() {Requests.pop_front()};

  virtual void HandleRequest() override;
  virtual void GetStartRequest();

  unsigned WorkersCount;
};

class LocalMaster: public BaseMaster
{
protected:
  virtual void WakeUpSlaves() override; 
  virtual void SendFinalReport() override;
  virtual void TakeRequests() override; 
  virtual void Report() override;
  virtual void InitWorkers() override;
  virtual void HandleOtherRequests() override;
  virtual void GetStartRequest() override; 

  vector<LocalWorker> Slaves;
  pthread_barrier_t WorkersBarrier;
  pthread_mutex_t WorkersSleepLock;
  pthread_cond_t WorkersSleepCV;
  pthread_cond_t MasterSleepCV;
  pthread_mutex_t RequestLock;
  vector<vector<vector<int>>> FieldsToSend;
  vector<BaseRequest> RequestsToSend;
};

class ThreadMaster: public LocalMaster 
{
};