#pragma once
#include <ctime>
#include <string>
#include <iostream>

inline void PrintHelpMessage() { 
  std::cout << "Available requests list:\n";
  std::cout << "START Y Z : Initiates the game on Y x Z  field\n";
  std::cout << "RUN X     : Runs X iterations of stopped game\n";
  std::cout << "STATUS    : Shows field status\n";
  std::cout << "STOP      : Stops game calculating\n";
  std::cout << "QUIT      : Closes the program\n";
}
  
class BaseRequest { 
public:
  BaseRequest(std::string type, size_t IterCount_ = 0):Type(type), IterCount(IterCount_) {
    CreationTime = clock();
  }
  BaseRequest() = default;
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
  {
    return s == Type;
  }
  virtual ~BaseRequest() = default; 
public: 
  time_t CreationTime;
  std::string Type;
  size_t IterCount;
  int y;
  int x; 
};












 
