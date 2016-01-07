#pragma once
#include "master.h"

inline void ReqToCode(BaseRequest req, int* code)
{
	code[0] = 0;
	code[1] = 0;
	code[2] = 0;
	if (req == "START")
	{
		code[0] = 1;
		code[1] = req.y;
		code[2] = req.x;
		return;
	}
	if (req == "RUN")
	{
		code[0] = 2;
		code[1] = req.IterCount;
		return; 
	}
	if (req == "STOP")
	{
		code[0] = 3;
		return;
	}
	if (req == "QUIT")
	{	
		code[0] = 4;
		return;
	}
}

class MpiMaster : public LocalMaster
{
public:
	MpiMaster(int workersCount){WorkersCount=workersCount;}

	virtual void SendRequest(BaseRequest req) override;
	virtual void SendFinalReport() override;
	virtual void InitWorkers() override;
	virtual void Pass() override;

	virtual void WorkersSync() override {}
};