#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "F13.h"

void swap_obat_penyakit(obat_penyakit *a, obat_penyakit *b) {
    obat_penyakit temp = *a;
    *a = *b;
    *b = temp;
}

void sort_obat_penyakit_by_urutan(obat_penyakit arr[], int jumlah) {
    for (int i = 0; i < jumlah - 1; i++) {
        for (int j = 0; j < jumlah - 1 - i; j++) {
            if (arr[j].urutan_minum > arr[j + 1].urutan_minum) {
                swap_obat_penyakit(&arr[j], &arr[j + 1]);
            }
        }
    }
}

void tampilkan_perbandingan_urutan(User* pasien, Sistem_pengobatan* sistem_pengobatan) {
    printf("\nUrutan peminuman obat yang diharapkan:\n");
    obat_penyakit relasi_terkait[MAKS_RELASI_OBAT_PENYAKIT];
    int jumlah_terkait = 0;

    for (int i = 0; i < sistem_pengobatan->jumlah_relasi; i++) {
        if (sistem_pengobatan->relasi_obat_penyakit[i].penyakit.id == pasien->penyakit->id) {
            boolean sudah_ada = false;
            for (int k = 0; k < jumlah_terkait; k++) {
                if (relasi_terkait[k]._obat.id_obat == sistem_pengobatan->relasi_obat_penyakit[i]._obat.id_obat) {
                    sudah_ada = true;
                    break;
                }
            }
            if (!sudah_ada) {
                relasi_terkait[jumlah_terkait++] = sistem_pengobatan->relasi_obat_penyakit[i];
            }
        }
    }

    sort_obat_penyakit_by_urutan(relasi_terkait, jumlah_terkait);
    for (int i = 0; i < jumlah_terkait; i++) {
        printf("=> %s\n", relasi_terkait[i]._obat.nama_obat);
    }

    printf("\nUrutan obat yang kamu minum:\n");
    for (int i = 0; i < pasien->jumlah_obat_yang_diminum; i++) {
        printf("=> %s\n", pasien->obat_yg_diminum[i].nama_obat);
    }
}

int cari_urutan_minum(int id_obat, int id_penyakit, Sistem_pengobatan* sistem) {
    for (int i = 0; i < sistem->jumlah_relasi; i++) {
        if (sistem->relasi_obat_penyakit[i]._obat.id_obat == id_obat &&
            sistem->relasi_obat_penyakit[i].penyakit.id == id_penyakit) {
            return sistem->relasi_obat_penyakit[i].urutan_minum;
        }
    }
    return MARK;
}

obat* cari_urutan_obat(Sistem_pengobatan* sistem, int id_penyakit, int* jumlah_out) {
    static obat hasil[MAKS_RELASI_OBAT_PENYAKIT];
    int count = 0;

    for (int i = 0; i < sistem->jumlah_relasi; i++) {
        if (sistem->relasi_obat_penyakit[i].penyakit.id == id_penyakit) {
            boolean sudah_ada = false;
            for (int j = 0; j < count; j++) {
                if (hasil[j].id_obat == sistem->relasi_obat_penyakit[i]._obat.id_obat) {
                    sudah_ada = true;
                    break;
                }
            }
            if (!sudah_ada) {
                hasil[count++] = sistem->relasi_obat_penyakit[i]._obat;
            }
        }
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            int u1 = cari_urutan_minum(hasil[j].id_obat, id_penyakit, sistem);
            int u2 = cari_urutan_minum(hasil[j + 1].id_obat, id_penyakit, sistem);
            if (u1 > u2) {
                obat temp = hasil[j];
                hasil[j] = hasil[j + 1];
                hasil[j + 1] = temp;
            }
        }
    }

    *jumlah_out = count;
    return hasil;
}

boolean cek_urutan_minum(User* u, Sistem_pengobatan* sistem) {
    // Kumpulkan semua obat yang seharusnya diminum berdasarkan penyakit pasien
    obat urutan_harus[MAKS_RELASI_OBAT_PENYAKIT];
    int jumlah_harus = 0;

    for (int i = 0; i < sistem->jumlah_relasi; i++) {
        if (sistem->relasi_obat_penyakit[i].penyakit.id == u->penyakit->id) {
            obat ob = sistem->relasi_obat_penyakit[i]._obat;

            // Cek duplikasi id_obat (jaga-jaga relasi dobel)
            bool duplikat = false;
            for (int j = 0; j < jumlah_harus; j++) {
                if (urutan_harus[j].id_obat == ob.id_obat) {
                    duplikat = true;
                    break;
                }
            }
            if (!duplikat) {
                urutan_harus[jumlah_harus++] = ob;
            }
        }
    }

    // Urutkan berdasarkan urutan_minum
    for (int i = 0; i < jumlah_harus - 1; i++) {
        for (int j = 0; j < jumlah_harus - i - 1; j++) {
            int urutan_a = cari_urutan_minum(urutan_harus[j].id_obat, u->penyakit->id, sistem);
            int urutan_b = cari_urutan_minum(urutan_harus[j + 1].id_obat, u->penyakit->id, sistem);
            if (urutan_a > urutan_b) {
                obat temp = urutan_harus[j];
                urutan_harus[j] = urutan_harus[j + 1];
                urutan_harus[j + 1] = temp;
            }
        }
    }

    // Cek apakah jumlah sama
    if (u->jumlah_obat_yang_diminum != jumlah_harus) return false;

    // Bandingkan dari belakang (stack top) ke depan
    for (int i = 0; i < jumlah_harus; i++) {
        int idx_stack = u->jumlah_obat_yang_diminum - 1 - i;
        if (u->obat_yg_diminum[idx_stack].id_obat != urutan_harus[i].id_obat) {
            return false;
        }
    }

    return true;
}


