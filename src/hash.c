#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include "BF.h"

int HT_CreateIndex(const char* filename, char attr_type, const char* attr_name,
                   size_t attr_length, int num_buckets) {
  if (BF_CreateFile(filename) < 0) {
    BF_PrintError("Error creating file");
    return -1;
  }

  int file_desc = BF_OpenFile(filename);
  if (file_desc < 0) {
    BF_PrintError("Error opening file");
    return -1;
  }

  if (BF_AllocateBlock(file_desc) < 0) {
    BF_PrintError("Error allocating block");
    return -1;
  }

  void* block;
  if (BF_ReadBlock(file_desc, 0, &block) < 0) {
    BF_PrintError("Error reading block");
    return -1;
  }

  memcpy(block, &attr_type, sizeof(attr_type));
  block = (char*)block + sizeof(attr_type);

  memcpy(block, &file_desc, sizeof(file_desc));
  block = (char*)block + sizeof(file_desc);

  memcpy(block, &num_buckets, sizeof(num_buckets));
  block = (char*)block + sizeof(num_buckets);

  memcpy(block, &attr_length, sizeof(attr_length));
  block = (char*)block + sizeof(attr_length);

  memcpy(block, attr_name, sizeof(char) * (strlen(attr_name) + 1));
  block = (char*)block + sizeof(char) * (strlen(attr_name) + 1);

  if (BF_WriteBlock(file_desc, 0) < 0) {
    BF_PrintError("Error writing block");
    return -1;
  }

  for(int i = 1; i <= num_buckets; ++i) {
    if (BF_AllocateBlock(file_desc) < 0) {
      BF_PrintError("Error allocating block");
      return -1;
    }

    if (BF_ReadBlock(file_desc, i, &block) < 0) {
      BF_PrintError("Error reading block");
      return -1;
    }

    memcpy(block, &i, sizeof(i));
    block = (char*)block + sizeof(i);

    memcpy(block, &i, sizeof(i));
    block = (char*)block + sizeof(i);

    int free_space = BLOCK_SIZE - sizeof(i) * 2 - sizeof(int);
    memcpy(block, &free_space, sizeof(free_space));
    block = (char*)block + sizeof(free_space);

    if (BF_WriteBlock(file_desc, i) < 0) {
      BF_PrintError("Error writing block");
      return -1;
    }
  }
  return 0;
}

struct HT_info* HT_OpenIndex(const char* filename) {
  return NULL;
}

int HT_CloseIndex(struct HT_info* hash) {
  if (BF_CloseFile(hash->file_desc) < 0) {
    BF_PrintError("Error closing file");
    return -1;
  }
  free(hash->attr_name);
  free(hash);
  return 0;
}

int HT_InsertEntry(struct HT_info hash, struct Record record) {
  return 0;
}

int HT_GetAllEntries(struct HT_info hash, void* value) {
  return 0;
}
