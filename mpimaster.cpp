#include "mpimaster.h"
#include "sendrecv.h"
#include <mpi.h>

void MpiMaster::SendRequest(BaseRequest req)
{
	//printf("SEND!TEXT:%s\n",req.GetType().c_str());
	int code[3];
	
	ReqToCode(req, code);
	for (size_t i = 0; i < WorkersCount;++i)
	{
		MPI_Send(code, 3, MPI_INT, i + 1, REQ_SEND_CODE/*1024 + IterNumber % 1024*/ , MPI_COMM_WORLD);
	}
	int x;
	MPI_Status status;
	for (size_t i = 0; i < WorkersCount; ++i)
	{
		MPI_Recv(&x, 1 , MPI_INT, i + 1, REQ_ACCEPT_CODE/*1024 + IterNumber % 1024*/, MPI_COMM_WORLD, &status);	
	}
	//printf("SENDEND\n");
}

void MpiMaster::InitWorkers()
{  		

	SendRequest(Requests.front());

	FieldsToSend.resize(WorkersCount);

  for (size_t i = 0; i < WorkersCount; ++i) 
  {
    size_t sz = (Height - 1) / WorkersCount + 1;
    size_t y0 = sz * i;
    FieldsToSend[i].resize(std::min(sz + 2 ,Height + 2 - y0));
    for (size_t y = 0; y < std::min(sz + 2, Height + 2 - y0); ++y)
    {
      FieldsToSend[i][y] = Field[y0 + y]; 
    }
  }

  for (size_t i = 0; i < WorkersCount; ++i)
  {
    SendField(FieldsToSend[i], i + 1, SEND_FIELD_CODE);
  }


}

void MpiMaster::SendFinalReport()
{
  for (size_t i = 0; i < WorkersCount; ++i)
  {
    RecvField(FieldsToSend[i], i + 1, SEND_RES_CODE);
  }
  for (size_t i = 0; i < WorkersCount; ++i) 
  {
   	size_t sz = (Height - 1) / WorkersCount + 1;
   	size_t y0 = sz * i;
    for (size_t y = 0; y < std::min(sz + 2, Height + 2 - y0); ++y)
    {
      Field[y0 + y] = FieldsToSend[i][y];
    }
  }
  printf("Done!\n");
  IterNumber = 0;
}

void MpiMaster::Pass()
{
	if (Requests.empty())
		SendRequest(BaseRequest("0"));
}
