
class OmpWorker : public LocalWorker 
{
public:
  OmpWorker(unsigned number, LocalWorkerData localData);
  virtual void CollabSync() override; 
  virtual void SyncWithMaster() override; /*TODO*/
}