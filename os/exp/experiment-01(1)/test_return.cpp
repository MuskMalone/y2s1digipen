// C++ example for return in main()
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
  
using namespace std;
  
class MyExitTest {
public:
    MyExitTest() { std::cout << "Inside MyExitTest's constructor\n"  << std::endl; }
  
    ~MyExitTest()
    {
        std::cout << "Inside MyExitTest's Destructor" << std::endl;
    }
};
  
int main()
{
    static MyExitTest m1;
    MyExitTest m2;
  
    return 0;
}