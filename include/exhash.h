#ifndef EXHASH_H_
#define EXHASH_H_

#include "record.h"

struct EH_info {
  char attr_type;
  int file_desc;
  int depth;
  size_t attr_length;
  char* attr_name;
};
typedef struct EH_info EH_info;

int EH_CreateIndex(const char* filename, const char* attr_name, char attr_type,
                   size_t attr_length, int num_buckets);

struct EH_info* EH_OpenIndex(const char* filename);
int EH_CloseIndex(struct EH_info* hash);

int EH_InsertEntry(struct EH_info* hash, struct Record record);
int EH_GetAllEntries(struct EH_info hash, const void* value);
int HashStatistics(const char* filename);

#endif  // EXHASH_H_
