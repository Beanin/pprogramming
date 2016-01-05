#include "ompmaster.h"

int main(int argc, char** argv)
{
  int WorkersCount;	
  if (argc == 1)
    WorkersCount = 1;
  else	
    WorkersCount = atoi(argv[1]);
  OmpMaster master(WorkersCount);
  master.Id = -1;
  master.Handle();
}