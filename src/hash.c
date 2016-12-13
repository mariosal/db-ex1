#include "hash.h"
#include "BF.h"

int HT_CreateIndex(const char* filename, char attr_type, const char* attr_name,
                   size_t attr_length, int num_buckets) {
  return 0;
}

struct HT_info* HT_OpenIndex(const char* filename) {
  return NULL;
}

int HT_CloseIndex(struct HT_info* hash) {
  return 0;
}

int HT_InsertEntry(struct HT_info hash, struct Record record) {
  return 0;
}

int HT_GetAllEntries(struct HT_info hash, void* value) {
  return 0;
}
