#ifndef F02_H
#define F02_H

#include <stdio.h>
#include "HeaderSemuaADT.h"


int CaseInsensitive(const char* s1, const char* s2);

void registerPasien(UserSet* userSet);

const char* roleToString(Role role);
void modify_user_csv(int line_number, const char *new_line) ;

void saveUserToCSV(User user, const char* filename, int id);

int getIdFromUsername(const char* username, const char* filepath);

#endif
