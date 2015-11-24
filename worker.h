#include <vector>
#include <pthread.h>
#include <memory>
#include <queue>

typedef enum {WAITING, RUNNING, STOPPING, STOPPED, ENDED} TState;

using std::vector;
using std::shared_ptr;
using std::queue;

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
   
  virtual void* Handle();
  virtual void Init(); 
  virtual void TakeRequest() = 0;
  virtual void SendCalculations() = 0;
  virtual void ReceiveCalculations() = 0;

  void Report();
  virtual void HandleRequest() = 0;  
  virtual void StopWait() = 0;
  virtual void CollabSync() = 0;
  virtual void MasterSync() = 0;
  virtual void SendCommonReport() = 0;
  virtual void SendFinalReport() = 0;
  virtual void Calculate() = 0;
  virtual ~BaseWorker() = default;
  virtual void Sleep() = 0;
  virtual void End() = 0;
  class ReportConfig;
  class CollabConfig;
  
protected:
  vector<vector<int>>> Field;
  vector<vector<int>>> OldField;
  size_t IterNumber;  
  size_t IterCount;
  size_t Height, Width;
  unsigned int Id;
  TState State;
  queue<BaseRequest> Requests;
  ReportConfig Master;
  ReportConfig Sender;
  ReportConfig Receiver;
  bool ReqDone;
};

class ThreadWorker : public BaseWorker {
public:
  ThreadWorker();
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
  BaseRequest StoredRequest;
};
