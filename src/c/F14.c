#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "F14.h"
// void initAntrianCheckUp(antrianCheckUp* antrian) {
//     antrian->depan = antrian->belakang = NULL;
//     antrian->panjang = 0;
void saveConfig(Data_sistem* sistem, const char* filename, UserSet* userset) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Gagal membuka file config");
        return;
    }

    char buffer[MAX_LINE_LENGTH];

    // 1. Ukuran denah
    sprintf(buffer, "%d %d\n", sistem->panjang_ruangan, sistem->lebar_ruangan);
    fputs(buffer, file);

    // 2. Kapasitas ruangan dan antrian
    if (sistem->jumlah_ruangan > 0) {
        sprintf(buffer, "%d %d\n",
            sistem->daftar_ruangan[0].kapasitas,
            sistem->daftar_ruangan[0].kapasitas_antrian);
    } else {
        sprintf(buffer, "0 0\n");
    }
    fputs(buffer, file);

    // 3. Data tiap ruangan
    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        Data_ruangan* r = &sistem->daftar_ruangan[i];

        int dokter_id = 0;
        if (r->dokter_di_ruangan != NULL) {
            dokter_id = r->dokter_di_ruangan->id;
        }

        boolean ada_antrian = (r->antrian_pasien != NULL && r->antrian_pasien->panjang > 0);
        boolean ada_pasien = (r->jumlah_pasien_aktif > 0);

        if (dokter_id == 0 && !ada_pasien && !ada_antrian) {
            fputs("0\n", file);
            continue;
        }

        int offset = 0;
        offset += sprintf(buffer + offset, "%d", dokter_id);
        for (int j = 0; j < r->jumlah_pasien_aktif; j++) {
            offset += sprintf(buffer + offset, " %d", r->pasien_aktif[j].id);
        }
        if (ada_antrian) {
            NodeUser* curr = r->antrian_pasien->depan;
            while (curr != NULL) {
                if (curr->data != NULL) {
                    boolean already_in_room = false;
                    for (int k = 0; k < r->jumlah_pasien_aktif; k++) {
                        if (r->pasien_aktif[k].id == curr->data->id) {
                            already_in_room = true;
                            break;
                        }
                    }
                    if (!already_in_room) {
                        offset += sprintf(buffer + offset, " %d", curr->data->id);
                    }
                }
                curr = curr->next;
            }
        }

        offset += sprintf(buffer + offset, "\n");
        fputs(buffer, file);
    }

    // 4. Pasien dengan inventory obat
    int count_inventory = 0;
    for (int i = 0; i < userset->count; i++) {
        if (userset->users[i].jumlah_obat_inventory > 0) {
            count_inventory++;
        }
    }
    sprintf(buffer, "%d\n", count_inventory);
    fputs(buffer, file);

    for (int i = 0; i < userset->count; i++) {
        User* u = &userset->users[i];
        if (u->jumlah_obat_inventory > 0) {
            int offset = 0;
            offset += sprintf(buffer + offset, "%d", u->id);
            for (int j = 0; j < u->jumlah_obat_inventory; j++) {
                offset += sprintf(buffer + offset, " %d", u->inventory_obat[j].id_obat);
            }
            offset += sprintf(buffer + offset, "\n");
            fputs(buffer, file);
        }
    }

    // 5. Pasien dengan obat diminum
    int count_perut = 0;
    for (int i = 0; i < userset->count; i++) {
        if (userset->users[i].jumlah_obat_yang_diminum > 0) {
            count_perut++;
        }
    }
    sprintf(buffer, "%d\n", count_perut);
    fputs(buffer, file);

    for (int i = 0; i < userset->count; i++) {
        User* u = &userset->users[i];
        if (u->jumlah_obat_yang_diminum > 0) {
            int offset = 0;
            offset += sprintf(buffer + offset, "%d", u->id);
            for (int j = 0; j < u->jumlah_obat_yang_diminum; j++) {
                offset += sprintf(buffer + offset, " %d", u->obat_yg_diminum[j].id_obat);
            }
            offset += sprintf(buffer + offset, "\n");
            fputs(buffer, file);
        }
    }

    fclose(file);
}

void inisialisasiMap(Data_sistem* sistem) {
    sistem->dokter_ke_idx_ruangan.size = 0;

    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        User* dokter = sistem->daftar_ruangan[i].dokter_di_ruangan;
        if (dokter != NULL && dokter->id != 0) {
            sistem->dokter_ke_idx_ruangan.data[sistem->dokter_ke_idx_ruangan.size].key = dokter->id;
            sistem->dokter_ke_idx_ruangan.data[sistem->dokter_ke_idx_ruangan.size].value = i;
            sistem->dokter_ke_idx_ruangan.size++;
        }
    }
}

