#include <ctime>
#include <string>

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
    Type == "RUN";
  }
  bool operator==(std::string s)
    return (type.compare(s) == 0);
  virtual ~BaseRequest() = default; 
private: 
  time_t CreationTime;
  std::string Type;
  size_t IterNumber;
};

BaseRequest::~BaseRequest() {}











 
