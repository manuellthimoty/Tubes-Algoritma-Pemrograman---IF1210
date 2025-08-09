#ifndef F14_H
#define F14_H
#include "HeaderSemuaADT.h"
#include "F01.h"
#include "F02.h"
#include "Boolean.h"

#include "F09.h"
void inisialisasiMap(Data_sistem* sistem) ;
void createMap(Map* m) ;
void insertMap(Map* m, int key, int value);
boolean isInAnyQueue(Data_sistem* sistem, int idPasien) ;
int getValueMap(Map* m, int key);
int isKeyInMap(Map* m, int key);
void listAvailableDoctors(Data_sistem* sistem, int* valid_indices, int* count) ;
boolean isInAntrianCheckUp(Data_ruangan* dataruangan, int idPasien);
void antrianDaftarCheckUp(UserSet* userSet, Data_sistem* data_sistem);
void updateUserInCSV(User* user);
void saveConfig(Data_sistem* sistem, const char* filename, UserSet* userset);

// typedef struct daftarCheckUp {
//     int idPasien;
//     struct daftarCheckUp* next;
// } daftarCheckUp;

// typedef struct {
//     User* depan; //elemen pertama dalam antrian
//     User* belakang; //elemen terakhir dalam antrian
//     int panjang;
// } antrianCheckUp;

// typedef struct {
//     int dokterId;
//     char ruangan[2];
//     antrianCheckUp antrian;
// } entriAntrianDokter;


// void initAntrianCheckUp(antrianCheckUp* antrian);
// bool isInAntrianCheckUp(antrianCheckUp* antrian, int idPasien);
// void enqueueCheckUp(antrianCheckUp* antrian, int idPasien);
// int getPosisiAntrianCheckUp(antrianCheckUp* antrian, int idPasien);
// void antrianDaftarCheckUp(UserSet* userSet, entriAntrianDokter antrianDokter[], int* jumlahDokter, Data_ruangan* ruangan);
// void tampilkanAntrianCheckUp(UserSet* userSet, entriAntrianDokter antrianDokter[], int jumlahDokter);

#endif