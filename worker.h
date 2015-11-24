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
    /*for (size_t i = 1; i < OldField.size() - 1; ++i) {
      for (size_t j = 1; j < OldField[0].size() - 1; ++j) 
        OldField[i][j] = Field[i-1][j-1];

    }
    for (size_t i = 1; i < OldField[0].size() - 1; ++i) {
      OldField[0][i] = (*recT)[i];
      OldField[OldField.size() - 1][i] = (*recB)[i];
    }*/
   
  void* Handle();
  void HandleRequest();  
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
  virtual void HandleOtherRequests() {}
  
  virtual ~BaseWorker() = default;
protected:
  vector<vector<int>>> Field;
  vector<vector<int>>> OldField;
  size_t IterNumber;  
  size_t IterCount;
  size_t Height, Width;
  unsigned int Id;
  TState State;
  deque<BaseRequest> Requests;
};



struct ThreadWorkerData
{
  pthread_barrier_t* WorkersBarrier;
  pthread_mutex_t* WorkersSleepLock;
  pthread_cond_t* WorkersSleepCV;
  pthread_cond_t* MasterSleepCV;
  pthread_mutex_t* RequestLock;
  vector<vector<int>>* SrcField;
  vector<vector<int>>* SendFieldTop;
  vector<vector<int>>* SendFieldBottom;
  vector<vector<int>>* ReceiveFieldTop;
  vector<vector<int>>* ReceiveFieldBottom;
  const vector<BaseRequest>* RequestsFromMaster;
};

class ThreadWorker : public BaseWorker, public ThreadWorkerData 
{
public:
  virtual void MasterSync() override;
  virtual void TakeRequests() override;
  virtual void SendCalculations() override;
  virtual void ReceiveCalculations() override;
  virtual void CollabSync() override;
  virtual void WakeUpMaster() override;
  virtual void SendFinalReport() override;
  virtual void Calculate() override;
  virtual void Sleep() override;


  static bool UpdatingQueue;
  static unsigned int Updated; 
  size_t RequestQueuePosition;
  pthread_t pid;
protected:
  size_t NeighboursCount(size_t x, size_t y);
};
