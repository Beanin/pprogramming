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

ThreadWorker::ThreadWorker(unsigned number, LocalWorkerData localData, ThreadWorkerDataCommon threadCommon):LocalWorker(number, localData),
  ThreadWorkerDataCommon(threadCommon)
{
  pthread_create(&pid, nullptr, UseThread, this);
}  