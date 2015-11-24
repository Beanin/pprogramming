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
  virtual void SendCommonReport() = 0;
  virtual void SendFinalReport() = 0;
  virtual void Calculate() = 0;
  virtual void Sleep() = 0;
  
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



class ThreadWorkerData
{
  pthread_barrier_t* WorkersBarrier;
  pthread_mutex_t* WorkersSleepLock;
  pthread_cond_t* WorkersSleepCV;
  pthread_mutex_t* MasterSleepLock;
  pthread_cond_t* MasterSleepCV;
  vector<vector<int>>* SrcField;
  vector<vector<int>>* SendField;
  vector<vector<int>>* ReceiveField;
};

class ThreadWorker : public BaseWorker, public ThreadWorkerData 
{
public:
  virtual void LockQueue();
  virtual void UnlockQueue(); 
  virtual MasterSync() override;
  virtual void TakeRequests() override;
  virtual void SendCalculations() override;
  virtual void ReceiveCalculations() override;
  virtual void CollabSync() override;
  virtual void WakeUpMaster() override;
  virtual void SendCommonReport() override;
  virtual void SendFinalReport() override;
  virtual void Calculate() override;
  virtual void Sleep() override;
  virtual void Report() override;
  
  static bool UpdatingQueue;
  size_t RequestQueuePosition;  
 /* ThreadWorker();
  ThreadWorker(unsigned int id, vector<vector<int>>& field, vector<int>* sendT, vector<int>* sendB, vector<int>* recT,
               vector<int>* recB, BaseRequest* request, pthread_barrier_t* barrier, pthread_mutex_t* reqLock, 
               pthread_cond_t reqCV, pthread_mutex_t* stopLock, pthread_cond_t* stopCV, bool* updating, pthread_t thrid);
  void Init() override; 
  void TakeRequest() override;
  void SendCommonRecord() override;
  void SendFinalReport() override;
  void HandleRequest() override; 
  void StopWait() override;
  ~ThreadWorker() {
    pthread_join(Thrid, NULL);    
  }
protected:
  pthread_mutex_t* UpdateMtx;
  pthread_cond_t* UpdateCV;
  vector<vector<int>> *SendField;
  bool* Updating;  
  pthread_barrier_t* Barrier; 
  pthread_mutex_t* StopLock;
  pthread_cond_t* StopCV;  
  size_t NeighboursCount(size_t , size_t); 
  bool Reported;
  pthread_cond_t* ReqCV; 
  pthread_mutex_t* ReqLock;
  pthread_t Thrid;
  BaseRequest StoredRequest;*/
};
