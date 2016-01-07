#include "master.h"
#include "threadworker.h"

class ThreadMaster: public LocalMaster 
{
public:
  ThreadMaster(int);
  ~ThreadMaster();
  virtual void WorkersSync() override; 
  virtual void InitWorkers() override;
  pthread_barrier_t WorkersBarrier;
  pthread_barrier_t MasterBarrier;
  vector<std::shared_ptr<ThreadWorker>> Slaves;
};