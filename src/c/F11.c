#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "F11.h"

boolean is_valid_patient(const User* user, const char* current_dokter) {
    return user->id != 0 && 
           user->role == ROLE_PASIEN && 
           strcmp(user->assigned_dokter, current_dokter) == 0;
}
boolean is_urutan_pertama_ruangan(const User* user,Antrian* antrian_dalam_ruangan){
    return strcmp(antrian_dalam_ruangan->depan->data->username,user->username)==0;
}

Data_ruangan* ruangan_dokter(User dokter, Data_sistem* sistem) {
    if (sistem == NULL || dokter.ruangan[0] == '\0') {
        return NULL;
    }
    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        if (sistem->daftar_ruangan[i].dokter_di_ruangan == NULL) {
            continue;  // skip jika NULL, supaya gak segfault
        }
        if (strcmp(dokter.ruangan, sistem->daftar_ruangan[i].NamaRuangan) == 0 &&
            strcmp(dokter.username, sistem->daftar_ruangan[i].dokter_di_ruangan->username) == 0) {
            return &sistem->daftar_ruangan[i];
        }
    }
    return NULL;
}

void Diagnosis(Daftar_Penyakit daftar_penyakit, UserSet* User_Set, Data_sistem* sistem) {
    if (!User_Set || !sistem) {
        printf("❌ Error: Invalid parameters\n");
        return;
    }

    printf("\n>>> DIAGNOSIS\n");

    if (User_Set->loggedInIndex < 0 || User_Set->loggedInIndex >= User_Set->count) {
        printf("❌ Error: Invalid user session\n");
        return;
    }

    User* dokter = &User_Set->users[User_Set->loggedInIndex];
    if (dokter->role != ROLE_DOKTER) {
        printf("❌ Hanya dokter yang dapat melakukan diagnosis.\n");
        return;
    }

    Data_ruangan* ruangan = ruangan_dokter(*dokter, sistem);
    if (!ruangan){
        printf("❌ Ruangan error\n");
        return;
    }
    if (!ruangan->antrian_pasien_dalam_ruangan || !ruangan->antrian_pasien_dalam_ruangan->depan) {
        printf("Tidak ada pasien dalam antrian untuk didiagnosis.\n");
        return;
    }

    char* username_pasien = ruangan->antrian_pasien_dalam_ruangan->depan->data->username;
    User* target = getUser_by_username(username_pasien, User_Set);
    if (!target) {
        printf("Error: Pasien tidak ditemukan di user set.\n");
        return;
    }

    boolean ditemukan = false;
    for (int j = 0; j < daftar_penyakit.count; j++) {
        Penyakit* p = &daftar_penyakit.penyakit[j];
        if (p->id == 0) continue;

        if (lewat_batas_suhu(*target, *p) ||
            lewat_batas_tekanan_darah_sistolik(*target, *p) ||
            lewat_batas_tekanan_darah_diastolik(*target, *p) ||
            lewat_batas_detak_jantung(*target, *p) ||
            lewat_batas_saturasi_oksigen(*target, *p) ||
            lewat_batas_gula_darah(*target, *p) ||
            lewat_batas_berat_badan(*target, *p) ||
            lewat_batas_tinggi_badan(*target, *p) ||
            lewat_batas_kolesterol(*target, *p) ||
            lewat_batas_trombosit(*target, *p)) {

            printf("[🧾DIAGNOSIS]| suhu: %.2f | sistolik: %d | diastolik: %d | jantung: %d\n",
                   target->suhu_tubuh,
                   target->tekanan_darah_sistolik,
                   target->tekanan_darah_diastolik,
                   target->detak_jantung);
            printf("[🧾DIAGNOSIS]| saturasi: %.2f | gula: %d | berat: %.2f | tinggi: %d\n",
                   target->saturasi_oksigen,
                   target->kadar_gula_darah,
                   target->berat_badan,
                   target->tinggi_badan);
            printf("[🧾DIAGNOSIS]| kolesterol: %d | trombosit: %d\n",
                   target->kadar_kolesterol,
                   target->trombosit);

            printf("\n ✅Pasien %s Terdiagnosa Penyakit: 🦠[%s]\n", target->username, p->nama_penyakit);

            // if (target->penyakit == NULL) {
            //     target->penyakit = malloc(sizeof(Penyakit));
            //     if (target->penyakit == NULL) {
            //         perror("Gagal alokasi penyakit");
            //         return;
            //     }
            // }

            // memcpy(target->penyakit, p, sizeof(Penyakit));
            
            target->penyakit = p;
            target->penyakit->nama_penyakit[MAX_LINE_LENGTH - 1] = '\0';

            strncpy(target->riwayat_penyakit, p->nama_penyakit, sizeof(target->riwayat_penyakit) - 1);
            target->riwayat_penyakit[sizeof(target->riwayat_penyakit) - 1] = '\0';

            updateUserInCSVDiagnosis(target);
            ditemukan = true;
            break;
        }
    }

    if (!ditemukan) {
        printf("%s: Tidak terdiagnosa penyakit tertentu\n", target->username);
    }
}
boolean lewat_batas_suhu(User user, Penyakit p) {
    return user.suhu_tubuh < p.suhu_tubuh_min ||
           user.suhu_tubuh > p.suhu_tubuh_max;
}

