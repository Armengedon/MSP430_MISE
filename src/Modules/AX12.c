/*
 * AX12.c
 *
 *  Created on: 18 may. 2019
 *      Author: Jordi
 */

// ------------------------------------- INCLUDES -------------------------------------

#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include <HAL/uart.h>
#include <HAL/timers.h>
#include <Modules/AX12.h>
#include <Modules/lcd.h>

// ------------------------------------- DEFINES --------------------------------------

#define statusPacket_timeout    0xFF
#define AX12_CWANGLE_ADDR       0x06
#define AX12_LED_ADDR           0x19
#define AX12_MOV_SPEED_ADDR     0x20
#define AX12_TEMP_ADDR          0x2B


// ------------------------------------- TYPEDEFS -------------------------------------

typedef struct RxReturn{
        byte statusPacket[4];
        byte error;
        byte parametres[16];
}RxReturn;

// ----------------------------------- PRIVATE VARS  ----------------------------------

// ----------------------------- PRIVATE METHODS PROTOTYPES ---------------------------

void Sentit_Dades_Rx(void);

void Sentit_Dades_Tx(void);

struct RxReturn RxPacket(void);

byte TxPacket(byte bID, byte bParameterLength, byte bInstruction, byte Parametros[16]);

void AX12_setAngle(void);

void AX12_readError(byte error);

// ----------------------------------- PRIVATE METHODS --------------------------------

void AX12_readError(byte error) {
    uint8_t buffer[16] = "@";

    lcd_clearDisplay();

    if (error & BIT0) {
        sprintf((char *) buffer, "%s%s", buffer, "Vin,");  //prepare char pointer to send to the lcd
        wait_ms(3);
    }
    if (error & BIT1) {
        sprintf((char *) buffer, "%s%s", buffer, "ºlim,");
        wait_ms(3);
    }
    if (error  & BIT2) {
        sprintf((char *) buffer, "%s%s", buffer, "ºC,");
        wait_ms(3);
    }
    if (error & BIT3) {
        sprintf((char *) buffer, "%s%s", buffer, "Range,");
        wait_ms(3);
    }
    if (error & BIT4) {
        sprintf((char *) buffer, "%s%s", buffer, "Chk,");
        wait_ms(3);
    }
    if (error & BIT5) {
        sprintf((char *) buffer, "%s%s", buffer, "Ovload,");
        wait_ms(3);
    }
    if (error  & BIT6) {
        sprintf((char *) buffer, "%s%s", buffer, "Ins,");
        wait_ms(3);
    }

    if (error & 0xFF) {
        sprintf((char *) buffer, "@AX12 TIMEOUT");
    }

    lcd_sendLine(buffer);
    while(1) {
        lcd_shiftLeft();
        wait_ms(200);
    }

}


/* funcions per canviar el sentit de les comunicacions */
void Sentit_Dades_Rx(void) {
    //Configuració del Half Duplex dels motors: Recepció
    P4OUT &= ~UART_DATA_DIR; //El pin P4.6 (DIRECTION_PORT) el posem a 0 (Rx)
}

void Sentit_Dades_Tx(void) {
    //Configuració del Half Duplex dels motors: Transmissió
    P4OUT |= UART_DATA_DIR; //El pin P4.6 (DIRECTION_PORT) el posem a 1 (Tx)
}

//Funcio per rebre el status packet
struct RxReturn RxPacket(void) {
    RxReturn respuesta = {{0}};             //Inicialitzem el struct tot a 0
    byte bCount, bLength;
    uint8_t Rx_time_out = 0;
    bLength = 0;
    Sentit_Dades_Rx();                      //Posem el sentit de les dades en recepcio
    for(bCount = 0; bCount<4; bCount++) {      //Primer agafem els 5 primers bytes(2 de inici de tram, ID i longitud i l'error
        timerA_timeout_start(80);
        while(!uart_isByte_Recibido()) {              //Mentre esperem les dades vigilem no superar el temps
            Rx_time_out = timers_isTimeout();     //En aquest cas hem volgut reduir el temps al màxim per buscar un equilibri
            if(Rx_time_out) break;           //Si hi ha timeout surt del bucle
        }
        if(Rx_time_out) break;               //Si hi ha timeout surt del bucle
        respuesta.statusPacket[bCount] = uart_getDatoLeido();    //guardem les dades rebudes al struct
    }

    if(!Rx_time_out) {
        timerA_timeout_start(50);
        while(!uart_isByte_Recibido()) {
            Rx_time_out = timers_isTimeout();
            if(Rx_time_out)break;
        }
        respuesta.error = uart_getDatoLeido();
    }

    bLength = respuesta.statusPacket[3] -1;
    if(!Rx_time_out){

        for(bCount = 0; bCount<bLength; bCount++){    //En aquest cas fem el mateix amb la resta de la trama
            timerA_timeout_start(50);
            while(!uart_isByte_Recibido()) {
                Rx_time_out = timers_isTimeout();
                if(Rx_time_out)break;
            }
            if(Rx_time_out)break;
            respuesta.parametres[bCount] = uart_getDatoLeido();
        }
    }
    if (Rx_time_out) respuesta.error = statusPacket_timeout;
    timerA_timeout_stop();
    return respuesta;
}

