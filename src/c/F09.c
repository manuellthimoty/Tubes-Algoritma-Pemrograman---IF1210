// Fungsi lihat semua antrian (print semua ruangan dan isi antriannya)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "F09.h"

// extern char koderuangan[][16];


User* makeEmptyUser() {
    User* u = (User*) malloc(sizeof(User));
    if (u == NULL) return NULL;

    u->id = -1;
    strcpy(u->username, "");
    strcpy(u->password, "");
    u->penyakit = NULL;
    u->role = ROLE_NONE;
    strcpy(u->riwayat_penyakit, "");
    u->suhu_tubuh = 0.0;
    u->tekanan_darah_sistolik = 0;
    u->tekanan_darah_diastolik = 0;
    u->detak_jantung = 0;
    u->saturasi_oksigen = 0.0;
    u->kadar_gula_darah = 0;
    u->berat_badan = 0.0;
    u->tinggi_badan = 0;
    u->kadar_kolesterol = 0;
    u->kadar_kolesterol_ldl = 0;
    u->trombosit = 0;
    strcpy(u->assigned_dokter, "");
    strcpy(u->ruangan, "");
    u->jumlah_obat_inventory = 0;
    u->jumlah_obat_yang_diminum = 0;
    u->telah_Didiagnosis = false;
    u->aura = 0;
    return u;
}

Penyakit* make_Empty_Penyakit() {
    Penyakit* p = (Penyakit*) malloc(sizeof(Penyakit));
    if (p == NULL) return NULL;

    p->id = -1;
    strcpy(p->nama_penyakit, "");
    p->suhu_tubuh_min = p->suhu_tubuh_max = 0.0;
    p->tekanan_darah_sistolik_min = p->tekanan_darah_sistolik_max = 0;
    p->tekanan_darah_diastolik_min = p->tekanan_darah_diastolik_max = 0;
    p->detak_jantung_min = p->detak_jantung_max = 0;
    p->saturasi_oksigen_min = p->saturasi_oksigen_max = 0.0;
    p->kadar_gula_darah_min = p->kadar_gula_darah_max = 0.0;
    p->berat_badan_min = p->berat_badan_max = 0.0;
    p->tinggi_badan_min = p->tinggi_badan_max = 0;
    p->kadar_kolesterol_min = p->kadar_kolesterol_max = 0;
    p->trombosit_min = p->trombosit_max = 0;

    return p;
}

obat* make_Empty_Obat() {
    obat* o = (obat*) malloc(sizeof(obat));
    if (o == NULL) return NULL;

    o->id_obat = 0;
    strcpy(o->nama_obat, "");
    return o;
}

void CreateAntrian(Antrian *q){
    q->depan = NULL;
    q->belakang = NULL;
    q->panjang = 0;
}

boolean isEmpty(Antrian q){
    return (q.depan == NULL);
}

int length(Antrian *q){
    return q->panjang;
}

void enqueue(Antrian *q, User *val) {
    NodeUser *newNode = malloc(sizeof(NodeUser));
    if (!newNode) {
        
        return;
    }
    newNode->data = val;
    newNode->next = NULL;

    if (q->depan == NULL) {
        q->depan = newNode;
        q->belakang = newNode;
    } else {
        q->belakang->next = newNode;
        q->belakang = newNode;
    }
    q->panjang++;
}
void dequeue(Antrian *q, User *val) {
    if (isEmpty(*q)) {
        memset(val, 0, sizeof(User));
        return;
    }

    NodeUser *temp = q->depan;
    *val = *(temp->data); 
    q->depan = q->depan->next;
    
    if (q->depan == NULL) {
        q->belakang = NULL;
    }
    // if (temp->data != NULL) {
         //free(temp->data); 
    // }
    free(temp);
    q->panjang--;
}
User* getUser_by_username(char* name, UserSet* userset){
    for (int i = 0; i < userset->count; i++){
        if (strcmp(userset->users[i].username, name) == 0){
            return &userset->users[i];
        }
    }
    return makeEmptyUser();
}

