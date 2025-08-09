#include "HeaderSemuaADT.h"
#include "BackToMenu.h"


void back(){
    printf("\n>>>>  TEKAN TOMBOL [M] UNTUK KEMBALI KE MAIN MENU  >>>>>\n");
    printf("\nInput:");
    char input;
    boolean valid = false;
    while (!valid)
    {
       scanf("%c",&input);
        if (input == 'M'||input == 'm'){
            return;
        }
    }
    printf("\n>>> Maaf Input salah silahkan Ulangi kembali ! >>>\n");
    
}