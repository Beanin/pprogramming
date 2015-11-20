#include "request.h"
#include "worker.h"

class Master : public virtual BaseWorker
{
public:
protected:
  virtual void HelpHandle()=0;
  BaseRequest* SendReq;
  virtual void StartHandle()=0;
  virtual void QuitHandle()=0;
  virtual void RunHandle()=0;
  virtual void StatusHandle()=0;
  virtual void StopHandle()=0;
  virtual ~Master() = default; 
  vector<vector<vector<int>>> WorkerFields;
  int WorkersCount;
};



class ThreadMaster():public Master {
public:
  ThreadMaster()
  {
    Updating = false;
    if (pthread_mutex_init(&StopLock))
    {
      //Check
    }
    if (pthread_mutex_init(&ReqLock))
    {
      //Check
    }
    if (pthread_cond_init(&ReqCV, NULL))
    {
      //Check
    }
    if (pthread_cond_init(&StopCV, NULL))
    {
      //Check
    }
    
  }
  
  void UpdReq();
  
  ~ThreadMaster()
  {
    pthread_mutex_destroy(&StopLock);
    pthread_mutex_destroy(&ReqLock);
    pthread_cond_destroy(&ReqCV);
    pthread_cond_destroy(&StopCV);
    pthread_barrier_destroy(&Barrier);
  }
  
  
protected:
  vector<ThreadWorkers> Workers;
  vector<pthread_t> Threads;
  int IterCount;
  vector<vector<int>> Post[2];
  pthread_cond_t UpdateCV;
  bool Updating;
  pthread_barrier_t Barrier;
  bool Ready;
  pthread_mutex_t StopLock;
  pthread_cond_t StopCV;
  pthread_cond_t ReqCV;
  pthread_mutex_t ReqLock;
  Request SendRequest;
}



 
