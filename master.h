#include "request.h"
#include "worker.h"

class BaseMaster: public BaseWorker
{
  virtual void WakeUpSlaves() = 0; 
  virtual void Sleep() override {}
  virtual void WakeUpMaster() override {}
  virtual void SendCalculations() override {}
  virtual void ReceiveCalculations() override {}
  virtual void CollabSync() override {};
  virtual void Calculate() override {};

};

class LocalMaster: public BaseMaster
{
public:
  virtual void WakeUpSlaves() override; 
  virtual void SendFinalReport() override;
  virtual void TakeRequests() override; 
  virtual void Report() override;


protected:
  vector<TThreadWorker> Slaves;
  pthread_barrier_t WorkersBarrier;
  pthread_mutex_t WorkersSleepLock;
  pthread_cond_t WorkersSleepCV;
  pthread_cond_t MasterSleepCV;
  pthread_mutex_t RequestLock;
  vector<vector<vector<int>>> FieldsToSend;
  vector<BaseRequest> RequestsToSend;
};
