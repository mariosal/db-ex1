#include <stdio.h>
#include "BF.h"
#if defined(STATIC) || !defined(EXTENDIBLE)
#include "hash.h"
#else
#include "exhash.h"
#endif

int main(void) {
  BF_Init();
#if defined(STATIC) || !defined(EXTENDIBLE)
  printf("Static\n");
#else
  printf("Extendible\n");
#endif
  return 0;
}