void createMap(Map* m) {
    m->size = 0;
}

void insertMap(Map* m, int key, int value) {
    for (int i = 0; i < m->size; i++) {
        if (m->data[i].key == key) {
            m->data[i].value = value;
            return;
        }
    }
    if (m->size < MAX_MAP_SIZE) {
        m->data[m->size].key = key;
        m->data[m->size].value = value;
        m->size++;
    }
}

int getValueMap(Map* m, int key) {
    for (int i = 0; i < m->size; i++) {
        if (m->data[i].key == key) {
            return m->data[i].value;
        }
    }
    return -1; 
}

int isKeyInMap(Map* m, int key) {
    for (int i = 0; i < m->size; i++) {
        if (m->data[i].key == key) {
            return 1;
        }
    }
    return 0;
}

boolean isInAntrianCheckUp(Data_ruangan* dataruangan, int idPasien) {
    for (int i=0;i<BANYAK_RUANGAN;i++){
        NodeUser* temp=dataruangan[i].antrian_pasien->depan;
        while (temp) {
        if(temp->data->id == idPasien) return true;
        temp = temp->next;
        }
    }
    return false;
}
boolean isInAnyQueue(Data_sistem* sistem, int idPasien) {
    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        NodeUser* current_antrian_luar_ruangan = sistem->daftar_ruangan[i].antrian_pasien->depan;
         while (current_antrian_luar_ruangan != NULL) {
            if (current_antrian_luar_ruangan->data->id== idPasien) {
                return true;
            }
            current_antrian_luar_ruangan = current_antrian_luar_ruangan->next;
        }
        NodeUser* current_antrian_dalam_ruangan = sistem->daftar_ruangan[i].antrian_pasien_dalam_ruangan->depan;
        while (current_antrian_dalam_ruangan!=NULL) {
            if (current_antrian_dalam_ruangan->data->id == idPasien) {
                return true;
            }
            current_antrian_dalam_ruangan = current_antrian_dalam_ruangan->next;
        }
    }
    return false;
}

void listAvailableDoctors(Data_sistem* sistem, int* valid_indices, int* count) {
    *count = 0;
    printf("\nBerikut adalah daftar dokter yang tersedia:\n");

    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        User* dokter = sistem->daftar_ruangan[i].dokter_di_ruangan;
        if (dokter != NULL && dokter->id != 0 && dokter->role == ROLE_DOKTER && strlen(dokter->username) > 0) {
            printf("%d. Dr. %s - Ruangan %s (Antrian: %d orang) - Aura %d\n",
                  *count + 1,
                  dokter->username,
                  sistem->daftar_ruangan[i].NamaRuangan,
                  sistem->daftar_ruangan[i].antrian_pasien->panjang,
                  dokter->aura);

            valid_indices[*count] = i;
            (*count)++;
        }
    }

    if (*count == 0) {
        printf("Tidak ada dokter yang tersedia saat ini.\n");
    }
}


// [Previous functions remain the same...]

