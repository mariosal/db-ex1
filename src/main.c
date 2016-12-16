#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "BF.h"
#if defined(STATIC) || !defined(EXTENDIBLE)
#include "hash.h"
#else
#include "exhash.h"
#endif

static bool Equals(const char* a, const char* b) {
  if (a[0] == '-') {
    return Equals(a + 1, b);
  }
  if (b[0] == '-') {
    return Equals(a, b + 1);
  }
  if (tolower(a[0]) == tolower(b[0])) {
    if (a[0] == '\0') {
      return true;
    }
    return Equals(a + 1, b + 1);
  }
  return false;
}

int main(int argc, char** argv) {
  BF_Init();
  char filename[512];
  strcpy(filename, "build/HT_hashFile");
  char attr_type = 'c';
  char attr_name[16];
  strcpy(attr_name, "city");
  int num_buckets = 127;
  bool create = true;
  for (int i = 1; i < argc; ++i) {
    if (Equals(argv[i], "o")) {
      ++i;
      strcpy(filename, argv[i]);
    } else if (Equals(argv[i], "type")) {
      ++i;
      sscanf(argv[i], "%c", &attr_type);
    } else if (Equals(argv[i], "name")) {
      ++i;
      sscanf(argv[i], "%s", attr_name);
    } else if (Equals(argv[i], "b")) {
      ++i;
      sscanf(argv[i], "%d", &num_buckets);
    } else if (Equals(argv[i], "nocreate")) {
      create = false;
    }
  }
  size_t attr_length = strlen(attr_name);

  if (create) {
#if defined(STATIC) || !defined(EXTENDIBLE)
    HT_CreateIndex(filename, attr_type, attr_name, attr_length, num_buckets);
#else
#endif
  }

#if defined(STATIC) || !defined(EXTENDIBLE)
  struct HT_info* hash = HT_OpenIndex(filename);
#else
#endif

  struct Record record;
  while (scanf("%d,\"%14[^\"]\",\"%19[^\"]\",\"%24[^\"]\"", &record.id,
      record.name, record.surname, record.city) == 4) {
#if defined(STATIC) || !defined(EXTENDIBLE)
    HT_InsertEntry(*hash, record);
#else
#endif
  }

  char* value = "Keratsini";
#if defined(STATIC) || !defined(EXTENDIBLE)
  HT_GetAllEntries(*hash, value);
#else
#endif

#if defined(STATIC) || !defined(EXTENDIBLE)
  HT_CloseIndex(hash);
#else
#endif
  return 0;
}
