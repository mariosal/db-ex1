#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include "BF.h"

int ET_CreateIndex(const char* filename, const char* attr_name, char attr_type,
                   size_t attr_length, int depth) {
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

  memcpy(block, &depth, sizeof(num_buckets));
  block = (char*)block + sizeof(num_buckets);

  memcpy(block, &attr_length, sizeof(attr_length));
  block = (char*)block + sizeof(attr_length);

  memcpy(block, attr_name, sizeof(char) * (attr_length + 1));
  block = (char*)block + sizeof(char) * (attr_length + 1);

  if (BF_WriteBlock(file_desc, 0) < 0) {
    BF_PrintError("Error writing block");
    return -1;
  }
  int num_buckets = 2;
  for (int i = 0; i < depth; ++i) {
    num_buckets *= 2;
  }

  if (BF_AllocateBlock(file_desc) < 0) {
    BF_PrintError("Error allocating block");
    return -1;
  }

  if (BF_ReadBlock(file_desc, 1, &block) < 0) {
    BF_PrintError("Error reading block");
    return -1;
  }
  // int index_blocks = (num_buckets / (BLOCK_SIZE / sizeof(int))) + 1
  int index_blocks = 1;
  int bytes = 0;
  for (int i = 1; i <= num_buckets; ++i) {
    int p = index_blocks + i; // deixnei sto block
    bytes += sizeof(p);
    if (bytes > BLOCK_SIZE) {
      if (BF_AllocateBlock(file_desc) < 0) {
        BF_PrintError("Error allocating block");
        return -1;
      }
      if (BF_ReadBlock(file_desc, ++index_blocks, &block) < 0) {
        BF_PrintError("Error reading block");
        return -1;
      }
      bytes = sizeof(p);
    }
    memcpy(block, &p, sizeof(i));
    block = (char*)block + sizeof(p);
  }

  for (int i = 1; i <= num_buckets; ++i) {
    if (BF_AllocateBlock(file_desc) < 0) {
      BF_PrintError("Error allocating block");
      return -1;
    }

    if (BF_ReadBlock(file_desc, index_blocks + i, &block) < 0) {
      BF_PrintError("Error reading block");
      return -1;
    }
    int local_depth = depth;
    memcpy(block, &local_depth, sizeof(local_depth));
    block = (char*)block + sizeof(local_depth);

    int num_entries = 0;
    memcpy =(block, &num_entries, sizeof(num_entries));
    block = (char*)block + sizeof(num_entries);

    int free_space = BLOCK_SIZE - sizeof(int) * 3  ;
    memcpy(block, &free_space, sizeof(free_space));

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

struct ET_info* ET_OpenIndex(const char* filename) {
  struct ET_info* hash = malloc(sizeof(struct ET_info));
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

  memcpy(&hash->depth, block, sizeof(hash->depth));
  block = (char*)block + sizeof(hash->num_buckets);

  memcpy(&hash->attr_length, block, sizeof(hash->attr_length));
  block = (char*)block + sizeof(hash->attr_length);

  hash->attr_name = malloc(sizeof(char) * (hash->attr_length + 1));
  memcpy(hash->attr_name, block, sizeof(char) * (hash->attr_length + 1));

  return hash;
}

int ET_CloseIndex(struct ET_info* hash) {
  if (BF_CloseFile(hash->file_desc) < 0) {
    BF_PrintError("Error closing file");
    return -1;
  }
  free(hash->attr_name);
  free(hash);
  return 0;
}

int ET_InsertEntry(struct ET_info* hash, struct Record record) {
  return 0;
}

int ET_GetAllEntries(struct ET_info hash, const void* value) {
  return 0;
}

