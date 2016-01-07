#pragma once
#include "worker.h"

inline BaseRequest CodeToReq(int* code)
{	
	if (code[0] == 1) 
	{
		BaseRequest req("START");
		req.y = code[1];
		req.x = code[2];
		return req;
	}
	if (code[0] == 2)
	{
		BaseRequest req("RUN");
		req.IterCount = code[1];
		return req;
	}
	if (code[0] == 3)
		return BaseRequest("STOP");
	if (code[0] == 4)
		return BaseRequest("QUIT");
	else 
		return BaseRequest("0");
}


class MpiWorker: public BaseWorker
{
public:
	MpiWorker(int rank, int workersCount);
	virtual void HandleOtherRequests() override;
	virtual void SendCalculations() override;
  virtual void ReceiveCalculations() override;
  virtual void SendFinalReport() override;
  virtual void CollabSync() {}
  virtual void SyncWithMaster() {} 
  virtual void TakeRequests() override;
  int CommSize;
};