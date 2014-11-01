#include <cstring>
#include <errno.h>
#include <iomanip>
#include <iostream>

#include "helpers.h"
#include "colors.h"

using namespace Colors;

void ok()
{
  std::cout << green << " [ OK ]" << reset << std::endl;
}

void error()
{
  std::cout << red << " [ ERROR ]" << reset << std::endl;
  std::cout << strerror(errno) << " (errno=" << errno << ")." << std::endl;
}
