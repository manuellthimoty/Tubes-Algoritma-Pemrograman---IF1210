#include "F16.h"
#include "Boolean.h"


void geser_kanan_obat_statis(obat arr[], int* jumlah) {
    if (*jumlah >= MAX_INVENTORY_OBAT) return; 

    for (int i = *jumlah; i > 0; i--) {
        arr[i] = arr[i - 1];
    }
    (*jumlah)++; 
    
}

void geser_kiri_obat_statis(obat arr[], int* jumlah, int indeks_hapus) {
    for (int i = indeks_hapus; i < *jumlah - 1; i++) {
        arr[i] = arr[i + 1];
    }
    (*jumlah)--;
}

void print_clean_string(const char *s) {
    for (int i = 0; s[i] != '\0'; i++) {
        if (s[i] == '\n') break;
        putchar(s[i]);
    }
}

char* getNamaObatById(Sistem_pengobatan* sistem, int id_obat) {
    for (int i = 0; i < sistem->jumlah_obat; i++) {
        if (sistem->daftar_obat[i].id_obat == id_obat) {
            return sistem->daftar_obat[i].nama_obat;
        }
    }
    return "(Obat Tidak Ditemukan)";
}

void Minum_Obat( UserSet* userset,Sistem_pengobatan* sistemObat) {
    printf(">>>💊 MINUM_OBAT\n");
    printf("============ DAFTAR OBAT ============\n");
    User* user = &userset->users[userset->loggedInIndex];

    // int jumlah_obat_inventory = userset->users[userset->loggedInIndex].jumlah_obat_inventory;
    // int jumlah_obat_diminum = userset->users[userset->loggedInIndex].jumlah_obat_yang_diminum;
    if (user->jumlah_obat_inventory  <= 0) {
        printf("❌📦 Kamu tidak memiliki obat untuk diminum.\n");
        return;
    }
    for (int i = 0; i < user->jumlah_obat_inventory; i++) {
        int id = user->inventory_obat[i].id_obat;
        char* nama = getNamaObatById(sistemObat, id);  
        printf("%d. %s\n", i+1, nama);
        strcpy(user->inventory_obat[i].nama_obat,nama);
        //printf("%d. %s\n", i + 1, user->inventory_obat[i].nama_obat);
    }
    int pilihan;
    printf(">>>💊 Pilih obat untuk diminum: ");
    scanf("%d", &pilihan);

    if (pilihan < 1 || pilihan > user->jumlah_obat_inventory) {
        printf("❌Pilihan nomor tidak tersedia!\n");
        return;
    }
    obat obat_dipilih;
    strncpy(obat_dipilih.nama_obat, user->inventory_obat[pilihan - 1].nama_obat, MAX_CHAR_OBAT - 1);
    obat_dipilih.nama_obat[MAX_CHAR_OBAT - 1] = '\0';
    obat_dipilih.id_obat = user->inventory_obat[pilihan - 1].id_obat;
    //printf("[DEBUG] Sebelum geser: '%s'\n", obat_dipilih.nama_obat);


    geser_kiri_obat_statis(user->inventory_obat, &user->jumlah_obat_inventory, pilihan - 1);
    geser_kanan_obat_statis(user->obat_yg_diminum, &user->jumlah_obat_yang_diminum);
    user->obat_yg_diminum[0] = obat_dipilih;
    //printf("[DEBUG] Setelah assign: '%s'\n", user->obat_yg_diminum[0].nama_obat);
    // for (int i = pilihan - 1; i < jumlah_obat_inventory - 1; i++) {
    //     userset->users[userset->loggedInIndex].inventory_obat[i] = userset->users[userset->loggedInIndex].inventory_obat[i + 1];
    // }

    // userset->users[userset->loggedInIndex].jumlah_obat_inventory--;
    // userset->users[userset->loggedInIndex].jumlah_obat_yang_diminum++;

    // userset->users->obat_yg_diminum[userset->users[userset->loggedInIndex].jumlah_obat_yang_diminum-1] = obat_dipilih;
    
    // userset->users[userset->loggedInIndex].obat_yg_diminum = malloc(sizeof(obat));

    // *userset->users[userset->loggedInIndex].obat_yg_diminum = obat_dipilih;
    printf("GLEKGLEKGLEK....\n");
    printf("💊%s\n", user->obat_yg_diminum[0].nama_obat);
    printf("berhasil diminum!!\n");
}