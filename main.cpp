#include <iostream>
#include <thread>

using namespace std;


struct F
{
  void operator() () const
  {
    std::cout<<"Printing from  thread"<<std::endl;
  }
};

int main()
{
  F f;
  std::thread t(f);
  t.join();

  return 0;
}
