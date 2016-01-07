#include "mpiworker.h"
#include "sendrecv.h"
#include <mpi.h>

void MpiWorker::TakeRequests()
{
	//printf("TAKE.Id:%dIter:%d.Rsize:%d\n", Id, IterNumber,Requests.size());
	int code[3];
	MPI_Status status;
	MPI_Recv(code, 3, MPI_INT, 0,  REQ_SEND_CODE , MPI_COMM_WORLD, &status);
	int x = true;
	MPI_Send(&x, 1, MPI_INT, 0,  REQ_ACCEPT_CODE , MPI_COMM_WORLD);
	BaseRequest req = CodeToReq(code);
	//printf("TAKEEND.Id:%dIter:%d!TEXT:%s\n", Id, IterNumber, req.GetType().c_str());
	if (req.GetType() != "0")
		Requests.push_back(req); 
}

MpiWorker::MpiWorker(int rank, int workersCount)
{
	IterNumber = 0;
	CommSize = workersCount;
	Id = rank;
	Field.resize(1);
	OldField.resize(1);
}



void MpiWorker::HandleOtherRequests()
{
	if (Requests.front() == "START" && State == WAITING)
	{
		RecvField(OldField, 0, SEND_FIELD_CODE);
		Field = OldField;
		Height = OldField.size() - 2;
		Width = OldField[0].size();
		State = STOPPED;
		Requests.pop_front();
	}
	else 
	{
	  fprintf(stderr, "Request handling error! Request: %s\nId:%u\n", Requests.front().GetType().c_str(), Id);
  	Requests.pop_front();
  }
}

void MpiWorker::SendCalculations()
{
	//printf("Send.Id:%d\n", Id);
	if (CommSize == 1) 
	{
		OldField[0] = OldField[OldField.size() - 2];
		OldField.back() = OldField[1];
		return;
	}
	SendRow(OldField[1], (Id + CommSize - 2) % CommSize + 1, SEND_CALC_TOP_CODE);
	SendRow(OldField[OldField.size()-1], Id % CommSize + 1, SEND_CALC_BOT_CODE);
}

void MpiWorker::ReceiveCalculations()
{
	//printf("Rec.Id:%d\n", Id);
	if (CommSize == 1)
		return;
	RecvRow(OldField[0], (Id + CommSize - 2) % CommSize + 1, SEND_CALC_BOT_CODE);
	RecvRow(OldField.back(), Id % CommSize + 1, SEND_CALC_TOP_CODE);
}

void MpiWorker::SendFinalReport()
{
	//printf("GoodGame");
	SendField(OldField, 0, SEND_RES_CODE);
	IterNumber = 0;
}

