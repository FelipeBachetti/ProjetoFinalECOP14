#include <pic18f4520.h>
#include "config.h"
#include "lcd.h"
#include "keypad.h"
#include "bits.h"
#include "ssd.h"
#include "atraso.h"
#include "pwm.h"
#include "adc.h"

void main(void) {
    int altitude = 0, mudancaGradual;
    int ultimoAlt[8] = {0, 1500, 3000, 4500, 6000, 7500, 9000, 10500};
    unsigned char leds[7] = {0x00,0x01,0x03,0x07,0x0F,0x2F,0xFF};
    char j=0, k=0, slot=0, op=0, pwmVal;
    unsigned int tecla = 16;
    
    lcdInit();
    kpInit();
    ssdInit();
    pwmInit();
    adcInit();
    
    TRISD = 0x00;
    PORTD = 0x00;
    
    //Informações iniciais do LCD
    lcdPosition(0,0);
    lcdString("Destino: Galeao");
    lcdPosition(1,0);
    lcdString("No chao");
    
    for(;;){
        switch(slot){
            case 0:
                kpDebounce();
                if (kpRead() != tecla){
                    tecla = kpRead();
                    //Mundaca de altitude
                    if (bitTst(tecla, 0)&& altitude <= 9500) { 
                        altitude+=500;
                    }
                    if (bitTst(tecla, 2) && altitude>0) { 
                        altitude-=500;
                    }
                    //Mudanca de visualização entre SSD e LEDs
                    if (bitTst(tecla, 1)){ 
                        op = 0;
                    }
                    if (bitTst(tecla, 3)) { 
                        op = 1;
                    }
                    //Ligar/Desligar led dependendo da altitude
                    if(altitude>=ultimoAlt[k+1]){
                        k++;
                    }else if(altitude<ultimoAlt[k]){
                        k--;
                    }
                    //Informações sobre a altitude atual
                    if(altitude>=6000&&altitude<7000){
                        lcdPosition(1,0);
                        lcdString("Voo de cruzeiro");
                        pwmVal = 0;
                        j=1;
                    }
                    if(altitude<6000 && altitude>1 && j<1){
                        lcdPosition(1,0);
                        lcdString("Decolando       ");
                    }
                    if(altitude<6000 && altitude>1 && j>0){
                        lcdPosition(1,0);
                        lcdString("Pousando        ");
                    }
                    if(altitude>7000 && altitude<10000){
                        lcdPosition(1,0);
                        lcdString("Muito alto      ");
                        pwmVal = 100;
                    }
                    if(altitude==0){
                        lcdPosition(1,0);
                        lcdString("No chao         ");
                        j=0;
                    }
                    if(altitude<0){
                        altitude = 0;
                    }
                    if(altitude>9999){
                        altitude = 9999;
                    }
                }
                slot = 1;
                break;
            case 1:
                //Altitude mostrada no ssd
                ssdDigit(((altitude/1)%10),3);
                ssdDigit(((altitude/10)%10),2);
                ssdDigit(((altitude/100)%10),1);
                ssdDigit(((altitude/1000)%10),0);
                slot = 0;
                break;
            default:
                slot = 0;
                break;
        }
        if(op==0){
            ssdUpdate();
        }else{
            PORTD = leds[k];
        }
        mudancaGradual = adcRead(0);
        if(altitude>7000){
            altitude -= mudancaGradual/100;
        }else{
            pwmVal = 0;
        }
        //Ativa o buzzer se o aviao estiver alto demais
        pwmSet(pwmVal);
        atraso_ms(20);
    }
}
