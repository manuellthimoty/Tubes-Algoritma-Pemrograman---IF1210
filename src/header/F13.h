#ifndef F13_H
#define F13_H
#include "F11.h"
#include "Boolean.h"
#include "F09.h"
#include "F10.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap_obat_penyakit(obat_penyakit *a, obat_penyakit *b);
void sort_obat_penyakit_by_urutan(obat_penyakit arr[], int jumlah);

void tampilkan_perbandingan_urutan(User* pasien, Sistem_pengobatan* sistem_pengobatan);
int cari_urutan_minum(int id_obat, int id_penyakit, Sistem_pengobatan* sistem);
void hapusPasienDariRuangan(Data_ruangan* ruangan, int pasien_id) ;
boolean isAntrianKosong(Antrian* antrian);
boolean cek_urutan_minum(User* pasien, Sistem_pengobatan* sistem);
boolean isInPasienAktif(Data_ruangan* ruangan, int idPasien);
void boleh_pulang_gak_dok(UserSet* userset, Sistem_pengobatan* sistem, Data_sistem* data_sistem);
#endif
