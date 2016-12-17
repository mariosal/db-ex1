#ifndef EXHASH_H_
#define EXHASH_H_

#include "record.h"

struct ET_info {
  char attr_type;
  int file_desc;
  int depth;
  size_t attr_length;
  char* attr_name;
};
typedef struct ET_info ET_info;

int ET_CreateIndex(const char* filename, const char* attr_name, char attr_type,
                   size_t attr_length, int num_buckets);

struct ET_info* ET_OpenIndex(const char* filename);
int ET_CloseIndex(struct ET_info* hash);

int ET_InsertEntry(struct ET_info* hash, struct Record record);
int ET_GetAllEntries(struct ET_info hash, const void* value);

#endif  // EXHASH_H_
