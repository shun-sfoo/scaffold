#include "util.h"

#include <cstdio>
#include <cstdlib>

void errif(bool condition, const char* errMsg) {
  if (condition) {
    perror(errMsg);
    exit(EXIT_FAILURE);
  }
}
