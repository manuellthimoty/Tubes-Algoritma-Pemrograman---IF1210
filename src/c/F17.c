#include "F17.h"

void Minum_Penawar(UserSet *userset){
    printf("\n>>>🧪 PENAWAR\n");
    if(strcmp(userset->users[userset->loggedInIndex].username,"")==0){
        printf("❌💊 Perut kosong!! Belum ada obat yang dimakan\n");
        return;
    }
    User* user = &userset->users[userset->loggedInIndex];

    // int jumlah_obat_inventory = userset->users[userset->loggedInIndex].jumlah_obat_inventory;
    // int jumlah_obat_diminum = userset->users[userset->loggedInIndex].jumlah_obat_yang_diminum;
    if (user->jumlah_obat_yang_diminum <= 0) {
        printf("❌💊 Perut kosong!! Belum ada obat yang dimakan.\n");
        return;
    }
    obat obat_keluar = user->obat_yg_diminum[0]; 
    geser_kiri_obat_statis(user->obat_yg_diminum,&user->jumlah_obat_yang_diminum,0);
    geser_kanan_obat_statis(user->inventory_obat, &user->jumlah_obat_inventory);
    user->inventory_obat[0] = obat_keluar;
    //printf("[DEBUG] Setelah assign: '%s'\n", user->obat_yg_diminum[0].nama_obat);
    //userset->users[userset->loggedInIndex].obat_yg_diminum = make_Empty_Obat();
    printf("\nUwekkk!!! \n");
    printf("🧪%s\n ",obat_keluar.nama_obat);
    printf("📦Telah keluar dan kembali ke inventory\n");

}