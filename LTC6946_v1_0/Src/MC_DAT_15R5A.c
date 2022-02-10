
#include "MC_DAT_15R5A.h"
#include "stm32f0xx_hal.h"


void SetAttenuator(uint8_t Power){
	
	uint8_t idx=0;
	
	LE_Low();
	
	// first clear B5 stop bit to 0
	Power &= ~(1 << 5);

	
	for (idx =0; idx < 6; ++idx){
		Clock_Low();
		Delay(smallWait);
		
		//check if that  each bit is set
		// there are 6 bits (stop bit plus 5 data bits) and I'm starting at the largest bit
		if ((Power & 0x20>>  idx)){
			Data_High();
		}
		else{
			Data_Low();
		}
		Delay(smallWait);
		Clock_High();		
		Delay(largeWait);		
	}
	
}
	
void Delay(uint32_t N)
{
uint32_t j;
    while(N--)
        for(j=0;j < MILLISEC;j++);
}

void LE_Low(void){
	 HAL_GPIO_WritePin(ANT_LE_GPIO_Port, ANT_LE_Pin, GPIO_PIN_RESET);
}

void LE_High(void){
	 HAL_GPIO_WritePin(ANT_LE_GPIO_Port, ANT_LE_Pin, GPIO_PIN_SET);
}


void Clock_Low(void){
	 HAL_GPIO_WritePin(ANT_CLK_GPIO_Port, ANT_CLK_Pin, GPIO_PIN_RESET);
}


void Clock_High(void){
	 HAL_GPIO_WritePin(ANT_CLK_GPIO_Port, ANT_CLK_Pin, GPIO_PIN_SET);
}

void Data_Low(void){
	 HAL_GPIO_WritePin(ANT_DATA_GPIO_Port, ANT_DATA_Pin, GPIO_PIN_RESET);
}


void Data_High(void){
	 HAL_GPIO_WritePin(ANT_DATA_GPIO_Port, ANT_DATA_Pin, GPIO_PIN_SET);
}
