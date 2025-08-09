#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "F06.h"

char* getUsernameById(int id, char usernames[][50], int max_users) {
    if (id < 0 || id >= max_users) return NULL; // invalid ID guard
    return usernames[id];
}

void generateKodeRuanganArray(Data_sistem* sistem) {
    int jumlah = sistem->panjang_ruangan * sistem->lebar_ruangan;
    sistem->jumlah_ruangan = jumlah;

    for (int i = 0; i < jumlah; i++) {
        int row = i / sistem->lebar_ruangan;
        int col = i % sistem->lebar_ruangan;

        sprintf(sistem->daftar_ruangan[i].NamaRuangan, "%c%d", 'A' + row, col + 1);
    }
}


int getRoleById(int id) {
    FILE* file = fopen(USER_CSV_PATH, "r");
    if (!file) return -1;

    char line[300];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        int uid;
        char username[50], password[50], role[50];
        char* ptr = line;
        char* semi = strchr(ptr, ';');
        if (!semi) continue;
        *semi = '\0';
        uid = atoi(ptr);
        ptr = semi + 1;
        if (uid != id) continue;
        semi = strchr(ptr, ';');
        if (!semi) continue;
        *semi = '\0';
        strncpy(username, ptr, 49);
        username[49] = '\0';
        ptr = semi + 1;
        semi = strchr(ptr, ';');
        if (!semi) continue;
        *semi = '\0';
        strncpy(password, ptr, 49);
        password[49] = '\0';
        ptr = semi + 1;

        strncpy(role, ptr, 49);
        role[49] = '\0';

        fclose(file);
        if (strcmp(role, "dokter") == 0) return ROLE_DOKTER;
        if (strcmp(role, "pasien") == 0) return ROLE_PASIEN;
        if (strcmp(role, "manager") == 0) return ROLE_MANAGER;
        return -1;
    }

    fclose(file);
    return -1;
}


void loadUsernames(char usernames[][50], int max_users) {
    FILE* file = fopen(USER_CSV_PATH, "r");
    if (!file) return;

    char line[300];
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';

        int id;
        char username[50];
        char* ptr = line;

        // Ambil ID
        char* semi = strchr(ptr, ';');
        if (!semi) continue;
        *semi = '\0';
        id = atoi(ptr);
        ptr = semi + 1;

        // Ambil username
        semi = strchr(ptr, ';');
        if (!semi) continue;
        *semi = '\0';
        strncpy(username, ptr, 49);
        username[49] = '\0';

        if (id >= 0 && id < max_users) {
            strcpy(usernames[id], username);
        }
    }

    fclose(file);
}



User getUserById(int id, UserSet* userset) {
    for (int i = 0; i < userset->count; i++) {
        if (userset->users[i].id == id) {
            return userset->users[i];
        }
    }
    return *makeEmptyUser();
}

