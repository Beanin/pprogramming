#include "request.h"
#include "worker.h"

class BaseMaster: public BaseWorker
{
};

struct ThreadMasterData;

class ThreadMaster: public BaseMaster, public ThreadMasterData 
{
  
} 
