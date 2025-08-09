#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "F05.h"

void Help(UserSet *db) {
    int index;
    if (db->loggedInIndex == -1) {
        printf(">>> 🆘 HELP\n\n");
        printf("=========== 🆘 HELP MENU 🆘 ===========\n\n");
        printf("🚫 Kamu belum login sebagai role apapun. Silakan login terlebih dahulu.\n\n");
        printf("    1. 🔐 LOGIN        : Masuk ke dalam akun yang sudah terdaftar\n");
        printf("    2. 📝 REGISTER     : Membuat akun baru\n\n");
        printf("📌 Footnote:\n");
        printf("    1. 💡 Untuk menggunakan aplikasi, silakan masukkan nama fungsi yang terdaftar\n");
        printf("    2. ⚠️ Jangan lupa untuk memasukkan input yang valid\n");
    } else {
        index = db->loggedInIndex;
        Role role = db->users[index].role;

        if (role == ROLE_DOKTER) {
            printf(">>> 🆘 HELP\n\n");
            printf("=========== 🩺 HELP UNTUK DOKTER 🩺 ===========\n\n");
            printf("👨‍⚕️ Halo Dokter %s. Kamu memanggil command HELP. Berikut adalah hal-hal yang dapat kamu lakukan:\n\n", db->users[index].username);
            printf("    1. 🔓 LOGOUT    : Keluar dari akun yang sedang digunakan\n");
            printf("    2. 🧾 DIAGNOSIS : Melakukan diagnosis pasien berdasarkan kondisi tubuh\n");
            printf("    3. 💊 NGOBATIN  : Memberikan obat kepada pasien\n");
            printf("    4. 📋 ANTRIAN   : Melihat antrian pasien\n\n");
            printf("📌 Footnote:\n");
            printf("    1. 💡 Masukkan nama fungsi sesuai yang tersedia di menu\n");
            printf("    2. ⚠️ Pastikan input yang diberikan valid\n");
        } else if (role == ROLE_PASIEN) {
            printf(">>> 🆘 HELP\n\n");
            printf("=========== 🧑‍🦽 HELP UNTUK PASIEN 🧑‍🦽 ===========\n\n");
            printf("🙋‍♂️ Selamat datang, %s. Berikut adalah hal-hal yang dapat kamu lakukan:\n\n", db->users[index].username);
            printf("    1. 🔓 LOGOUT         : Keluar dari akun yang sedang digunakan\n");
            printf("    2. 📅 DAFTAR_CHECKUP : Mendaftarkan diri untuk pemeriksaan\n");
            printf("    3. ⏳ ANTRIAN_SAYA   : Melihat antrian kamu saat ini\n");
            printf("    4. 💊 MINUM_OBAT     : Minum obat sesuai instruksi\n");
            printf("    5. 🧪 MINUM_PENAWAR  : Minum penawar jika tersedia\n");
            printf("    6. 🏃‍♂️ PULANGDOK      : Cek apakah boleh pulang\n\n");
            printf("📌 Footnote:\n");
            printf("    1. 💡 Gunakan nama fungsi yang terdaftar\n");
            printf("    2. ⚠️ Input harus valid dan sesuai instruksi\n");
        } else if (role == ROLE_MANAGER) {
            printf(">>> 🆘 HELP\n\n");
            printf("=========== 🧑‍💼 HELP UNTUK MANAGER 🧑‍💼 ===========\n\n");
            printf("💼 Halo Manager %s. Berikut adalah hal-hal yang bisa kamu lakukan sekarang:\n\n", db->users[index].username);
            printf("    1. 🔓 LOGOUT         : Keluar dari akun yang sedang digunakan\n");
            printf("    2. ➕ TAMBAH_DOKTER  : Menambahkan dokter baru ke sistem\n");
            printf("    3. 🏷️  ASSIGN_DOKTER  : Menugaskan dokter ke ruangan\n");
            printf("    4. 🧑‍⚕️ CARI_DOKTER   : Mencari informasi dokter\n");
            printf("    5. 🧑‍🦽 CARI_PASIEN  : Mencari informasi pasien\n");
            printf("    6. 👥 LIHAT_USER     : Melihat semua user dalam sistem\n");
            printf("    7. 🔄 PINDAH_DOKTER  : Memindahkan dokter ke ruangan lain\n");
            printf("    8. 🏗️ UBAH_DENAH      : Mengubah denah ruangan\n");
            printf("    9. 📋 LIHAT_ANTRIAN  : Melihat semua antrian\n\n");
            printf("📌 Footnote:\n");
            printf("    1. 💡 Gunakan nama fungsi yang tersedia di menu\n");
            printf("    2. ⚠️ Pastikan input valid dan sesuai\n");
        }
    }
}
