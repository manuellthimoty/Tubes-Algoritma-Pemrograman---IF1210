#ifndef F10_H
#define F10_H


#include "HeaderSemuaADT.h"
#include "F01.h"
#include "F06.h"

void intToStr(int value, char* buffer);
void floatToStr(float value, char* buffer);
void saveAllUsersToCSV(UserSet* userSet, const char* filename) ;
int generateId(UserSet* userSet);
//int samaIgnoreCase(const char* a, const char* b) ;
void tambahDokter(UserSet* userSet);
void assignDokter(UserSet* userSet,Data_sistem*sistem);

#endif
