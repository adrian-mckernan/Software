/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef MC_DAT_15R5A_H
#define MC_DAT_15R5A_H

//Truth table for attenuation
//Attenuation 	8 4 2 1 0.5
//Reference 		0 0 0 0 0
//0.5 (dB) 		0 0 0 0 1
//1 (dB) 			0 0 0 1 0
//2 (dB) 			0 0 1 0 0
//4 (dB) 			0 1 0 0 0
//8 (dB) 			1 0 0 0 0
//15.5 (dB) 		1 1 1 1 1


#include "main.h"
#include "stdint.h"

#define smallWait 2
#define largeWait 4
#define ATTN_0DBM 0
#define ATTN_HalfDBM 1
#define ATTN_1DBM 2
#define ATTN_2DBM 4
#define ATTN_4DBM 8
#define ATTN_8DBM 16


void SetAttenuator(uint8_t Power);

void LE_Low(void);

void LE_High(void);

void Clock_Low(void);

void Clock_High(void);

void Data_Low(void);

void Data_High(void);

void Delay(uint32_t);

#endif  // MC_DAT_15R5A_H
