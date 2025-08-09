#ifndef F07_H
#define F07_H

#include "HeaderSemuaADT.h"
#include "F02.h"

#include <stdio.h>
#include <string.h>
#include "F07.h"
typedef enum {SORT_ID=1,SORT_NAME=2,SORT_PENYAKIT_AURA=3}Short_KEY;
typedef enum {SORT_PASIEN=1,SORT_DOKTER}Short_KEY_Role;
//pengguna daftarPengguna[MAKS_PENGGUNA];
//int jumlahPengguna = 0;
int compareUserById(const void* a, const void* b) ;
int compareUserByUsername(const void* a, const void* b);
void TampilkanUser_Spesifik(UserSet*userSet,int sort,Short_KEY key,Short_KEY_Role role);
void TampilkanUser(UserSet*userSet,int sort,Short_KEY key);
void lihatUser(UserSet *userSet);

#endif