void antrianDaftarCheckUp(UserSet* userSet, Data_sistem* sistem) {
    // Validate patient login
    if (userSet->loggedInIndex == -1 || userSet->users[userSet->loggedInIndex].role != ROLE_PASIEN) {
        printf("Fitur ini hanya dapat digunakan oleh pasien.\n");
        return;
    }
    if (isInAnyQueue(sistem,userSet->users[userSet->loggedInIndex].id)){
        printf("Anda Sudah Di Checkup\n");
        return;
    }

    User* pasien = &userSet->users[userSet->loggedInIndex];
    printf(">>> 📅 DAFTAR CHECKUP\n");
    printf("Silakan masukkan data Check-up Anda:\n");
    printf("Suhu Tubuh (Celcius): ");
    scanf("%f", &pasien->suhu_tubuh);
    if(pasien->suhu_tubuh <= 0) {
        printf("Suhu tubuh harus berupa angka positif!\n");
        return;
    }

    printf("Tekanan Darah (sistol/diastol, contoh 120 80): ");
    scanf("%d %d", &pasien->tekanan_darah_sistolik, &pasien->tekanan_darah_diastolik);
    if(pasien->tekanan_darah_sistolik <= 0 || pasien->tekanan_darah_diastolik <= 0) {
        printf("Tekanan darah harus berupa angka positif!\n");
        return;
    }

    printf("Detak Jantung (bpm): ");
    scanf("%d", &pasien->detak_jantung);
    
    printf("Saturasi Oksigen (%%): ");
    scanf("%f", &pasien->saturasi_oksigen);

    printf("Kadar Gula Darah (mg/dL): ");
    scanf("%d", &pasien->kadar_gula_darah);

    printf("Berat Badan (kg): ");
    scanf("%f", &pasien->berat_badan);

    printf("Tinggi Badan (cm): ");
    scanf("%d", &pasien->tinggi_badan);

    printf("Kadar Kolesterol (mg/dL): ");
    scanf("%d", &pasien->kadar_kolesterol);

    printf("Kadar Kolesterol LDL (mg/dL): ");
    scanf("%d", &pasien->kadar_kolesterol_ldl);

    printf("Trombosit (ribu/uL): ");
    scanf("%d", &pasien->trombosit);
    char buffer[512];
    User user = *pasien;

    
    // Check if already in queue
    if (isInAnyQueue(sistem, pasien->id)) {
        printf("✅🧾 Anda sudah terdaftar dalam antrian check-up!\n");
        return;
    }
    snprintf(buffer, sizeof(buffer), "%d;%s;%s;%s;%s;%f;%d;%d;%d;%f;%d;%f;%d;%d;%d;%d\n",
            user.id,
            user.username,
            user.password,
            roleToString(user.role),
            user.riwayat_penyakit,
            user.suhu_tubuh,
            user.tekanan_darah_sistolik,
            user.tekanan_darah_diastolik,
            user.detak_jantung,
            user.saturasi_oksigen,
            user.kadar_gula_darah,
            user.berat_badan,
            user.tinggi_badan,
            user.kadar_kolesterol,
            user.kadar_kolesterol_ldl,
            user.trombosit
        );
    modify_user_csv(user.id,buffer);
    int valid_room_indices[BANYAK_RUANGAN];
    int jumlahDokter;
    listAvailableDoctors(sistem, valid_room_indices, &jumlahDokter);

    if (jumlahDokter == 0) {
        printf("Tidak ada dokter yang tersedia saat ini.\n");
        return;
    }

    // Doctor selection
    int pilihan;
    printf("\n Pilih dokter (1-%d): ", jumlahDokter);
    if (scanf("%d", &pilihan) != 1 || pilihan < 1 || pilihan > jumlahDokter) {
        printf("🚫 Pilihan tidak valid!\n");
        return;
    }

    int ruangan_idx = valid_room_indices[pilihan-1];
    Data_ruangan* ruangan = &sistem->daftar_ruangan[ruangan_idx];
    User* dokter = ruangan->dokter_di_ruangan;

    // Add to queue
    enqueue(ruangan->antrian_pasien, pasien);
    
    // Update patient's doctor assignment
    strncpy(pasien->assigned_dokter, dokter->username, sizeof(pasien->assigned_dokter));
    strncpy(pasien->ruangan, ruangan->NamaRuangan, sizeof(pasien->ruangan));

    // Calculate queue position
    int posisi = 0;
    NodeUser* current = ruangan->antrian_pasien->depan;
    while (current != NULL && current->data->id != pasien->id) {
        posisi++;
        current = current->next;
    }

    printf("\n✅ Pendaftaran check-up berhasil!\n");
    printf("📅 Anda terdaftar pada antrian Dr. %s di ruangan %s.\n", 
           dokter->username, ruangan->NamaRuangan);
    printf("⏳ Posisi antrian Anda: %d\n", posisi + 1);

    // Save changes
    saveConfig(sistem, CONFIG_PATH,userSet);
    updateUserInCSV(pasien);  // Implement this to update user.csv
}
void updateUserInCSV(User* user) {
    char line[512];
    snprintf(line, sizeof(line), "%d;%s;%s;%s;%s;%.1f;%d;%d;%d;%.1f;%d;%.1f;%d;%d;%d;%d;%s;%s\n",
            user->id, user->username, user->password, roleToString(user->role),
            user->riwayat_penyakit, user->suhu_tubuh, user->tekanan_darah_sistolik,
            user->tekanan_darah_diastolik, user->detak_jantung, user->saturasi_oksigen,
            user->kadar_gula_darah, user->berat_badan, user->tinggi_badan,
            user->kadar_kolesterol, user->kadar_kolesterol_ldl, user->trombosit,
            user->assigned_dokter, user->ruangan);
    
    modify_user_csv(user->id+1, line);
}

// void tampilkanAntrianCheckUp(UserSet* userSet, entriAntrianDokter antrianDokter[], int jumlahDokter) {
//     printf("\n=== DAFTAR ANTRIAN SAAT INI ===\n");
//     for (int i = 0; i < jumlahDokter; i++) {
//         printf("Dokter ID %d (Ruangan %s): ", antrianDokter[i].dokterId,antrianDokter[i].ruangan);
//         daftarCheckUp* temp = antrianDokter[i].antrian.depan;
//         while (temp) {
//             printf("[%d] ", temp->idPasien);
//             temp = temp->next;
//         }
//         printf("\n");
//     }
// }