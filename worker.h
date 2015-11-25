#include <vector>
#include <pthread.h>
#include <memory>
#include <deque>
#include <iostream>

typedef enum {WAITING, RUNNING, STOPPED, ENDED} TState;

using std::vector;
using std::shared_ptr;
using std::deque;

class BaseWorker {
public:
  void* Handle();
protected:
  virtual void HandleRequest();  
  void MasterSync();

  virtual void Report() = 0;
  virtual void TakeRequests() = 0;
  virtual void SendCalculations() = 0;
  virtual void ReceiveCalculations() = 0;
  virtual void CollabSync() = 0;
  virtual void WakeUpMaster() = 0;
  virtual void SendFinalReport() = 0;
  virtual void Calculate() = 0;
  virtual void Sleep() = 0;

  virtual void Report() {}
  
  virtual ~BaseWorker() = default;

  vector<vector<int>>> Field;
  vector<vector<int>>> OldField;
  size_t IterNumber;  
  size_t IterCount;
  size_t Height, Width;
  unsigned int Id;
  TState State;
  deque<BaseRequest> Requests;
};


struct LocalWorkerDataCommon
{
  const vector<BaseRequest>* RequestsFromMaster;
};

struct ThreadWorkerDataCommon  
{
  pthread_barrier_t* Barrier;
  pthread_mutex_t* WorkersSleepLock;
  pthread_cond_t* WorkersSleepCV;
  pthread_cond_t* MasterSleepCV;
  pthread_mutex_t* RequestLock;
  
};

struct LocalWorkerDataInd
{
  vector<vector<int>>* SrcField;
  vector<vector<int>>* SendFieldTop;
  vector<vector<int>>* SendFieldBottom;
  vector<vector<int>>* ReceiveFieldTop;
  vector<vector<int>>* ReceiveFieldBottom;
};

struct LocalWorkerData : public ThreadWorkerDataCommon, public ThreadWorkerDataInd  
{}
class LocalWorker : public BaseWorker, public LocalWorkerData
{
protected:
  virtual void Calculate() override;
  virtual void SendFinalReport() override;
  virtual void SendCalculations() override;
  virtual void ReceiveCalculations() override;
  size_t NeighboursCount(size_t x, size_t y);
};

class ThreadWorker : public LocalWorker, public ThreadWorkerDataCommon
{
protected:
  virtual void TakeRequests() override;
  virtual void CollabSync() override;
  virtual void WakeUpMaster() override;
  virtual void Sleep() override;

  static bool UpdatingQueue;
  static unsigned int Updated; 
  size_t RequestQueuePosition;
  pthread_t pid;
  friend class ThreadMaster;

};
