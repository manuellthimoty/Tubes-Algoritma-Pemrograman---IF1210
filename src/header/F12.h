#ifndef F12_H
#define F12_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Boolean.h"
#include "HeaderSemuaADT.h"
#include "F11.h"
#include "F09.h"
//#include "F14.h"  

void baca_obat(Sistem_pengobatan *sistem);
//char* idToName(int id);
//void urutan_obat(int penyakit_id);
void inisialisasi_sistem_pengobatan(Sistem_pengobatan *sistemObat);
obat *obat_dari_id(int id,Sistem_pengobatan* sistemobat);
Penyakit* penyakit_dari_id(int id,Daftar_Penyakit* daftarpenyakit);
void Load_Obat_Penyakit_CSV(Sistem_pengobatan *sistemObat,Daftar_Penyakit *daftarpenyakit);
void Ngobatin(UserSet* userset,Data_sistem *sistem,Sistem_pengobatan* sistem_pengobatan);

#endif