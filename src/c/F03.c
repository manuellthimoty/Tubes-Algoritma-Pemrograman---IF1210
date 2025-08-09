#include <stdio.h>
#include "F03.h"

void logout(UserSet* userSet) {
    if (userSet->loggedInIndex == -1) {
        printf("Logout gagal!\nAnda belum login.\n");
    } else {
        userSet->loggedInIndex = -1;
        printf("Sampai jumpa!\n");
    }
}

