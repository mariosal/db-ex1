#ifndef RECORD_H_
#define RECORD_H_

#include <stdio.h>

struct Record;

void RecordInitialize(struct Record** record, int id, char* firstname,
                      char* lastname, char* city);
void RecordReset(struct Record** record);

int RecordPrint(const struct Record* record, FILE* out);

#endif  // RECORD_H_