void initDataRuangan(Data_sistem* sistem, int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        // Initialize all fields
        memset(&sistem->daftar_ruangan[i], 0, sizeof(Data_sistem));
        //strncpy(sistem->daftar_ruangan[i].NamaRuangan, koderuangan[i], sizeof(sistem->daftar_ruangan[i].NamaRuangan)-1);
        
        // Allocate 2D array
        sistem->daftar_ruangan[i].data = malloc(sistem->panjang_ruangan * sizeof(int*));
        if (!sistem->daftar_ruangan[i].data) {
            printf("Error: Gagal alokasi memori untuk data ruangan %d\n", i);
            exit(1);
        }
        
        for (int j = 0; j <sistem->panjang_ruangan; j++) {
            sistem->daftar_ruangan[i].data[j] = malloc(sistem->lebar_ruangan * sizeof(int));
            if (!sistem->daftar_ruangan[i].data[j]) {
                printf("Error: Gagal alokasi memori untuk baris %d ruangan %d\n", j, i);
                exit(1);
            }
            memset(sistem->daftar_ruangan[i].data[j], -1, sistem->lebar_ruangan * sizeof(int));
        }

        // Initialize patient array
        sistem->daftar_ruangan[i].pasien_aktif = malloc(sistem->daftar_ruangan[i].kapasitas * sizeof(User));
        if (!sistem->daftar_ruangan[i].pasien_aktif) {
            printf("Error: Gagal alokasi memori untuk pasien aktif ruangan %d\n", i);
            exit(1);
        }
        for (int j = 0; j < sistem->daftar_ruangan[i].kapasitas; j++) {
            sistem->daftar_ruangan[i].pasien_aktif[j] = *makeEmptyUser();
        }

        CreateAntrian(sistem->daftar_ruangan[i].antrian_pasien);
    }
}

void lihat_semua_antrian(Data_ruangan* data_ruangan,Data_sistem *sistem) {
    printf("\n>>>📋 LIHAT_SEMUA_ANTRIAN\n");
    int total_ruangan = sistem->jumlah_ruangan;
    for (int i = 0; i < total_ruangan; i++) {
        printf("\n============ %s ============\n", data_ruangan[i].NamaRuangan);
        printf("Kapasitas  : %d\n", data_ruangan[i].kapasitas);

        // Cek apakah dokter ada dan valid
        printf("🩺 Dokter     : ");
        if (data_ruangan[i].dokter_di_ruangan != NULL &&
            data_ruangan[i].dokter_di_ruangan->id > 0 &&
            data_ruangan[i].dokter_di_ruangan->role == ROLE_DOKTER) {
            printf("%s\n", data_ruangan[i].dokter_di_ruangan->username);
        } else {
            printf("-\n");
        }

        // Pasien aktif
        printf("🧑‍⚕️ Pasien di dalam ruangan:\n");
        if (data_ruangan[i].jumlah_pasien_aktif > 0) {
            for (int j = 0; j < data_ruangan[i].jumlah_pasien_aktif; j++) {
                if (data_ruangan[i].pasien_aktif[j].role == ROLE_PASIEN) {
                    printf("  %d. %s\n", j + 1, data_ruangan[i].pasien_aktif[j].username);
                }
            }
        } else {
            printf("  (Belum ada pasien)\n");
        }

        // Cek apakah antrian NULL
        printf("Antrian luar (%d):\n", 
               data_ruangan[i].antrian_pasien ? data_ruangan[i].antrian_pasien->panjang : 0);

        if (data_ruangan[i].antrian_pasien != NULL &&
            !isEmpty(*data_ruangan[i].antrian_pasien)) {
            NodeUser* current = data_ruangan[i].antrian_pasien->depan;
            int counter = 1;
            while (current != NULL) {
                printf("  %d. %s\n", counter++, current->data->username);
                current = current->next;
            }
        } else {
            printf("  (Tidak ada antrian)\n");
        }

        printf("\n");
    }
}
