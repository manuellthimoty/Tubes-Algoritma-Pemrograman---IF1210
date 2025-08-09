#ifndef F09_H
#define F09_H
#include "HeaderSemuaADT.h"
#include "Boolean.h"

User* makeEmptyUser();
Penyakit *make_Empty_Penyakit();
obat* make_Empty_Obat();
void CreateAntrian(Antrian *q);
boolean isEmpty(Antrian q);
//boolean isFull(Antrian q);
int length(Antrian *q);
void enqueue(Antrian *q, User *val) ;
void dequeue(Antrian *q, User *val) ;
// void TambahAntrian(Antrian *q, User val);
// void HapusAntrian(Antrian *q, User *val);
void initDataRuangan(Data_sistem* sistem, int jumlah);
User* getUser_by_username(char*name,UserSet* userset);
// int get_User_idx(UserSet* userset, User user);
// int find_no_ruangan(int panjang_data,char*koderuangan_src ,char* koderuangan_des);
void lihat_semua_antrian(Data_ruangan* data_ruangan,Data_sistem *sistem)  ;

#endif