void parse_line_manual(const char* line, int* output, int* count) {
    *count = 0;
    int num = 0;
    int sign = 1;
    int in_number = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        // Manually check for whitespace (space, tab, newline, etc.)
        if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r' || line[i] == '\f' || line[i] == '\v') {
            if (in_number) {
                output[(*count)++] = num * sign;
                num = 0;
                sign = 1;
                in_number = 0;
            }
        } 
        else if (line[i] == '-') {
            sign = -1;
            in_number = 1;
        } 
        // Manually check if the character is a digit (0-9)
        else if (line[i] >= '0' && line[i] <= '9') {
            num = num * 10 + (line[i] - '0');
            in_number = 1;
        } 
        else {
            // Non-digit, non-space character - ignore or break if needed
        }
    }

    // If the line ends with a number, add the last number
    if (in_number) {
        output[(*count)++] = num * sign;
    }
}
User* getUserByIdPointer(int id, UserSet* userset) {
    if (!userset || id <= 0) {  
        return NULL;
    }

    // Search for user
    for (int i = 0; i < userset->count; i++) {
        if (userset->users[i].id == id) {
            return &userset->users[i];  
        }
    }

    return NULL;  // Not found
}
void load_ruangan_config(Data_sistem* sistem, UserSet* userset) {
    FILE* fp = fopen(CONFIG_PATH, "r");
    if (!fp) {
        printf("Gagal buka file config\n");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int panjang = 0, lebar = 0;
    int kapasitas_ruangan = 0, kapasitas_antrian = 0;

    if (fgets(line, sizeof(line), fp)) sscanf(line, "%d %d", &panjang, &lebar);
    if (fgets(line, sizeof(line), fp)) sscanf(line, "%d %d", &kapasitas_ruangan, &kapasitas_antrian);

    sistem->panjang_ruangan = panjang;
    sistem->lebar_ruangan = lebar;
    sistem->jumlah_ruangan = panjang * lebar;

    if (sistem->jumlah_ruangan <= 0) {
        printf("Ukuran denah tidak valid\n");
        fclose(fp);
        return;
    }

    sistem->daftar_ruangan = malloc(sistem->jumlah_ruangan * sizeof(Data_ruangan));

    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        Data_ruangan* ruangan = &sistem->daftar_ruangan[i];
        ruangan->kapasitas = kapasitas_ruangan;
        ruangan->kapasitas_antrian = kapasitas_antrian;
        ruangan->jumlah_pasien_aktif = 0;

        snprintf(ruangan->NamaRuangan, sizeof(ruangan->NamaRuangan), "%c%d", 'A' + (i / lebar), (i % lebar) + 1);

        ruangan->pasien_aktif = malloc(kapasitas_ruangan * sizeof(User));
        ruangan->dokter_di_ruangan = NULL;
        ruangan->antrian_pasien = malloc(sizeof(Antrian));
        ruangan->antrian_pasien_dalam_ruangan = malloc(sizeof(Antrian));
        CreateAntrian(ruangan->antrian_pasien);
        CreateAntrian(ruangan->antrian_pasien_dalam_ruangan);
    }

    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        if (!fgets(line, sizeof(line), fp)) break;

        int tokens[MAX_TOKENS], token_count = 0;
        char* p = line;
        while (token_count < MAX_TOKENS) {
            int val, chars_read;
            if (sscanf(p, "%d%n", &val, &chars_read) == 1) {
                tokens[token_count++] = val;
                p += chars_read;
                while (*p == ' ' || *p == '\t') p++;
            } else break;
        }

        Data_ruangan* ruangan = &sistem->daftar_ruangan[i];

        if (token_count == 1 && tokens[0] == 0) continue;

        int dokter_id = tokens[0];
        User* dokter = getUserByIdPointer(dokter_id, userset);
        if  (dokter != NULL && dokter->id > 0 && dokter->role == ROLE_DOKTER) {
            ruangan->dokter_di_ruangan = dokter;
            strncpy(dokter->ruangan, ruangan->NamaRuangan, sizeof(dokter->ruangan) - 1);
            dokter->ruangan[sizeof(dokter->ruangan) - 1] = '\0';
        }

        int batas_pasien = (token_count - 1 < kapasitas_ruangan) ? token_count - 1 : kapasitas_ruangan;
        for (int j = 0; j < batas_pasien; j++) {
            User* pasien = getUserByIdPointer(tokens[j + 1], userset);
            if (pasien && pasien->role == ROLE_PASIEN) {
                ruangan->pasien_aktif[ruangan->jumlah_pasien_aktif++] = *pasien;
                strncpy(pasien->assigned_dokter, dokter ? dokter->username : "", sizeof(pasien->assigned_dokter) - 1);
                strncpy(pasien->ruangan, ruangan->NamaRuangan, sizeof(pasien->ruangan) - 1);
                enqueue(ruangan->antrian_pasien_dalam_ruangan, pasien);
            }
        }

        for (int j = 1 + kapasitas_ruangan; j < token_count; j++) {
            User* pasien_antri = getUserByIdPointer(tokens[j], userset);
            if (pasien_antri && pasien_antri->role == ROLE_PASIEN) {
                if (!isInAnyQueue(sistem, pasien_antri->id)) {
                    enqueue(ruangan->antrian_pasien, pasien_antri);
                }
            }
        }
    }

    int jumlah_pasien_dengan_obat = 0;
    if (fgets(line, sizeof(line), fp)) sscanf(line, "%d", &jumlah_pasien_dengan_obat);

    for (int i = 0; i < jumlah_pasien_dengan_obat; i++) {
        if (!fgets(line, sizeof(line), fp)) break;
        line[strcspn(line, "\r\n")] = 0;

        int tokens[MAX_TOKENS], token_count = 0;
        char* p = line;
        while (token_count < MAX_TOKENS) {
            int val, chars_read;
            if (sscanf(p, "%d%n", &val, &chars_read) == 1) {
                tokens[token_count++] = val;
                p += chars_read;
                while (*p == ' ' || *p == '\t') p++;
            } else break;
        }

        if (token_count < 2) continue;

        User* pasien = getUserByIdPointer(tokens[0], userset);
        if (pasien && pasien->role == ROLE_PASIEN) {
            pasien->jumlah_obat_inventory = 0;
            for (int j = 1; j < token_count; j++) {
                pasien->inventory_obat[pasien->jumlah_obat_inventory++].id_obat = tokens[j];
            }
        } else {
            printf("[WARNING] Pasien ID %d tidak ditemukan atau bukan pasien.\n", tokens[0]);
        }
    }

    int jumlah_pasien_dengan_perut = 0;
    if (fgets(line, sizeof(line), fp)) sscanf(line, "%d", &jumlah_pasien_dengan_perut);

    for (int i = 0; i < jumlah_pasien_dengan_perut; i++) {
        if (!fgets(line, sizeof(line), fp)) break;

        int tokens[MAX_TOKENS], token_count = 0;
        char* p = line;
        while (token_count < MAX_TOKENS) {
            int val, chars_read;
            if (sscanf(p, "%d%n", &val, &chars_read) == 1) {
                tokens[token_count++] = val;
                p += chars_read;
                while (*p == ' ' || *p == '\t') p++;
            } else break;
        }

        if (token_count < 2) continue;
        User* pasien = getUserByIdPointer(tokens[0], userset);
        if (pasien && pasien->role == ROLE_PASIEN) {
            pasien->jumlah_obat_yang_diminum = 0;
            for (int j = 1; j < token_count; j++) {
                pasien->obat_yg_diminum[pasien->jumlah_obat_yang_diminum++].id_obat = tokens[j];
            }
        }
    }

    fclose(fp);
}



