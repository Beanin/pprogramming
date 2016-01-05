#include "master.h"
#include "threadmaster.h"

int main(int argc, char** argv)
{
  int WorkersCount;	
  if (argc == 1)
    WorkersCount = 1;
  else	
    WorkersCount = atoi(argv[1]);	
  ThreadMaster master(WorkersCount);
  master.Id = -1;
  master.Handle();
}