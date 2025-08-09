#ifndef F08_H
#define F08_H

#include "HeaderSemuaADT.h"
#include "F07.h" 

// Fungsi utama pencarian
void Cari_User(UserSet* userSet);
void CariPasien(UserSet* userSet);
void Cari_Dokter(UserSet* userSet);

// Fungsi bantu pencarian User
void Mencari_User_bedasarkan_ID(UserSet* userSet, int id_dicari);
void Mencari_User_bedasarkan_UserName(UserSet* userSet, char* username_dicari);

// Fungsi bantu pencarian Pasien
void Cari_Pasien_Bedasarkan_ID(UserSet* userset, int id);
void Cari_Pasien_Bedasarkan_Nama(UserSet* userset, char* nama);
void Cari_Pasien_Bedasarkan_Penyakit(UserSet* userset, char* penyakit);

// Fungsi bantu pencarian Dokter
void Cari_Dokter_Bedasarkan_ID(UserSet* userset, int id);
void Cari_Dokter_Bedasarkan_Nama(UserSet* userset, char* nama);
void Cari_Dokter_Bedasarkan_Aura(UserSet* userset, int aura);

// Fungsi utilitas internal (bisa dibuat static kalau tidak ingin diekspos keluar)
void sortUsers(UserSet* userset, int sortKey);
void cetakPasien(User u);
void cetakDokter(User u);

#endif
