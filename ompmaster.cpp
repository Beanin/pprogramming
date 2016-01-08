#include <omp.h>

#include "ompmaster.h"

OmpMaster::OmpMaster(int i)
{
	WorkersCount = i;
	omp_set_num_threads(i);
}

void OmpMaster::SendFinalReport()
{
	Field = OldField;
  printf("Done\n");
  IterNumber = 0;
}

void OmpMaster::HandleOtherRequests()
{
  if (Requests.front() == "START" && State == WAITING)
  {
  	GetField();
  	InitWorkers();
  	Requests.pop_front();
  	State = STOPPED;
  }
  else 
  	LocalWorker::HandleOtherRequests();
}

void OmpMaster::InitWorkers() 
{
	OldField = Field;
	SrcField = &Field;
	SendFieldTop = &OldField.back();
	SendFieldBottom = &OldField[0];
	ReceiveFieldTop = &OldField[Field.size() - 1];
  OldField[0] = *ReceiveFieldTop;
	ReceiveFieldBottom = &OldField[1];
  OldField.back() = *ReceiveFieldBottom;
}

void OmpMaster::Calculate()
{
	const int size = OldField.size();
	#pragma omp parallel for 
	for (int y = 1; y < size - 1; ++y)
  {
    for (size_t x = 0; x < OldField[0].size(); ++x)
    {
      if (OldField[y][x] && (NeighboursCount(x, y) == 3 || NeighboursCount(x, y) == 4))
        Field[y][x] = 1;
      else if (OldField[y][x] == 0 && NeighboursCount(x, y) == 3)
        Field[y][x] = 1;
      else Field[y][x] = 0;   
    }
  }
  Field.swap(OldField);
}