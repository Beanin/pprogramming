#include "threadworker.h"

void* UseThread(void* arg)
{
  return ((ThreadWorker*)arg)->Handle(); 
}

void ThreadWorker::CollabSync()
{
  pthread_barrier_wait(WorkersBarrier);
}

void ThreadWorker::SyncWithMaster()
{
  pthread_barrier_wait(MasterBarrier);
}

ThreadWorker::ThreadWorker(unsigned number, LocalWorkerData localData,
 ThreadWorkerDataCommon threadCommon, vector<BaseRequest>* req):LocalWorker(number, localData),
  ThreadWorkerDataCommon(threadCommon), RequestsFromMaster(req)
{
  RequestQueuePosition = 0;
  pthread_create(&pid, nullptr, UseThread, this);
}  

void ThreadWorker::TakeRequests()
{
  SyncWithMaster();
  SyncWithMaster();
  for (; RequestQueuePosition < RequestsFromMaster->size(); ++RequestQueuePosition)
  {
    Requests.push_back((*RequestsFromMaster)[RequestQueuePosition]);
  }
}