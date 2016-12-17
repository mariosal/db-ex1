#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include "BF.h"
// seira orismatwn
int ET_CreateIndex(const char* filename, const char* attr_name, char attr_type,
                   size_t attr_length, int depth) {
  return 0;
}

struct ET_info* ET_OpenIndex(const char* filename) {
  return NULL;
}

int ET_CloseIndex(struct ET_info* hash) {
  return 0;
}

int ET_InsertEntry(struct ET_info* hash, struct Record record) {
  return 0;
}

int ET_GetAllEntries(struct ET_info hash, const void* value) {
  return 0;
}

