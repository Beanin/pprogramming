#include "mpimaster.h"
#include "convert.h"
#include <mpi.h>

void MpiMaster::SendRequest(BaseRequest req)
{
	int code[3];
	ReqToCode(req, code);
	MPI_Bcast(code, 3, MPI_INT, 0, MPI_COMM_WORLD);
}



void MpiMaster::InitWorkers()
{
	SendRequest(Requests.front());
	int sizes[WorkersCount + 1];
	int shifts[WorkersCount + 1];
	int buf[Width * (Height + WorkersCount * 2)];
	shifts[0] = 0;
  sizes[0] = 0;
  buf[0] = 0;
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
    sizes[i + 1] = FieldsToSend[i].size() * Width;
    shifts[i + 1] = shifts[i] + sizes[i]; 
    FieldToBuf(buf + shifts[i + 1], FieldsToSend[i]);
  }
  int b;
  MPI_Scatterv(buf, sizes, shifts, MPI_INT, 
    &b, 0, MPI_INT, 0, MPI_COMM_WORLD);
}

void MpiMaster::SendFinalReport()
{
  int sizes[WorkersCount + 1];
  int shifts[WorkersCount + 1];
  int buf[Width * (Height + WorkersCount * 2)];
  shifts[0] = 0;
  sizes[0] = 0;
//printf("HI!BUFF SIZE IS %d\n", Width * (Height + WorkersCount * 2));
//printf("H:%dW:%dWC:%d\n", Height, Width, WorkersCount);

  for (size_t i = 0; i < WorkersCount; ++i) 
    {
    size_t sz = (Height - 1) / WorkersCount + 1;
    size_t y0 = sz * i;
    sizes[i + 1] = FieldsToSend[i].size() * Width;
    shifts[i + 1] = shifts[i] + sizes[i]; 
  }
  int b = 0;
  MPI_Gatherv(&b, 0, MPI_INT, buf, sizes, shifts, MPI_INT, 
    0, MPI_COMM_WORLD);
  for (size_t i = 0; i < WorkersCount; ++i) 
  {
    size_t sz = (Height - 1) / WorkersCount + 1;
    size_t y0 = sz * i;
    BufToField(buf + shifts[i + 1], FieldsToSend[i]);
    for (size_t y = 1; y < std::min(sz + 2, Height + 2 - y0) - 1; ++y)
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