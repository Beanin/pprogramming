#include <pthread.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <exception>
#include <stdexcept>
#include <memory>
#include <iostream>

#include "worker.h"

using std::exception;
using std::vector;
using std::shared_ptr;
using std::cin;

void* UseThread(void* arg)
{
  (ThreadWorker*)arg->Handle(); 
}

void ThreadMaster::Init() {
  TakeRequest();
  HandleRequest();
}

void ThreadMaster::UpdReq(const Request& req)
{ 
   pthread_mutex_lock(&ReqLock);
   if (!Updating) 
    {
                    //Probably another mutex is needed
      if (pthread_cond_wait(&ReqCV, &ReqLock)) {
        perror("pthread_cond_wait");
        throw std::runtime_error("pthread_cond_wait");
      }
    }
    SendRequest = req;
    pthread_mutex_unlock(&ReqLock);
}

void ThreadMaster::HandleRequest(){
  if (Request.GetType() == "START")
    StartHandle();
  else if (Request.GetType() == "RUN")  
    RunHandle(Request.GetIterCount());
  else if (Request.GetType() == "STOP")
    StopHanlde();
  else if (Request.GetType() == "STATUS")
    StatusHandle();
  else if (Request.GetType() == "QUIT")
    QuitHandle();
  else 
    HelpHandle();
}

void ThreadMaster::TakeRequest() {
  std::string ReqType;
  std::cin >> ReqType;
  size_t icount = 0;
  if (ReqType == "RUN") 
  {
    std::cin >> icount;
  }
  if (Request) 
    delete Request;
  Request = new Request(ReqType, icount);
}

void ThreadMaster::StartHandle() {
  if (State != WAITING)
    std::cout<<"Already started"<<std::endl;
  else {
    State = STOPPED;
    GetData(WorkersCount, Height, Width, OldField);
    Threads.resize(WorkersCount);
    Post[0].resize(WorkersCount, std::vector<int>(Width));
    Post[1].resize(WorkersCount, std::vector<int>(Width));
    Field = OldField;
    if (pthread_barrier_init(&Barrier, nullptr, WorkersCount)) {
      perror("pthread_barrier_init");
      throw std::runtime_error("pthread_barrier_init");
    }
    /*if (pthread_barrier_init(&UpdateStableBarrier, nullptr, ThreadsCount)) {
      perror("pthread_barrier_init");
      throw std::runtime_error("pthread_barrier_init");
    }*/
    WorkerFields.resize(WorkersCount);
    Threads.resize(WorkersCount);
    for (size_t ThrdNum = 0; ThrdNum < ThreadsCount; ++ThrdNum) {
      vector<vector<int>> FieldToSend(
	min(Height / WorkersCount, Height - ThrdNum * (Height / WorkersCount)),vector<int>(Width));   
      for (size_t i = 0; i < FieldToSend.size(); ++i)
      {
	for (size_t j = 0; j < Width; ++j)
	{
	  FieldToSend[i][j] = Field[i + ThrdNum * (Height / WorkersCount), j];
	}
      }
      WorkerFields[ThrdNum] = std::move(FieldToSend);
      Post[0][ThrdNum] = Field[ThrdNum * (Height / WorkersCount)];
      Post[1][ThrdNum] = Field[ThrdNum * (Height / WorkersCount) + FieldToSend.size() - 1];
      
      Workers[ThrdNum] = ThreadWorker(ThrdNum, WorkerFields[ThrdNum], &Post[0][ThrdNum], &Post[1][ThrdNum],
	&Post[1][(ThrdNum + WorkersCount - 1) % WorkersCount], &Post[0][(ThrdNum + 1) % WorkersCount], &SendRequest
        &Barrier, &ReqLock, &ReqCV, &StopLock, &StopCV);
      if (pthread_create(&Threads[ThrdNum], NULL, UseThread ,&Workers[ThrdNum])) {
       perror("pthread_create");
       throw std::runtime_error("pthread_create");
      }
      std::cout << "Thread " << ThrdNum << " started\n";
    }
  }
}

ThreadMaster::HelpHandle()
{
  std::cout << "Available requests list:\n";
  std::cout << "START   : Initiates the game\n";
  std::cout << "RUN X   : Runs X iterations of stopped game\n";
  std::cout << "STATUS  : Shows field status\n";
  std::cout << "STOP    : Stops game calculating\n";
  std::cout << "QUIT    : Closes the program\n";
}
ThreadMaster::RunHandle()
{
  if (State != STOPPED)
    throw std::invalid_argument("Not able to run");
  std::cin >> IterCount;
  State = RUNNING;
}

