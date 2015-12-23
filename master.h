#include "request.h"
#include "worker.h"
#include <memory>

class BaseMaster: public BaseWorker
{
public:
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
  virtual void InitWorkers() {};
  virtual void HandleRequest() override;

  unsigned WorkersCount;
};

class LocalMaster: public BaseMaster
{
public:
  virtual void WakeUpSlaves() override; 
  virtual void SendFinalReport() override;
  virtual void TakeRequests() override; 
  virtual void Report() override;
  virtual void HandleOtherRequests() override;

  vector<std::shared_ptr<LocalWorker>> Slaves;
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
public:
  ThreadMaster();
  ~ThreadMaster();
  virtual void InitWorkers() override;
  virtual void InitWorker(unsigned i) override;
};