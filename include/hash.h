#ifndef HASH_H_
#define HASH_H_

#include "record.h"

struct HT_info {
  char attr_type;
  int file_desc;
  int num_buckets;
  size_t attr_length;
  char* attr_name;
};
typedef struct HT_info HT_info;

int HT_CreateIndex(const char* filename, char attr_type, const char* attr_name,
                   size_t attr_length, int num_buckets);

struct HT_info* HT_OpenIndex(const char* filename);
int HT_CloseIndex(struct HT_info* hash);

int HT_InsertEntry(struct HT_info hash, struct Record record);
int HT_GetAllEntries(struct HT_info hash, const void* value);
int HashStatistics(const char* filename);

#endif  // HASH_H_