ThreadMaster::StatusHandle()
{
  
}

ThreadMaster::StopHandle()
{
  if (State != STOPPED)
  {
    UpdReq(Request("STOP"));
    pthread_mutex_lock(&ReqLock);
    if (!Updating) 
    {
                    //Probably another mutex is needed
      if (pthread_cond_wait(&ReqCV, &ReqLock)) {
        perror("pthread_cond_wait");
        throw std::runtime_error("pthread_cond_wait");
      }
    }
    SendRequest = Request("STOP");
    pthread_mutex_unlock(&ReqLock);
    pthread_mutex_lock(&StopLock);
    if (pthread_cond_wait(&StopCV, &StopLock)) {
      perror("pthread_cond_wait");
      throw std::runtime_error("pthread_cond_wait");
    }
      //CHECK IT!!!
    pthread_mutex_unlock(&StopLock);
  }
}

/*void* UseThread(void* args) {
  Worker worker((TThreadArg*) args);
  worker.Handle();
}

void PrintStart(const vector<vector<int>> &Src, pthread_mutex_t *UpdateMtx, pthread_cond_t *UpdateCV, const bool &Updating) {
    pthread_mutex_lock(UpdateMtx);
    if (Updating) {
        pthread_cond_wait(UpdateCV, UpdateMtx);
    }
    for (size_t i = 0; i < Src.size(); ++i) {
        for (size_t j = 0; j < Src[i].size(); ++j) {
            std::cout << Src[i][j];
        }
        std::cout << std::endl;
    }
    pthread_mutex_unlock(UpdateMtx);
}

vector<vector<int>> GetFieldFromFile(size_t &ThreadsCount, size_t &Height, size_t &Width, vector<vector<int>> &Field) {
    vector<vector<int>> Res;
    return Res;
}

void GetRandomField(size_t &ThreadsCount, size_t &Height, size_t &Width, vector<vector<int>> &Field) {
    size_t Probability;
    std::cout << "Enter count of threads, height and width of field" << std::endl;
    std::cin >> ThreadsCount >> Height >> Width;
    Probability = 40;
    srand(time(0));
    Field.resize(Height);
    for (size_t i = 0; i < Height; ++i) {
       Field[i].resize(Width);
       for (size_t j = 0; j < Width; ++j)
         Field[i][j] = rand() % 100 < Probability ? 1 : 0;
    }
}

void GetData(size_t &ThreadsCount, size_t &Height, size_t &Width, vector<vector<int>> &Field) {
    std::string type;
    bool read = false;
    std::cout << "Choose CVS or RAND type:" << std::endl;
    while (!read) {
    read = true;
    std::cin >> type;
      if (type == "CVS") {
          GetFieldFromFile(ThreadsCount, Height, Width, Field);
      }
      else if (type == "RAND")
      {
          GetRandomField(ThreadsCount, Height, Width, Field);
      } else {
         read = false;
         std::cout << "Type CVS or RAND\n";
      }
    }
}


void Work() {
    size_t ThreadsCount;
    vector<vector<int>> FieldTemporary;
    vector<vector<int>> FieldStable;
    vector<vector<int>> StartField;
    TState State = WAITING;
    size_t Height;
    size_t Width;
    vector<pthread_t> Threads;
    pthread_barrier_t UpdateBarrier, UpdateStableBarrier;
    pthread_mutex_t UpdateStableMtx;
    if (pthread_mutex_init(&UpdateStableMtx, nullptr)) {
        perror("pthread_mutex_init");
        throw std::runtime_error("pthread_mutex_init");
    }
    pthread_cond_t UpdateCV;
    if (pthread_cond_init(&UpdateCV, nullptr)) {
        perror("pthread_cond_init");
        throw std::runtime_error("pthread_cond_init");
    }
    std::string Request;
    bool Updating = false;
    int IterCount;
    vector<std::shared_ptr<TThreadArg>> ThreadArgs;
    std::cout << "Waiting for request. \"HELP\" request might help you. " << std::endl;
    while (std::cin >> Request) {
        if (Request == "START") {
            if (State != WAITING)
                std::cout<<"Already started"<<std::endl;
            else {
                State = STOPPED;
                GetData(ThreadsCount, Height, Width, FieldStable);
                Threads.resize(ThreadsCount);
                FieldTemporary = FieldStable;
                StartField = FieldStable;
                if (pthread_barrier_init(&UpdateBarrier, nullptr, ThreadsCount)) {
                    perror("pthread_barrier_init");
                    throw std::runtime_error("pthread_barrier_init");
                }
                if (pthread_barrier_init(&UpdateStableBarrier, nullptr, ThreadsCount)) {
                    perror("pthread_barrier_init");
                    throw std::runtime_error("pthread_barrier_init");
                }
                ThreadArgs.resize(ThreadsCount, nullptr);
                for (size_t ThrdNum = 0; ThrdNum < ThreadsCount; ++ThrdNum) {
                    std::shared_ptr<TThreadArg> Arg (new TThreadArg);
                    Arg->GlobalFieldPtr = &FieldTemporary;
                    Arg->GlobalStableFieldPtr = &FieldStable;
                    Arg->StartField = &StartField;
                    Arg->UpdateStableMtx = &UpdateStableMtx;
                    Arg->UpdateStableBarrier = &UpdateStableBarrier;
                    Arg->UpdateBarrier = &UpdateBarrier;
                    Arg->UpdateCV = &UpdateCV;
                    Arg->CornerY = Height / ThreadsCount * ThrdNum;
                    Arg->Height = std::min(Height / ThreadsCount, Height - Arg->CornerY);
                    Arg->Width = Width;
                    Arg->CornerX = 0;
                    Arg->State = &State;
                    Arg->Updating = &Updating;
                    Arg->ThrdNum = ThrdNum;
                    Arg->IterCount = &IterCount;
                    ThreadArgs[ThrdNum] = Arg;
                    if (pthread_create(&Threads[ThrdNum], NULL, UseThread ,Arg.get())) {
                        perror("pthread_create");
                        throw std::runtime_error("pthread_create");
                    }
#ifdef DEBUG
                    std::cout << "Thread " << ThrdNum << " started\n";
#endif
                }
            }
        } else if (Request == "RUN") {
            if (State != STOPPED)
                throw std::invalid_argument("Not able to run");
            std::cin >> IterCount;
            State = RUNNING;
        } else if (Request == "STATUS") {
            PrintStart(StartField, &UpdateStableMtx, &UpdateCV, Updating);
        } else if (Request == "STOP") {
            if (State != STOPPED) {
                pthread_mutex_lock(&UpdateStableMtx);
                if (!Updating) {
                    //Probably another mutex is needed
                    if (pthread_cond_wait(&UpdateCV, &UpdateStableMtx)) {
                        perror("pthread_cond_wait");
                        throw std::runtime_error("pthread_cond_wait");
                    }
                }
                State = STOPPED;
                if (pthread_cond_wait(&UpdateCV, &UpdateStableMtx)) {
                    perror("pthread_cond_wait");
                    throw std::runtime_error("pthread_cond_wait");
                }
               //CHECK IT!!!
                pthread_mutex_unlock(&UpdateStableMtx);
            }
        } else if (Request == "QUIT") {
            if (State != STOPPED) 
                std::cout << "Stop before ending" << std::endl;
            else {
                State = ENDED;
                break;
            }
        } else if (Request == "HELP") {
            std::cout << "Available requests list:\n";
            std::cout << "START   : Initiates the game\n";
            std::cout << "RUN X   : Runs X iterations of stopped game\n";
            std::cout << "STATUS  : Shows field status\n";
            std::cout << "STOP    : Stops game calculating\n";
            std::cout << "QUIT    : Closes the program\n";
        } else
            std::cout << "Wrong request" << std::endl;
        std::cout << "Waiting for request. \"HELP\" request might help you. " << std::endl;
    }
    
    for (size_t ThrdNum = 0; ThrdNum < ThreadsCount; ++ThrdNum) {
#ifdef DEBUG
        std::cout <<" lol";
#endif
        pthread_join(Threads[ThrdNum], NULL);
    }
    pthread_mutex_destroy(&UpdateStableMtx);
    pthread_barrier_destroy(&UpdateStableBarrier);
    pthread_barrier_destroy(&UpdateBarrier);
}

int main() {
    master();
}*/