boolean lewat_batas_tekanan_darah_sistolik(User user, Penyakit p) {
    return user.tekanan_darah_sistolik < p.tekanan_darah_sistolik_min || 
           user.tekanan_darah_sistolik > p.tekanan_darah_sistolik_max;
}

boolean lewat_batas_tekanan_darah_diastolik(User user, Penyakit p) {
    return user.tekanan_darah_diastolik < p.tekanan_darah_diastolik_min ||
           user.tekanan_darah_diastolik > p.tekanan_darah_diastolik_max;
}

boolean lewat_batas_detak_jantung(User user, Penyakit p) {
    return user.detak_jantung < p.detak_jantung_min || 
           user.detak_jantung > p.detak_jantung_max;
}

boolean lewat_batas_saturasi_oksigen(User user, Penyakit p) {
    return user.saturasi_oksigen < p.saturasi_oksigen_min || 
           user.saturasi_oksigen > p.saturasi_oksigen_max;
}

boolean lewat_batas_gula_darah(User user, Penyakit p) {
    return user.kadar_gula_darah < p.kadar_gula_darah_min || 
           user.kadar_gula_darah > p.kadar_gula_darah_max;
}

boolean lewat_batas_berat_badan(User user, Penyakit p) {
    return user.berat_badan < p.berat_badan_min || 
           user.berat_badan > p.berat_badan_max;
}

boolean lewat_batas_tinggi_badan(User user, Penyakit p) {
    return user.tinggi_badan < p.tinggi_badan_min || 
           user.tinggi_badan > p.tinggi_badan_max;
}

boolean lewat_batas_kolesterol(User user, Penyakit p) {
    return user.kadar_kolesterol < p.kadar_kolesterol_min ||
           user.kadar_kolesterol > p.kadar_kolesterol_max;
}

boolean lewat_batas_trombosit(User user, Penyakit p) {
    return user.trombosit < p.trombosit_min ||
           user.trombosit > p.trombosit_max;
}

