#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "F12.h"

// obat init_Obat_csv(){
//     obat ob;
//     ob.id_obat = 0;
//     strcpy(ob.nama_obat,"");
// }

void baca_obat(Sistem_pengobatan *sistem) {
    // Validate system pointer and array
    if (sistem == NULL || sistem->daftar_obat == NULL) {
        printf("Error: Sistem atau daftar obat tidak terinisialisasi!\n");
        return;
    }

    FILE *file = fopen(OBAT_CSV_PATH, "r");
    if (file == NULL) {
        printf("Gagal membuka file %s\n", OBAT_CSV_PATH);
        perror("Error");
        return;
    }

    char line[MAX_LINE];
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("File obat kosong atau header tidak valid\n");
        fclose(file);
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL && sistem->jumlah_obat < MAX_OBAT) {
        if (strlen(line) <= 1) continue;

        char fields[MAX_FIELDS_OBAT][MAX_FIELD_LEN] = {0};
        int field_index = 0;
        int char_index = 0;

        for (int i = 0; line[i] != '\0' && line[i] != '\n' && field_index < MAX_FIELDS_OBAT; i++) {
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
        if (field_index < 1) {
            printf("⚠️ Warning: Format baris tidak valid: %s", line);
            continue;
        }
        obat* Obat = &sistem->daftar_obat[sistem->jumlah_obat];
        char *endptr;
        Obat->id_obat = strtol(fields[0], &endptr, 10);
        if (*endptr != '\0') {
            printf("⚠️ Warning: ID obat tidak valid: %s\n", fields[0]);
            continue;
        }
        strncpy(Obat->nama_obat, fields[1], MAX_FIELD_LEN - 1);
        Obat->nama_obat[MAX_FIELD_LEN - 1] = '\0'; 
        sistem->jumlah_obat++;
    }
    fclose(file);
    //printf("Berhasil memuat %d data obat dari %s\n", sistem->jumlah_obat, OBAT_CSV_PATH);
}

void inisialisasi_sistem_pengobatan(Sistem_pengobatan *sistemObat) {
    sistemObat->daftar_penyakit = NULL;
    sistemObat->daftar_obat = NULL;
    sistemObat->relasi_obat_penyakit = NULL;
    sistemObat->daftar_penyakit = malloc(sizeof(Penyakit) * MAKS_PENYAKIT);
    sistemObat->daftar_obat = malloc(sizeof(obat) * MAX_OBAT); 
    sistemObat->relasi_obat_penyakit = malloc(sizeof(obat_penyakit) * MAKS_RELASI_OBAT_PENYAKIT);
    

    if (sistemObat->daftar_obat == NULL) {
        perror("Gagal mengalokasikan memori untuk daftar obat");
        free(sistemObat->daftar_penyakit);
        free(sistemObat->relasi_obat_penyakit);
        return;
    }
    
    if (sistemObat->daftar_penyakit == NULL) {
        perror("Gagal mengalokasikan memori untuk penyakit");
        free(sistemObat->daftar_obat);
        free(sistemObat->relasi_obat_penyakit);
        return;
    }
    
    if (sistemObat->relasi_obat_penyakit == NULL) {
        perror("Gagal mengalokasikan memori untuk relasi obat");
        free(sistemObat->daftar_obat);
        free(sistemObat->daftar_penyakit);
        return;
    }
    sistemObat->jumlah_penyakit = 0;
    sistemObat->jumlah_obat = 0;
    sistemObat->jumlah_relasi = 0;
}

obat *obat_dari_id(int id, Sistem_pengobatan* sistemobat) {
    for (int i = 0; i < sistemobat->jumlah_obat; i++) {
        if (sistemobat->daftar_obat[i].id_obat == id) {
            return &sistemobat->daftar_obat[i];
        }
    }
    return NULL; // Tidak ditemukan
}


Penyakit *penyakit_dari_id(int id,Daftar_Penyakit* daftarpenyakit){
    //Penyakit *p = make_Empty_Penyakit();
    for (int i=0;i<daftarpenyakit->count;i++){
        if (daftarpenyakit->penyakit[i].id == id){
            return &daftarpenyakit->penyakit[i];
        }
    }
    return NULL;
}
void Load_Obat_Penyakit_CSV(Sistem_pengobatan *sistemObat, Daftar_Penyakit *daftarpenyakit) {
    FILE* f = fopen(OBAT_PENYAKIT_CSV_PATH, "r");
    if (!f) {
        perror("Tidak dapat membuka file Obat-Penyakit CSV");
        return;
    }

    char line[MAX_LINE];
    fgets(line, sizeof(line), f); // Lewati header

    while (fgets(line, sizeof(line), f)) {
        if (sistemObat->jumlah_relasi >= MAKS_RELASI_OBAT_PENYAKIT) {
            printf("Peringatan: Jumlah relasi obat-penyakit melebihi kapasitas maksimal.\n");
            break;
        }

        // Parsing manual dengan delimiter ';'
        char fields[MAX_FIELDS_OBAT_PENYAKIT][MAX_FIELD_LEN] = {{0}};
        int field_index = 0, char_index = 0;

        for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
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

        // Konversi dan isi struct
        int id_obat = atoi(fields[0]);
        int id_penyakit = atoi(fields[1]);
        int urutan = atoi(fields[2]);

        // Dapatkan pointer ke entry relasi yang baru
        obat_penyakit* op = &sistemObat->relasi_obat_penyakit[sistemObat->jumlah_relasi];

        // Validasi & assign
        obat* o = obat_dari_id(id_obat, sistemObat);
        if (o == NULL) {
            printf("Peringatan: Obat dengan ID %d tidak ditemukan. Baris dilewati.\n", id_obat);
            continue;
        }
        Penyakit *p = penyakit_dari_id(id_penyakit, daftarpenyakit);
        if (p->id == MARK) {
            printf("Peringatan: Penyakit dengan ID %d tidak ditemukan. Baris dilewati.\n", id_penyakit);
            continue;
        }

        op->_obat = *o;
        op->penyakit = *p;
        op->urutan_minum = urutan;

        sistemObat->jumlah_relasi++;
    }

    fclose(f);
}


