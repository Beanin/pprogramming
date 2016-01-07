#include "mpiworker.h"
#include "convert.h"
#include "sendrecv.h"

MpiWorker::MpiWorker(int rank, int workersCount)
{
	IterNumber = 0;
	CommSize = workersCount;
	Id = rank;
	Field.resize(1);
	OldField.resize(1);
}

void MpiWorker::TakeRequests()
{
	int code[3];
	MPI_Bcast(code, 3, MPI_INT, 0 , MPI_COMM_WORLD);
	BaseRequest req = CodeToReq(code);
	if (req.GetType() != "0")
		Requests.push_back(req); 
}

void MpiWorker::HandleOtherRequests()
{
	if (Requests.front() == "START" && State == WAITING)
	{
		int size[2];
		int h = Requests.front().y;
		Width = Requests.front().x;
		int sz = (h - 1) / CommSize + 1;
		int y0 = sz * (Id - 1);
		Height = std::min(sz + 2, h + 2 - y0) - 2;
		//printf("H:%d,W:%d!Id:%d\n", Height, Width, Id);
		OldField.resize(Height + 2);
		for (size_t j = 0; j < OldField.size(); ++j)
			OldField[j] = std::vector<int>(Width, 0);
		//printf("H:%d,W:%d!Id:%d\n", OldField.size(), OldField[0].size(), Id);
		int buf[(Height + 2) * Width];
		MPI_Scatterv(nullptr, nullptr, nullptr, MPI_INT, buf, (Height + 2) * Width,
		 MPI_INT, 0, MPI_COMM_WORLD);
		//printf("H:%d,W:%d!Id:%d\n", OldField.size(), OldField[0].size(), Id);
		BufToField(buf, OldField);
		Field = OldField;
		
		//printf("H:%d,W:%d!Id:%d\n", Field.size(), Field[0].size(), Id);
		State = STOPPED;
		Requests.pop_front();
	}
	else 
	{
	  fprintf(stderr, "Request handling error! Request: %s\nId:%u\n", Requests.front().GetType().c_str(), Id);
  	Requests.pop_front();
  }
}
void MpiWorker::SendFinalReport()
{
	int buf[(Height + 2) * Width];
	FieldToBuf(buf, OldField);
	MPI_Gatherv(buf, (Height + 2) * Width, MPI_INT, nullptr, nullptr, nullptr, MPI_INT, 0, MPI_COMM_WORLD);
	IterNumber = 0; 
}

void MpiWorker::SendCalculations()
{
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
	//printf("Rec.Id.:%dS1:%d S2%d\n", Id, OldField[0].size(), OldField.back().size());
	if (CommSize == 1)
		return;

	RecvRow(OldField[0], (Id + CommSize - 2) % CommSize + 1, SEND_CALC_BOT_CODE);
	RecvRow(OldField.back(), Id % CommSize + 1, SEND_CALC_TOP_CODE);
}