boolean isAntrianKosong(Antrian* antrian) {
    return (antrian == NULL || antrian->depan == NULL);
}

void hapusPasienDariRuangan(Data_ruangan* ruangan, int pasien_id) {
    if (!ruangan || ruangan->jumlah_pasien_aktif <= 0) return;

    int idx = -1;
    for (int i = 0; i < ruangan->jumlah_pasien_aktif; i++) {
        if (ruangan->pasien_aktif[i].id == pasien_id) {
            idx = i;
            break;
        }
    }

    if (idx != -1) {
        for (int i = idx; i < ruangan->jumlah_pasien_aktif - 1; i++) {
            ruangan->pasien_aktif[i] = ruangan->pasien_aktif[i + 1];
        }
        memset(&ruangan->pasien_aktif[ruangan->jumlah_pasien_aktif - 1], 0, sizeof(User));
        ruangan->jumlah_pasien_aktif--;
    }
}

boolean isInPasienAktif(Data_ruangan* ruangan, int idPasien) {
    for (int i = 0; i < ruangan->jumlah_pasien_aktif; i++) {
        if (ruangan->pasien_aktif[i].id == idPasien) {
            return true;
        }
    }
    return false;
}

void boleh_pulang_gak_dok(UserSet* userset, Sistem_pengobatan* sistem, Data_sistem* data_sistem) {
    User* u = &userset->users[userset->loggedInIndex];
    if (strlen(u->assigned_dokter) == 0 || !u->telah_Didiagnosis) {
        printf(">>> PULANGDOK\n");
        printf("❌🧾 Kamu belum menerima diagnosis apapun dari dokter, jangan buru-buru pulang!\n");
        return;
    }

    User* dokter = getUser_by_username(u->assigned_dokter, userset);
    Data_ruangan* ruangan = ruangan_dokter(*dokter, data_sistem);

    printf(">>>🥹 PULANGDOK\n");
    printf("🔎 Dokter sedang memeriksa keadaanmu..\n");

    int jumlah_obat_harus = 0;
    for (int i = 0; i < sistem->jumlah_relasi; i++) {
        if (sistem->relasi_obat_penyakit[i].penyakit.id == u->penyakit->id) {
            boolean sudah_ada = false;
            for (int j = 0; j < i; j++) {
                if (sistem->relasi_obat_penyakit[j].penyakit.id == u->penyakit->id &&
                    sistem->relasi_obat_penyakit[j]._obat.id_obat == sistem->relasi_obat_penyakit[i]._obat.id_obat) {
                    sudah_ada = true;
                    break;
                }
            }
            if (!sudah_ada) {
                jumlah_obat_harus++;
            }
        }
    }

    // Cek apakah jumlah obat yang diminum cukup
    if (u->jumlah_obat_yang_diminum < jumlah_obat_harus) {
        printf("⚠️ Masih ada obat yang belum kamu habiskan, minum semuanya dulu yukk!\n");
        return;
    }

    // Cek urutan
    if (!cek_urutan_minum(u, sistem)) {
        printf("❌💊 Urutan minum kamu salah, berikut perbandingannya:\n");
        tampilkan_perbandingan_urutan(u, sistem);
        return;
    }

    // Jika semua oke, lanjut proses keluar
    User dummy;
    dequeue(ruangan->antrian_pasien_dalam_ruangan, &dummy);
    hapusPasienDariRuangan(ruangan, u->id);

    printf("✅ Selamat! Kamu sudah menyelesaikan perawatan dan boleh pulang.\n");

    if (!isAntrianKosong(ruangan->antrian_pasien)) {
        User next_pasien;
        dequeue(ruangan->antrian_pasien, &next_pasien);

        if (!isInAnyQueue(data_sistem, next_pasien.id) && !isInPasienAktif(ruangan, next_pasien.id)) {
            enqueue(ruangan->antrian_pasien_dalam_ruangan, &next_pasien);
        }

        if (!isInPasienAktif(ruangan, next_pasien.id) && ruangan->jumlah_pasien_aktif < ruangan->kapasitas) {
            ruangan->pasien_aktif[ruangan->jumlah_pasien_aktif++] = next_pasien;
        }
    }

    dokter->aura++;
    saveConfig(data_sistem, CONFIG_PATH, userset);
}