void loadPenyakit(Daftar_Penyakit* daftar_penyakit) {
    if (daftar_penyakit == NULL) return;

    memset(daftar_penyakit, 0, sizeof(Daftar_Penyakit));

    FILE* file = fopen(PENYAKIT_CSV_PATH, "r");
    if (!file) {
        printf("Error: Gagal membuka file penyakit\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file) && daftar_penyakit->count < MAKS_PENYAKIT) {
        line[strcspn(line, "\n")] = '\0';

        char fields[MAX_FIELDS_PENYAKIT][MAX_FIELD_LEN] = {0};
        int field_index = 0, char_index = 0;

        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == ';') {
                fields[field_index][char_index] = '\0';
                field_index++;
                char_index = 0;
            } else {
                if (char_index < MAX_FIELD_LEN - 1) {
                    fields[field_index][char_index++] = line[i];
                }
            }
        }
        fields[field_index][char_index] = '\0';

        // if (field_index < 21) {
        //     printf("Data penyakit tidak lengkap pada baris ke-%d, dilewati.\n", daftar_penyakit->count + 1);
        //     continue;
        // }

        Penyakit* p = &daftar_penyakit->penyakit[daftar_penyakit->count];
        memset(p, 0, sizeof(Penyakit));

        p->id = atoi(fields[0]);
        strncpy(p->nama_penyakit, fields[1], MAX_LINE_LENGTH - 1);
        p->nama_penyakit[MAX_LINE_LENGTH - 1] = '\0';

        p->suhu_tubuh_min= atof(fields[2]);
        p->suhu_tubuh_max = atof(fields[3]);
        p->tekanan_darah_sistolik_min = atoi(fields[4]);
        p->tekanan_darah_sistolik_max = atoi(fields[5]);
        p->tekanan_darah_diastolik_min= atoi(fields[6]);
        p->tekanan_darah_diastolik_max= atoi(fields[7]);
        p->detak_jantung_min = atoi(fields[8]);
        p->detak_jantung_max= atoi(fields[9]);
        p->saturasi_oksigen_min= atof(fields[10]);
        p->saturasi_oksigen_max= atof(fields[11]);
        p->kadar_gula_darah_min = atof(fields[12]);
        p->kadar_gula_darah_max= atof(fields[13]);
        p->berat_badan_min= atof(fields[14]);
        p->berat_badan_max = atof(fields[15]);
        p->tinggi_badan_min  = atoi(fields[16]);
        p->tinggi_badan_max= atoi(fields[17]);
        p->kadar_kolesterol_min = atoi(fields[18]);
        p->kadar_kolesterol_max= atoi(fields[19]);
        p->trombosit_min= atoi(fields[20]);
        p->trombosit_max= atoi(fields[21]);

        daftar_penyakit->count++;
    }

    fclose(file);
}

void updateUserInCSVDiagnosis(User* user) {
    if (user == NULL) return;

    FILE* temp_file = fopen(TEMP_CSV_PATH, "w");
    FILE* original_file = fopen(USER_CSV_PATH, "r");
    
    if (!temp_file || !original_file) {
        if (temp_file) fclose(temp_file);
        if (original_file) fclose(original_file);
        printf("Error: Gagal membuka file user\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    bool updated = false;
    char id_str[16];
    sprintf(id_str, "%d", user->id);

    // Copy header
    if (fgets(line, sizeof(line), original_file)) {
        fputs(line, temp_file);
    }

    // Process each record
    while (fgets(line, sizeof(line), original_file)) {
        char* id_start = line;
        char* id_end = strchr(line, ';');
        if (id_end) {
            *id_end = '\0';
            if (strcmp(id_start, id_str) == 0) {
                // Write formatted output using multiple fputs and sprintf calls 
                char buffer[MAX_LINE_LENGTH];
                sprintf(buffer, "%d;%s;%s;%s;%s;%.1f;%d;%d;%d;%.1f;%d;%.1f;%d;%d;%d;%d\n",
                    user->id, user->username, user->password, roleToString(user->role),
                    user->riwayat_penyakit, user->suhu_tubuh, user->tekanan_darah_sistolik,
                    user->tekanan_darah_diastolik, user->detak_jantung, user->saturasi_oksigen,
                    user->kadar_gula_darah, user->berat_badan, user->tinggi_badan,
                    user->kadar_kolesterol, user->kadar_kolesterol_ldl, user->trombosit);
                fputs(buffer, temp_file);
                updated = true;
                continue;
            }
            *id_end = ';';
        }
        fputs(line, temp_file);
    }

    fclose(original_file);
    fclose(temp_file);

    // Replace original file if update was successful
    if (updated) {
        remove(USER_CSV_PATH);
        rename(TEMP_CSV_PATH, USER_CSV_PATH);
    } else {
        remove(TEMP_CSV_PATH);
    }
}