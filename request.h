#include <ctime>
#include <string>

inline void PrintHelpMessage() { 
  std::cout << "Available requests list:\n";
  std::cout << "START   : Initiates the game\n";
  std::cout << "RUN X   : Runs X iterations of stopped game\n";
  std::cout << "STATUS  : Shows field status\n";
  std::cout << "STOP    : Stops game calculating\n";
  std::cout << "QUIT    : Closes the program\n";
}
/*
  STOP
  START
  RUN X
  QUIT

  STATUS
  HELP
 */
  
class BaseRequest { 
public:
  BaseRequest(std::string type, size_t IterCount_ = 0):Type(type), IterCount(IterCount_) {
    CreationTime = clock();
  }
  time_t GetCreationTime() {
    return CreationTime;
  }
  std::string GetType() {
    return Type;     
  }
  size_t GetIterCount() {
    return (Type == "RUN") ? IterCount : 0;
  }
  bool operator==(std::string s)
    return (type.compare(s) == 0);
  virtual ~BaseRequest() = default; 
private: 
  time_t CreationTime;
  std::string Type;
  size_t IterCount;
};

BaseRequest::~BaseRequest() {}











 
