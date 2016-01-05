#include "master.h"

class OmpMaster: virtual public LocalMaster, virtual public LocalWorker
{
public:
	OmpMaster(int);
	virtual void CollabSync() override {};
	virtual void SyncWithMaster() override {};
	virtual void SendRequest(BaseRequest) override {}
	virtual void WorkersSync() override {}

	virtual void TakeRequests() override 
	{
		LocalMaster::TakeRequests();
	}
	virtual void HandleRequest() override
	{
		LocalWorker::HandleRequest();
	}  
	virtual void SendCalculations() override 
	{
		LocalWorker::SendCalculations();
	}
	virtual void ReceiveCalculations() override
	{
		LocalWorker::ReceiveCalculations();
	}
	virtual void SendFinalReport() override;

	virtual void GetField() override
	{
		LocalMaster::GetField();
	}

	virtual void HandleOtherRequests() override; 
	virtual void Calculate() override; 
	virtual void InitWorkers() override;
	//void* Handle(); 
};