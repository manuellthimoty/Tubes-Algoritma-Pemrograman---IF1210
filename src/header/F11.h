#ifndef F11_H
#define F11_H
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "HeaderSemuaADT.h"
#include "F06.h"
#include "F02.h"
#include "F09.h"
#include "F14.h"
#include "Boolean.h"


boolean is_valid_patient(const User* user, const char* current_dokter);
Data_ruangan *ruangan_dokter(User dokter,Data_sistem *sistem);
void Diagnosis(Daftar_Penyakit daftar_penyakit, UserSet* User_Set, Data_sistem *sistem);

boolean lewat_batas_suhu(User user, Penyakit p);
boolean lewat_batas_tekanan_darah_sistolik(User user, Penyakit p);
boolean lewat_batas_tekanan_darah_diastolik(User User, Penyakit p);
boolean lewat_batas_detak_jantung(User User, Penyakit p);
boolean lewat_batas_saturasi_oksigen(User User, Penyakit p);
boolean lewat_batas_gula_darah(User User, Penyakit p);
boolean lewat_batas_berat_badan(User User, Penyakit p);
boolean lewat_batas_tinggi_badan(User User, Penyakit p);
boolean lewat_batas_kolesterol(User User, Penyakit p);
boolean lewat_batas_trombosit(User User, Penyakit p);
    
void loadPenyakit(Daftar_Penyakit* daftar_penyakit);
void updateUserInCSVDiagnosis(User* user);

#endif