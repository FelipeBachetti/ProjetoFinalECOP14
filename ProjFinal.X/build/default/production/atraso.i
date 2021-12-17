# 1 "atraso.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:/Program Files/Microchip/MPLABX/v5.45/packs/Microchip/PIC18Fxxxx_DFP/1.2.26/xc8\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "atraso.c" 2

void atraso_ms(unsigned int num) {
    volatile unsigned char j, k;
    for (int i = 0; i < num; i++) {
        for (j = 0; j < 41; j++) {
            for (k = 0; k < 3; k++);
        }

    }
}