void lihat_denah(Data_sistem *sistem) {
    //int banyaknya_ruangan = sistem->panjang_ruangan * sistem->lebar_ruangan;
    printf("     ");
    for (int c = 1; c <= sistem->lebar_ruangan; c++) {
        printf("  %d   ", c);
    }
    printf("\n");

    for (int r = 0; r < sistem->panjang_ruangan; r++) {
        printf("   +");
        for (int c = 0; c < sistem->lebar_ruangan; c++) {
            printf("-----+");
        }
        printf("\n");
        printf(" %c |", 'A' + r);
        for (int c = 0; c < sistem->lebar_ruangan; c++) {
            int idx = r * sistem->lebar_ruangan + c;
            snprintf(sistem->daftar_ruangan[idx].NamaRuangan, 16, "%c%d", 'A' + r, c + 1);
            printf(" %-4s|", sistem->daftar_ruangan[idx].NamaRuangan); // rata kiri, lebar 5
        }
        printf("\n");
    }

    printf("   +");
    for (int c = 0; c < sistem->lebar_ruangan; c++) {
        printf("-----+");
    }
    printf("\n");
}

int getRuanganIndex(const char* kode,Data_sistem* sistem) {
    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        if (strcmp(sistem->daftar_ruangan[i].NamaRuangan, kode) == 0) return i;
    }
    return -1;
}   
int getRuanganIndexbyUser(User user,Data_sistem* sistem) {
    for (int i = 0; i < sistem->jumlah_ruangan; i++) {
        if (strcmp(sistem->daftar_ruangan[i].NamaRuangan,user.ruangan) == 0) return i;
    }
    return -1;
} 
void Lihat_Detail_Denah(Data_sistem *d) {
    char input[10];
    printf("\nPilih Kode Ruangan: ");
    scanf("%s", input);

    int no_ruangan = -1;
    for (int i = 0; i < d->panjang_ruangan * d->lebar_ruangan; i++) {
        //printf("[DEBUG]:%s Nama_Ruangan",d->daftar_ruangan[i].NamaRuangan);
        if (strcmp(d->daftar_ruangan[i].NamaRuangan, input) == 0) {
            no_ruangan = i;
            break;
        }
    }
    
    if (no_ruangan == -1) {
        printf("Ruangan tidak ditemukan.\n");
        return;
    }

    printf("\n>>> LIHAT_RUANGAN %s\n", input);
    printf("\n--- Detail Ruangan %s ---\n", input);
    printf("Kapasitas  : %d\n", d->daftar_ruangan[no_ruangan].kapasitas);

    User* dokter = d->daftar_ruangan[no_ruangan].dokter_di_ruangan;
    if (dokter != NULL && dokter->id > 0 && dokter->role == ROLE_DOKTER) {
        printf("Dokter     : %s\n", dokter->username);
    } else {
        printf("Dokter     : -\n");
    }

    printf("Pasien di dalam ruangan:\n");
    if (d->daftar_ruangan[no_ruangan].jumlah_pasien_aktif == 0) {
        printf("  (Belum ada pasien dalam ruangan)\n");
    } else {
        for (int i = 0; i < d->daftar_ruangan[no_ruangan].jumlah_pasien_aktif; i++) {
            printf("  %d. %s\n", i + 1, d->daftar_ruangan[no_ruangan].pasien_aktif[i].username);
        }
    }
    printf("------------------------------\n");
}

