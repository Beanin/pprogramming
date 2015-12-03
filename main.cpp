#include "master.h"

int main()
{
  PrintHelpMessage();	 
  set_fl(STDIN_FILENO, O_NONBLOCK); 
}