#include <stdio.h>

int main(void) {
#if STATIC
  printf("Static\n");
#elif EXTENDIBLE
  printf("Extendible\n");
#endif
  return 0;
}
