class OmpMaster: public LocalMaster
{
  virtual void WorkersSync() override; 
  virtual void InitWorkers() override;
}