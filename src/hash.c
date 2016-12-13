#include "hash.h"
#include "BF.h"

int HT_CreateIndex(const char* filename, char attr_type, const char* attr_name,
                   size_t attr_length, int num_buckets) {

  int file_desc, i;
  void *block;
  if (BF_CreateFile(filename) < 0) {
    BF_PrintError("Error creating file");
    return -1;
  }
  file_desc = BF_OpenFile(filename);
  if (file_desc < 0) {
    BF_PrintError("Error opening file");
    return -1;
  }
  if (BF_AllocateBlock(file_desc) < 0) {
    BF_PrintError("Error allocating block");
    return -1;
  }

  if (BF_ReadBlock(file_desc, 0, &block) < 0 ) {
    BF_PrintError("Error reading block");
    return -1;
  }

  memcpy(block, &attr_type, sizeof(char));
  block = (char *) block + sizeof(char)

  memcpy(block, &file_desc, sizeof(int));
  block = (char *) (block) + sizeof(int)

  memcpy(block, &num_buckets, sizeof(int));
  block = (char *) (block) + sizeof(int)

  memcpy(block, &attr_length, sizeof(size_t));
  block = (char *) (block) + sizeof(size_t)

  memcpy(block, attr_name, (strlen(attr_name) + 1) * sizeof(char));
  block = (char *) (block) + strlen(attr_name) + 1;

  if (BF_WriteBlock(file_desc, 0) < 0) {
    BF_PrintError("Error writing block");
    return -1;
  }


  for(i = 0, i < num_buckets, ++i) {
    if (BF_AllocateBlock(file_desc) < 0) {
      BF_PrintError("Error allocating block");
      return -1;
    }
  }

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
