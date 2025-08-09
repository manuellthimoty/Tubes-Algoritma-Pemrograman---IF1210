#ifndef HEADERSEMUAADT_H
#define HEADERSEMUAADT_H

// ======================
//      SEMUA INCLUDE
// ======================
#include "Boolean.h"
// ======================
//      SEMUA DIFINE
// ======================
#define MAX_DOKTER 50
#define MAX_MAP_SIZE 128
#define BANYAK_RUANGAN 6
#define CONFIG_PATH "../src/file/config.txt"
#define PENYAKIT_CSV_PATH "../src/file/penyakit.csv"
#define USER_CSV_PATH "../src/file/user.csv"
#define TEMP_CSV_PATH "../src/file/temp.txt"
#define OBAT_CSV_PATH "../src/file/obat.csv"
#define OBAT_PENYAKIT_CSV_PATH "../src/file/obat_penyakit.csv"
#define MAX_LINE_LENGTH 256
#define MAX_USERS 100
#define MAKS_PENYAKIT 100
#define QUEUE_CAPACITY 100
#define MAKS_NAMA 100
#define MAKS_PERAN 10
#define MAX_LINE 512
#define MAX_FIELDS_USER 16
#define MAX_FIELDS_PENYAKIT 22
#define MAX_FIELDS_OBAT_PENYAKIT 3
#define MAX_FIELDS_OBAT 2
#define MAX_FIELD_LEN 64
#define IDX_UNDEF -1
#define CAPACITY 100
#define MAX_TOKENS 64
#define MAX_DATA 64
#define MAX_PENYAKIT 100
#define MAX_OBAT 200
#define MAKS_RELASI_OBAT_PENYAKIT 200
#define MAX_INVENTORY_OBAT 20
#define MAX_CHAR_OBAT 200
#define MARK -9999
#define IDX_NOTVALID -1
// ======================
//      KUMPULAN ADT
// ======================
// ADT BATAS PENYAKIT
typedef struct {
    int id;
    char nama_penyakit[MAX_LINE_LENGTH];
    float suhu_tubuh_min;
    float suhu_tubuh_max;
    int tekanan_darah_sistolik_min;
    int tekanan_darah_sistolik_max;
    int tekanan_darah_diastolik_min;
    int tekanan_darah_diastolik_max;
    int detak_jantung_min;
    int detak_jantung_max;
    float saturasi_oksigen_min;
    float saturasi_oksigen_max;
    float kadar_gula_darah_min;
    float kadar_gula_darah_max;
    float berat_badan_min;
    float berat_badan_max;
    int tinggi_badan_min;
    int tinggi_badan_max;
    int kadar_kolesterol_min;
    int kadar_kolesterol_max;
    int trombosit_min;
    int trombosit_max;

} Penyakit;

// ADT OBAT
typedef struct{
    int id_obat;
    char nama_obat [MAX_CHAR_OBAT];
}obat;

typedef struct{
    obat _obat;
    Penyakit penyakit;
    int urutan_minum;
}obat_penyakit;

typedef struct {
    obat *daftar_obat;
    int jumlah_obat;
    Penyakit *daftar_penyakit;
    int jumlah_penyakit;  
    obat_penyakit *relasi_obat_penyakit;
    int jumlah_relasi;
} Sistem_pengobatan;

// ADT ROLE
typedef enum {
    ROLE_NONE,
    ROLE_MANAGER,
    ROLE_DOKTER,
    ROLE_PASIEN
} Role;

// ADT PENYAKIT
typedef struct {
    Penyakit penyakit[MAKS_PENYAKIT];
    int count;
} Daftar_Penyakit;


// ADT USER
typedef struct {
    int id;
    char username[50];
    char password[50];    
    Role role;
    // KHUSUS PASIEN DOANG //
    Penyakit* penyakit;
    char riwayat_penyakit[100];
    float suhu_tubuh;
    int tekanan_darah_sistolik;
    int tekanan_darah_diastolik;
    int detak_jantung;
    float saturasi_oksigen;
    int kadar_gula_darah;
    float berat_badan;
    int tinggi_badan;
    int kadar_kolesterol;
    int kadar_kolesterol_ldl;
    int trombosit;
    char assigned_dokter[50];
    char ruangan[16];
    obat obat_yg_diminum[MAX_INVENTORY_OBAT];
    obat inventory_obat[MAX_INVENTORY_OBAT];
    int jumlah_obat_inventory;
    int jumlah_obat_yang_diminum;
    boolean telah_Didiagnosis;
    // KHUSUS DOKTER DOANG //
    int aura;
} User;

// ADT USERSET
typedef struct {
    User* users;
    int count;
    int loggedInIndex;
} UserSet;


typedef struct nodeUser {
    User *data;
    struct nodeUser* next;
} NodeUser;

// ADT ANTRIAN/QUEUE
typedef struct Antrian {
    NodeUser* depan;
    NodeUser* belakang;
    int panjang;
} Antrian;

// ADT MAP
typedef struct {
    int key;
    int value;
} MapPair;

typedef struct {
    MapPair data[MAX_MAP_SIZE];
    int size;
} Map;
// ADT RUANGAN
typedef struct Data_ruangan {
    int** data;
    int panjangdata[64];
    User* dokter_di_ruangan;
    int jumlah_pasien_aktif;
    int kapasitas;
    int kapasitas_antrian;
    User* pasien_aktif;
    char NamaRuangan[16];
    Antrian* antrian_pasien;
    Antrian* antrian_pasien_dalam_ruangan;
} Data_ruangan;

// ADT SISTEM
typedef struct {
    Data_ruangan *daftar_ruangan;
    int jumlah_ruangan;
    Map dokter_ke_idx_ruangan;
    int panjang_ruangan;
    int lebar_ruangan;
    //char** koderuangan;
} Data_sistem;

// daftar_obat Obat[MAX_OBAT];
// obat_penyakit obat[MAX_PENYAKIT];


//BUAT KODE RUANGANNYA:
//extern char koderuangan[][16];
#endif