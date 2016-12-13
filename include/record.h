#ifndef RECORD_H_
#define RECORD_H_

#include <stdio.h>

struct Record {
  int id;
  char name[15];
  char surname[20];
  char city[25];
};
typedef struct Record Record;

void RecordInitialize(struct Record* record, int id, const char* name,
                      const char* surname, const char* city);

int RecordPrint(const struct Record* record, FILE* out);

#endif  // RECORD_H_
