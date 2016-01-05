OmpWorker::OmpWorker(unsigned number, LocalWorkerData localData):LocalWorker(number, localData)
{}

OmpWorker::CollabSync()
{
  #pragma omp barrier
} 