//TxPacket() 3 paràmetres: ID del Dynamixel, Mida dels paràmetres, Instruction byte. torna la mida del "Return packet"
byte TxPacket(byte bID, byte bParameterLength, byte bInstruction, byte Parametros[16]) {
    byte bCount, bCheckSum, bPacketLength;
    byte TxBuffer[32];
    Sentit_Dades_Tx(); //El pin P3.0 (DIRECTION_PORT) el posem a 1 (Transmetre)
    TxBuffer[0] = 0xff; //Primers 2 bytes que indiquen inici de trama FF, FF.
    TxBuffer[1] = 0xff;
    TxBuffer[2] = bID; //ID del mòdul al que volem enviar el missatge
    TxBuffer[3] = bParameterLength + 2; //Length(Parameter,Instruction,Checksum)
    TxBuffer[4] = bInstruction; //Instrucció que enviem al Mòdul
    for (bCount = 0; bCount < bParameterLength; bCount++) {//Comencem a generar la trama que hem d’enviar
        TxBuffer[bCount + 5] = Parametros[bCount];
    }
    bCheckSum = 0;
    bPacketLength = bParameterLength + 4 + 2;
    for (bCount = 2; bCount < bPacketLength - 1; bCount++) {//Càlcul del checksum
        bCheckSum += TxBuffer[bCount];
    }
    TxBuffer[bCount] = ~bCheckSum; //Escriu el Checksum (complement a 1)
    for (bCount = 0; bCount < bPacketLength; bCount++) { //Aquest bucle és el que envia la trama al Mòdul Robot
        uart_send_byte(TxBuffer[bCount]);
    }
    while ((UCA1STAT & UCBUSY)); //Espera fins que s’ha transmès el últim byte
    Sentit_Dades_Rx(); //Posem la línia de dades en Rx perquè el mòdul Dynamixel envia resposta
    return (bPacketLength);
}

void AX12_setAngle(void) {
    byte Parametros[16] = {AX12_CWANGLE_ADDR,0x00,0x00,0x00,0x00};      //Per fer un "gir infinit" hem de inicialitzar el CW i CCW a 0.
                                                                        //L'adreça dels angles ocupa 4 bytes, i per tant li enviem 1+4 parametres
    TxPacket(AX12_BROADCAST, 5, AX12_WRITE,Parametros);        //Fem un broadcast, els parametres son 5, la funcio es escriure al registre
}


// ----------------------------------- PUBLIC METHODS ---------------------------------

uint8_t AX12_readTemp(uint8_t moduleAddr) {
    RxReturn respuesta;
    byte Parametros[16] = {AX12_TEMP_ADDR ,  1};            //Preparem els parametres de entrada, addreça i quans bytes volem llegir
    TxPacket(moduleAddr, 2, AX12_READ ,Parametros);   //Enviem la trama (0xfe per enviarho a tots els motors), 2 pel numero de parametres, i 3 la instruccio write
    respuesta = RxPacket();                     //Agafem el status packet PERO demoment no el tractem
    if (respuesta.error){
        AX12_readError(respuesta.error);
    }
    return respuesta.parametres[0];
}

//Funcio per encendre els leds dels motors
void AX12_turnOnLeds(uint8_t moduleAddr) {
    RxReturn respuesta;
    byte Parametros[16] = {AX12_LED_ADDR ,  1};            //Preparem els parametres de entrada,
    TxPacket(moduleAddr, 2, AX12_READ ,Parametros);   //Enviem la trama (0xfe per enviarho a tots els motors), 2 pel numero de parametres, i 3 la instruccio write
    respuesta = RxPacket();                     //Agafem el status packet PERO demoment no el tractem
    if (respuesta.error){
        AX12_readError(respuesta.error);
    }
}

void AX12_turnOffLeds(uint8_t moduleAddr) {
    RxReturn respuesta;
    byte Parametros[16] = {AX12_LED_ADDR , 0};            //Preparem els parametres de entrada, 25 = 0x19 (adreça del led) i 1 per encendrel
    TxPacket(moduleAddr, 2, AX12_WRITE ,Parametros);   //Enviem la trama (0xfe per enviarho a tots els motors), 2 pel numero de parametres, i 3 la instruccio write
    respuesta = RxPacket();                     //Agafem el status packet PERO demoment no el tractem
    if (respuesta.error){
        AX12_readError(respuesta.error);
    }
}

void AX12_motorControl(uint8_t moduleAddr, uint16_t direction, uint16_t speed){
    RxReturn respuesta;
    byte Parametros[16];
    Parametros[0] = AX12_MOV_SPEED_ADDR;
    Parametros[1] = (uint8_t) speed;
    Parametros[2] = (speed >> 8) | direction;
    TxPacket(moduleAddr, 3, AX12_WRITE,Parametros);
    respuesta = RxPacket();
    if (AX12_BROADCAST != moduleAddr & respuesta.error){
        AX12_readError(respuesta.error);
    }
}

void AX12_init(void) {
    uart_init();
    AX12_setAngle();
}
