#include "threadmaster.h"

void ThreadMaster::InitWorkers() {
  ThreadWorkerDataCommon threadCommon;
  threadCommon.WorkersBarrier = &WorkersBarrier;
  threadCommon.MasterBarrier = &MasterBarrier;
  for (size_t i = 0; i < WorkersCount; ++i) 
  {
    size_t sz = (Height - 1) / WorkersCount + 1;
    size_t y0 = sz * i;
    FieldsToSend[i].resize(std::min(sz + 2 ,Height + 2 - y0));
    for (size_t y = 0; y < std::min(sz + 2, Height + 2 - y0); ++y)
    {
      FieldsToSend[i][y] = Field[y0 + y]; 
    }
  }
  for (size_t i = 0; i < WorkersCount; ++i)
  {
    LocalWorkerData localData;
    localData.SrcField = &FieldsToSend[i];
    localData.SendFieldBottom = &FieldsToSend[(i + 1) % WorkersCount][0]; 
    localData.SendFieldTop = &FieldsToSend[(i + WorkersCount - 1) % WorkersCount].back();
    localData.ReceiveFieldBottom = &FieldsToSend[i][FieldsToSend[i].size() - 1];
    localData.ReceiveFieldTop = &FieldsToSend[i][0];
    localData.RequestsFromMaster = &RequestsToSend;
    localData.RequestQueuePosition = 0;
    Slaves.push_back(std::shared_ptr<ThreadWorker>(new ThreadWorker(i, localData, threadCommon)));
  }
}

void ThreadMaster::WorkersSync()
{
  pthread_barrier_wait(&MasterBarrier);
}

ThreadMaster::ThreadMaster(int WorkersCount_)
{
  IterNumber = 0;
  WorkersCount = WorkersCount_;
  PrintHelpMessage();
  pthread_barrier_init(&WorkersBarrier, nullptr, WorkersCount);
  pthread_barrier_init(&MasterBarrier, nullptr, WorkersCount + 1);
  State = WAITING; 
}

ThreadMaster::~ThreadMaster() 
{
  pthread_barrier_destroy(&WorkersBarrier);
  pthread_barrier_destroy(&MasterBarrier);
}
