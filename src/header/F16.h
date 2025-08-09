#ifndef F16_H
#define F16_H

#include "HeaderSemuaADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void geser_kanan_obat_statis(obat arr[], int* jumlah);
void geser_kiri_obat_statis(obat arr[], int* jumlah, int indeks_hapus);
char* getNamaObatById(Sistem_pengobatan* sistem, int id_obat);
void Minum_Obat( UserSet* userset,Sistem_pengobatan* sistemObat);

#endif