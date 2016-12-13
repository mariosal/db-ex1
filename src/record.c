#include "record.h"
#include <stdlib.h>
#include <string.h>

struct Record {
  int id;
  char firstname[15];
  char lastname[20];
  char city[25];
};

void RecordInitialize(struct Record** record, int id, const char* firstname,
                      const char* lastname, const char* city) {
  *record = malloc(sizeof(struct Record));
  if (*record == NULL) {
    exit(EXIT_FAILURE);
  }
  (*record)->id = id;
  strncpy((*record)->firstname, firstname, 14);
  strncpy((*record)->lastname, lastname, 19);
  strncpy((*record)->city, city, 24);
  (*record)->firstname[14] = '\0';
  (*record)->lastname[19] = '\0';
  (*record)->city[24] = '\0';
}

void RecordReset(struct Record** record) {
  free(*record);
  *record = NULL;
}

int RecordPrint(const struct Record* record, FILE* out) {
  return fprintf(out, "%d %14s %19s %24s\n", record->id, record->firstname,
                 record->lastname, record->city);
}
