#ifndef F04_H
#define F04_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Boolean.h"
#include "HeaderSemuaADT.h"
#include "F02.h"

Role convert_string_to_role(const char* str);
// const char* roleToString(Role role);
void Parse_File_user(char** nama, Role* role, int* count);
boolean nama_valid(char* _nama, char** _namavalid, int panjang);
Role get_role_by_name(char* nama, char** nama_valid, Role* role_valid, int panjang);
void lupa_password_input(User *a);
char* Lenght_Encoding(char* input);
void Buat_Password_Baru(User* a);

#endif
