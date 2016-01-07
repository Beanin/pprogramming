#include <pthread.h>

#include "worker.h"

struct ThreadWorkerDataCommon  
{
  pthread_barrier_t* WorkersBarrier;
  pthread_barrier_t* MasterBarrier;
};

class ThreadWorker : public LocalWorker, public ThreadWorkerDataCommon
{
public:
  ThreadWorker(unsigned number, LocalWorkerData localData,
   ThreadWorkerDataCommon threadCommon, vector<BaseRequest>* req);
  virtual ~ThreadWorker() {
    pthread_join(pid, nullptr);
  }
  virtual void TakeRequests() override;
  virtual void CollabSync() override;
  virtual void SyncWithMaster() override;

  pthread_t pid;
  unsigned RequestQueuePosition;
  const vector<BaseRequest>* RequestsFromMaster;
};