// jack.cpp
// -- Jack isn't always well-behaved.
// csd2180/csd2182 9/23

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


int main(void) {
  srand(unsigned(time(0)));
  if (rand()%5 != 0)
    cout << "All work and no play makes Jack a dull boy." << endl;
  else
    while (true);
  return 0;
}
