#include <stdio.h>

int main(void) {
#if defined(STATIC) || !defined(EXTENDIBLE)
  printf("Static\n");
#else
  printf("Extendible\n");
#endif
  return 0;
}
