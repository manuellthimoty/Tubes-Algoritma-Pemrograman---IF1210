#ifndef F06_H
#define F06_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "HeaderSemuaADT.h"
#include "F09.h"
#include "F14.h"
#include "F13.h"

#define MAX_LINE_LENGTH 256
#define IDX_UNDEF -1
#define CAPACITY 100
#define MAX_LINE_LENGTH 256
// #define MAX_TOKENS 20 
#define MAX_DATA 64

typedef char* RUANGAN;
//extern char** koderuangan;
#define HEAD(q) ((q).depan->data)
#define TAIL(q) ((q).belakang->data)
#define IS_EMPTY(q) ((q).depan == NULL)

// char* koderuangan[BANYAK_RUANGAN] = {
//     "A1", "A2", "A3", "A4",
//     "B1", "B2", "B3", "B4",
//     "C1", "C2", "C3", "C4",
//     "D1", "D2", "D3", "D4"
// };
void generateKodeRuanganArray(Data_sistem* sistem);
int getRoleById(int id);
char* getUsernameById(int id, char usernames[][50], int size);
void loadUsernames(char usernames[][50], int max_users);
void lihat_denah(Data_sistem *sistem);
User getUserById(int id, UserSet* userset);
User* getUserByIdPointer(int id, UserSet* userset);
void Lihat_Detail_Denah(Data_sistem *d) ;
void load_ruangan_config(Data_sistem* sistem, UserSet* userset) ;
int getRuanganIndex(const char* kode,Data_sistem* sistem);
int getRuanganIndexbyUser(User user,Data_sistem* sistem);
#endif


