#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include "BF.h"

static int HashFn(const struct HT_info* hash, const void* value) {
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

static void BlockSetTail(void* block, int tail) {
  memcpy(block, &tail, sizeof(int));
}

static int BlockTail(const void* block) {
  int tail;
  memcpy(&tail, block, sizeof(int));
  return tail;
}

static void BlockSetNext(void* block, int next) {
  block = (char*)block + sizeof(int);
  memcpy(block, &next, sizeof(int));
}

static int BlockNext(const void* block) {
  int next;
  block = (const char*)block + sizeof(int);
  memcpy(&next, block, sizeof(int));
  return next;
}

static void BlockSetFreeSpace(void* block, int free_space) {
  block = (char*)block + sizeof(int) * 2;
  memcpy(block, &free_space, sizeof(int));
}

static int BlockFreeSpace(const void* block) {
  int free_space;
  block = (const char*)block + sizeof(int) * 2;
  memcpy(&free_space, block, sizeof(int));
  return free_space;
}

static void BlockSetNumEntries(void* block, int num_entries) {
  block = (char*)block + sizeof(int) * 3;
  memcpy(block, &num_entries, sizeof(int));
}

static int BlockNumEntries(const void* block) {
  int num_entries;
  block = (const char*)block + sizeof(int) * 3;
  memcpy(&num_entries, block, sizeof(int));
  return num_entries;
}

static void BlockSetEntry(void* block, int offset, const struct Record* record) {
  block = (char*)block + sizeof(int) * 4 + sizeof(struct Record) * offset;
  memcpy(block, record, sizeof(struct Record));
}

static struct Record* BlockEntry(const void* block, int offset) {
  struct Record* record = malloc(sizeof(struct Record));
  block = (const char*)block + sizeof(int) * 4 + sizeof(struct Record) * offset;
  memcpy(record, block, sizeof(struct Record));
  return record;
}

static void BlockInitialize(void* block, int tail) {
  BlockSetTail(block, tail);
  BlockSetNext(block, -1);
  BlockSetFreeSpace(block, BLOCK_SIZE - sizeof(int) * 4);
  BlockSetNumEntries(block, 0);
}

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

    BlockInitialize(block, i);
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

  int tail_id = BlockTail(head);
  void* tail;
  if (BF_ReadBlock(hash.file_desc, tail_id, &tail) < 0) {
    BF_PrintError("Error reading block");
    return -1;
  }

  if (BlockFreeSpace(tail) < sizeof(struct Record)) {
    if (BF_AllocateBlock(hash.file_desc) < 0) {
      BF_PrintError("Error allocating block");
      return -1;
    }

    int newtail_id = BF_GetBlockCounter(hash.file_desc) - 1;
    if (newtail_id < 0) {
      BF_PrintError("Error counting blocks");
      return -1;
    }

    BlockSetTail(head, newtail_id);
    if (BF_WriteBlock(hash.file_desc, head_id) < 0) {
      BF_PrintError("Error writing block");
      return -1;
    }

    BlockSetNext(tail, newtail_id);
    if (BF_WriteBlock(hash.file_desc, tail_id) < 0) {
      BF_PrintError("Error writing block");
      return -1;
    }

    tail_id = newtail_id;
    if (BF_ReadBlock(hash.file_desc, tail_id, &tail) < 0) {
      BF_PrintError("Error reading block");
      return -1;
    }

    BlockInitialize(tail, tail_id);
  }

  BlockSetFreeSpace(tail, BlockFreeSpace(tail) - sizeof(struct Record));
  BlockSetEntry(tail, BlockNumEntries(tail), &record);
  BlockSetNumEntries(tail, BlockNumEntries(tail) + 1);
  if (BF_WriteBlock(hash.file_desc, tail_id) < 0) {
    BF_PrintError("Error writing block");
    return -1;
  }
  return 0;
}

int HT_GetAllEntries(struct HT_info hash, void* value) {
  return 0;
}
