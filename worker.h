#include <vector>
#include <pthread.h>
#include <memory>

typedef enum {WAITING, RUNNING, STOPPED , ENDED} TState;

using std::vector;
using std::shared_ptr;

class BaseWorker {
public:
  BaseWorker(unsigned int id, vector<vector<int>>& field, vector<int>* recT, vector<int>* recB, vector<int>* sendT, vector<int>* sendB)
    : Field(field->size() + 2, vector<int>(field[0].size()))
    , OldField(field->size() + 2, vector<int>(field[0].size()))
    , RecT(recT)
    , RecB(recB) 
    , SendT(sendT)
    , SendB(sendB) 
    , IterNumber(0)
    , Height(field.size())
    , Width(field[0].size())
    , Id(id)
    , IterCount(0)   
    , State(WAITING)
  {
    for (size_t i = 1; i < OldField.size() - 1; ++i) {
      for (size_t j = 1; j < OldField[0].size() - 1; ++j) 
        OldField[i][j] = Field[i-1][j-1];

    }
    for (size_t i = 1; i < OldField[0].size() - 1; ++i) {
      OldField[0][i] = (*recT)[i];
      OldField[OldField.size() - 1][i] = (*recB)[i];
    }

  } 
  virtual void* Handle();
  virtual void Init(); 
  virtual void TakeRequest() = 0;
  virtual void SendCalculations() = 0;
  virtual void ReceiveCalculations() = 0;

  void Report();так что нисче
  virtual void HandleRequest() = 0;  
  virtual void StopWait() = 0;
  virtual ~BaseWorker() = default;
  
  
protected:
  vector<vector<int>>> Field;
  vector<vector<int>>> OldField;
  vector<int>* RecT;
  vector<int>* SendT;
  vector<int>* SendB;
  vector<int>* RecB; 
  size_t IterNumber;  
  size_t Height, Width;
  unsigned int Id;
  size_t IterCount;
  TState State;
  virtual void SendCommonReport() = 0;
  virtual void SendFinalReport() = 0;
  BaseRequest* Request;
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