// void urutan_obat(int penyakit_id){
//     // int count=0;
//     // int list_obat[10];
//     // for(int i=0;i<10;i++){
//     //     //cocok pada i=5, urutan =2,urutan 1-> list_obat[0]=obat
//     //     if(obat[i].penyakit_id==penyakit_id){
//     //         int idx=obat[i].urutan_minum-1;
//     //         list_obat[idx]=obat[i].obat_id;
//     //         count++;
//     //     }
//     // }
//     // for(int i=0;i<count;i++){
//     //     printf("%d. %s",i+1,idToName(list_obat[i]));
//     // }
// }

// User* getUserById(int id, UserSet* userset) {
//     for (int i = 0; i < userset->count; i++) {
//         if (userset->users[i].id == id) {
//             return userset->users;
//         }
//     }
//     return NULL;
// }

void Ngobatin(UserSet* userset, Data_sistem* sistem, Sistem_pengobatan* sistem_pengobatan) {
    if (!sistem_pengobatan || !sistem_pengobatan->daftar_obat) {
        printf("Error: Sistem atau daftar obat tidak terinisialisasi!\n");
        return;
    }

    printf("\n>>>🩺 NGOBATIN\n");

    if (userset->users[userset->loggedInIndex].role != ROLE_DOKTER) {
        printf("🚫Hanya Dokter yang bisa mengakses fungsi Ngobatin\n");
        return;
    }

    Data_ruangan* ruangan = ruangan_dokter(userset->users[userset->loggedInIndex], sistem);
    if (!ruangan || !ruangan->antrian_pasien_dalam_ruangan || !ruangan->antrian_pasien_dalam_ruangan->depan) {
        printf("Tidak ada pasien dalam antrian!\n");
        return;
    }

    User* pasien_pertama = ruangan->antrian_pasien_dalam_ruangan->depan->data;
    if (!pasien_pertama || !pasien_pertama->penyakit) {
        printf("Pasien belum memiliki penyakit!\n");
        return;
    }

    if (!isInAnyQueue(sistem, pasien_pertama->id)) {
        printf("Pasien belum di diagnosis!\n");
        return;
    }

    printf("📦Obat yang harus diberikan:\n");

    boolean found = false;
    int urutan = 1;
    int id_obat_ditampilkan[20];
    int jumlah_id_ditampilkan = 0;

    for (int i = 0; i < sistem_pengobatan->jumlah_relasi; i++) {
        obat_penyakit* relasi = &sistem_pengobatan->relasi_obat_penyakit[i];

        if (relasi->penyakit.id != pasien_pertama->penyakit->id) continue;

        // Cek apakah ID obat sudah pernah ditampilkan
        boolean sudah_ada = false;
        for (int j = 0; j < jumlah_id_ditampilkan; j++) {
            if (id_obat_ditampilkan[j] == relasi->_obat.id_obat) {
                sudah_ada = true;
                break;
            }
        }

        // Cek apakah ID obat sudah ada di inventory
        for (int j = 0; j < pasien_pertama->jumlah_obat_inventory; j++) {
            if (pasien_pertama->inventory_obat[j].id_obat == relasi->_obat.id_obat) {
                sudah_ada = true;
                break;
            }
        }

        if (!sudah_ada && pasien_pertama->jumlah_obat_inventory < MAX_INVENTORY_OBAT) {
            // Simpan ID yang ditampilkan agar tidak dobel
            id_obat_ditampilkan[jumlah_id_ditampilkan++] = relasi->_obat.id_obat;

            // Tambahkan ke inventory pasien
            pasien_pertama->inventory_obat[pasien_pertama->jumlah_obat_inventory++] = relasi->_obat;

            // Tampilkan
            printf("%d. %s\n", urutan++, relasi->_obat.nama_obat);

            found = true;
            pasien_pertama->telah_Didiagnosis = true;
        }
    }

    if (!found) {
        printf("❌ Tidak ditemukan obat untuk penyakit 🦠 %s\n", pasien_pertama->penyakit->nama_penyakit);
    }
}