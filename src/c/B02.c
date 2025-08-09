#include "B02.h"

void Ubah_Denah(Data_sistem* sistem, UserSet* userset) {
    printf("\n>>> 🗺️ UBAH DENAH \n");
    printf("\nUbah Menjadi?:\n (Baris Kolom)\n");
    int baris_baru, kolom_baru;
    int jumlah_lama = sistem->jumlah_ruangan;
    scanf("%d %d", &baris_baru, &kolom_baru);
    int jumlah_baru = baris_baru * kolom_baru;

    if (jumlah_baru < jumlah_lama) {
        for (int i = jumlah_baru; i < jumlah_lama; i++) {
            Data_ruangan* ruangan = &sistem->daftar_ruangan[i];
            if (ruangan->dokter_di_ruangan && ruangan->dokter_di_ruangan->id != -1) {
                printf("Tidak bisa ubah denah. Ruangan %s masih ditempati dokter %s.\n",
                       ruangan->NamaRuangan, ruangan->dokter_di_ruangan->username);
                return;
            }
        }
    }

    Data_ruangan* ruangan_baru = malloc(jumlah_baru * sizeof(Data_ruangan));
    int jumlah_yang_disalin = (jumlah_lama < jumlah_baru) ? jumlah_lama : jumlah_baru;

    for (int i = 0; i < jumlah_yang_disalin; i++) {
        ruangan_baru[i] = sistem->daftar_ruangan[i];
        snprintf(ruangan_baru[i].NamaRuangan, 16, "%c%d", 'A' + (i / kolom_baru), (i % kolom_baru) + 1);
    }

    for (int i = jumlah_yang_disalin; i < jumlah_baru; i++) {
        ruangan_baru[i].kapasitas = 5;
        ruangan_baru[i].kapasitas_antrian = 5;
        ruangan_baru[i].jumlah_pasien_aktif = 0;
        snprintf(ruangan_baru[i].NamaRuangan, 16, "%c%d", 'A' + (i / kolom_baru), (i % kolom_baru) + 1);

        ruangan_baru[i].pasien_aktif = malloc(5 * sizeof(User));
        ruangan_baru[i].dokter_di_ruangan = makeEmptyUser();
        ruangan_baru[i].antrian_pasien = malloc(sizeof(Antrian));
        CreateAntrian(ruangan_baru[i].antrian_pasien);
        ruangan_baru[i].antrian_pasien_dalam_ruangan = malloc(sizeof(Antrian));
        CreateAntrian(ruangan_baru[i].antrian_pasien_dalam_ruangan);
    }

    if (jumlah_baru < jumlah_lama) {
        for (int i = jumlah_baru; i < jumlah_lama; i++) {
            free(sistem->daftar_ruangan[i].pasien_aktif);
            free(sistem->daftar_ruangan[i].dokter_di_ruangan);
            free(sistem->daftar_ruangan[i].antrian_pasien);
            free(sistem->daftar_ruangan[i].antrian_pasien_dalam_ruangan);
        }
    }

    free(sistem->daftar_ruangan);
    sistem->daftar_ruangan = ruangan_baru;
    sistem->panjang_ruangan = baris_baru;
    sistem->lebar_ruangan = kolom_baru;
    sistem->jumlah_ruangan = jumlah_baru;

    printf("Ukuran denah berhasil diubah menjadi %dx%d.\n", baris_baru, kolom_baru);
    saveConfig(sistem, CONFIG_PATH, userset);
}

void Pindah_Dokter(UserSet* user, Data_sistem* sistem) {
    if (user->users[user->loggedInIndex].role != ROLE_MANAGER) return;

    char ruangan_asal[16], ruangan_pindah[16];
    printf("\nRuangan Asal: ");
    scanf("%s", ruangan_asal);
    printf("\nRuangan Pindah: ");
    scanf("%s", ruangan_pindah);

    int idx_ruangan_a = IDX_UNDEF, idx_ruangan_p = IDX_UNDEF;
    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        if (strcmp(sistem->daftar_ruangan[i].NamaRuangan, ruangan_asal) == 0) {
            idx_ruangan_a = i;
        }
        if (strcmp(sistem->daftar_ruangan[i].NamaRuangan, ruangan_pindah) == 0) {
            idx_ruangan_p = i;
        }
    }

    if (idx_ruangan_a == IDX_UNDEF || idx_ruangan_p == IDX_UNDEF) {
        printf("Salah satu ruangan tidak ditemukan.\n");
        return;
    }

    if (sistem->daftar_ruangan[idx_ruangan_p].dokter_di_ruangan != NULL &&
        sistem->daftar_ruangan[idx_ruangan_p].dokter_di_ruangan->id != -1) {
        printf("Pemindahan gagal. Ruangan %s sudah ditempati dokter lain.\n", ruangan_pindah);
        return;
    }

    User* dokter = sistem->daftar_ruangan[idx_ruangan_a].dokter_di_ruangan;
    sistem->daftar_ruangan[idx_ruangan_p].dokter_di_ruangan = dokter;
    sistem->daftar_ruangan[idx_ruangan_a].dokter_di_ruangan = makeEmptyUser();

    strcpy(dokter->assigned_dokter, ruangan_pindah);
    insertMap(&sistem->dokter_ke_idx_ruangan, dokter->id, idx_ruangan_p);

    printf("Dr. %s berhasil dipindahkan dari ruangan %s ke ruangan %s.\n",
           dokter->username, ruangan_asal, ruangan_pindah);

    saveConfig(sistem, CONFIG_PATH, user);
}
