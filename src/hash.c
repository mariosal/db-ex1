#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include "BF.h"

int HashFn(const struct HT_info* hash, const void* value);

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

  memcpy(block, attr_name, sizeof(char) * (attr_length + 1));
  block = (char*)block + sizeof(char) * (attr_length + 1);

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

    memcpy(block, &i, sizeof(int));  // Tail
    block = (char*)block + sizeof(int);

    memcpy(block, &i, sizeof(int));  // Next
    block = (char*)block + sizeof(int);

    int free_space = BLOCK_SIZE - sizeof(int) * 4;
    memcpy(block, &free_space, sizeof(int));
    block = (char*)block + sizeof(int);

    int num_entries = 0;
    memcpy(block, &num_entries, sizeof(int));

    if (BF_WriteBlock(file_desc, i) < 0) {
      BF_PrintError("Error writing block");
      return -1;
    }
  }

  if (BF_CloseFile(file_desc) < 0) {
    BF_PrintError("Error closing file");
    return -1;
  }
  return 0;
}

struct HT_info* HT_OpenIndex(const char* filename) {
  struct HT_info* hash = malloc(sizeof(struct HT_info));
  if (hash == NULL) {
    exit(EXIT_FAILURE);
  }

  hash->file_desc = BF_OpenFile(filename);
  if (hash->file_desc < 0) {
    BF_PrintError("Error opening file");
    return NULL;
  }

  void* block;
  if (BF_ReadBlock(hash->file_desc, 0, &block) < 0) {
    BF_PrintError("Error reading block");
    return NULL;
  }

  memcpy(&hash->attr_type, block, sizeof(hash->attr_type));
  block = (char*)block + sizeof(hash->attr_type);

  memcpy(&hash->file_desc, block, sizeof(hash->file_desc));
  block = (char*)block + sizeof(hash->file_desc);

  memcpy(&hash->num_buckets, block, sizeof(hash->num_buckets));
  block = (char*)block + sizeof(hash->num_buckets);

  memcpy(&hash->attr_length, block, sizeof(hash->attr_length));
  block = (char*)block + sizeof(hash->attr_length);

  hash->attr_name = malloc(sizeof(char) * (hash->attr_length + 1));
  memcpy(hash->attr_name, block, sizeof(char) * (hash->attr_length + 1));
  return hash;
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
  int head_id;
  if (!strcmp(hash.attr_name, "id")) {
    head_id = HashFn(&hash, &record.id) + 1;
  } else if (!strcmp(hash.attr_name, "name")) {
    head_id = HashFn(&hash, record.name) + 1;
  } else if (!strcmp(hash.attr_name, "surname")) {
    head_id = HashFn(&hash, record.surname) + 1;
  } else {
    head_id = HashFn(&hash, record.city) + 1;
  }

  void* head;
  if (BF_ReadBlock(hash.file_desc, head_id, &head) < 0) {
    BF_PrintError("Error reading block");
    return -1;
  }

  int tail_id;
  memcpy(&tail_id, head, sizeof(int));

  void* tail;
  if (BF_ReadBlock(hash.file_desc, tail_id, &tail) < 0) {
    BF_PrintError("Error reading block");
    return -1;
  }

  int free_space;
  tail = (char*)tail + sizeof(int) * 2;
  memcpy(&free_space, tail, sizeof(free_space));
  if (free_space < sizeof(record)) {
    if (BF_AllocateBlock(hash.file_desc) < 0) {
      BF_PrintError("Error allocating block");
      return -1;
    }

    int newtail_id = BF_GetBlockCounter(hash.file_desc);
    if (newtail_id < 0) {
      BF_PrintError("Error counting blocks");
      return -1;
    }

    memcpy(head, &newtail_id, sizeof(newtail_id));
    if (BF_WriteBlock(hash.file_desc, head_id) < 0) {
      BF_PrintError("Error writing block");
      return -1;
    }

    tail = (char*)tail - sizeof(int);
    memcpy(tail, &newtail_id, sizeof(newtail_id));
    if (BF_WriteBlock(hash.file_desc, tail_id) < 0) {
      BF_PrintError("Error writing block");
      return -1;
    }

    tail_id = newtail_id;
    if (BF_ReadBlock(hash.file_desc, tail_id, &tail) < 0) {
      BF_PrintError("Error reading block");
      return -1;
    }

    memcpy(tail, &tail_id, sizeof(int));  // Tail
    tail = (char*)tail + sizeof(int);

    memcpy(tail, &tail_id, sizeof(int));  // Next
    tail = (char*)tail + sizeof(int);

    free_space = BLOCK_SIZE - sizeof(int) * 4;
    memcpy(tail, &free_space, sizeof(int));
    tail = (char*)tail + sizeof(int);

    int num_entries = 0;
    memcpy(tail, &num_entries, sizeof(int));
    tail = (char*)tail - sizeof(int);
  }

  free_space -= sizeof(record);
  memcpy(tail, &free_space, sizeof(int));
  tail = (char*)tail + sizeof(int);

  int num_entries;
  memcpy(&num_entries, tail, sizeof(int));
  ++num_entries;
  memcpy(tail, &num_entries, sizeof(int));

  tail = (char*)tail + sizeof(int) + sizeof(record) * (num_entries - 1);
  memcpy(tail, &record, sizeof(record));

  if (BF_WriteBlock(hash.file_desc, tail_id) < 0) {
    BF_PrintError("Error writing block");
    return -1;
  }
  return 0;
}

int HT_GetAllEntries(struct HT_info hash, void* value) {
  return 0;
}

int HashFn(const struct HT_info* hash, const void* value) {
  if (hash->attr_type == 'c') {
    int sum = 0;
    size_t len = strlen(value);
    for (size_t i = 0; i < len; ++i) {
      sum += ((const char*)value)[i];
    }
    return sum % hash->num_buckets;
  }
  return *(const int*)value % hash->num_buckets;
}
