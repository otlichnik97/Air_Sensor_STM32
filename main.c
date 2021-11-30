
// Program AirSensor 
// 22.11.2021
// Sh. Alexander

//--------------------------------------------------------------------------------------------
// Includes
//--------------------------------------------------------------------------------------------

#include "stm32f1xx.h"                  // Device header

//--------------------------------------------------------------------------------------------
// Defines
//--------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------
// Prototypes Func
//--------------------------------------------------------------------------------------------

void InitRCC(void);
void InitGPIO(void);
void Delay(uint32_t time);

//--------------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------------

uint8_t Error_Flag = 0;

//--------------------------------------------------------------------------------------------
// Main programm
//--------------------------------------------------------------------------------------------

int main()
{
	InitRCC();
	InitGPIO();
	
	while(1) {}
	return 0;
		
}

//--------------------------------------------------------------------------------------------
// Functions
//--------------------------------------------------------------------------------------------

void InitRCC(void) 
{
	uint32_t StartUpCounter=0;	
	RCC->CR |= RCC_CR_HSEON; 																	// Start enternal oscillator
	  for(StartUpCounter=0;;StartUpCounter++)
		{
			if (RCC->CR & RCC_CR_HSERDY) 													// Wait for HSE ready 
				break;
			if (StartUpCounter > 1000U)
			{
				RCC->CR &= ~RCC_CR_HSEON; 													// Stop HSE if it is not ready
				return;
			}
		}
	RCC->CFGR |= RCC_CFGR_PLLMULL9 | RCC_CFGR_PLLSRC; 				// PLL multipler is 9 and HSE is clock source
  RCC->CR |= RCC_CR_PLLON; 																	// Start PLL

  for(StartUpCounter=0;;StartUpCounter++)
  {
    if (RCC->CR & RCC_CR_PLLRDY) 														// Wait for PLL ready 
      break;
    
    if (StartUpCounter > 1000U)
    {
      RCC->CR &= ~(RCC_CR_HSEON | RCC_CR_PLLON); 						// Stop HSE and PLL if are not ready							
      return;
    }
  }
  
  FLASH->ACR |= FLASH_ACR_LATENCY_1; 												// Set 2 Flash cycles for SYSCLK = 72 MHz
  
  RCC->CFGR &= ~(RCC_CFGR_PPRE2_Msk | RCC_CFGR_HPRE_Msk); 	// Disable APB2 div and AHB div
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; 												// Prescaler APB1 is 2
           
  RCC->CFGR |= RCC_CFGR_SW_PLL; 														//Select PLL as system clock 
  
  while((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL) // Wait for switch to PLL
		{}
  RCC->CR &= ~RCC_CR_HSION;											
  
  return;
}

void Delay(uint32_t time)
{
	SysTick->LOAD = time;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) != (SysTick_CTRL_COUNTFLAG_Msk))
	{}
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->CTRL &= ~SysTick_CTRL_COUNTFLAG_Msk;
}

void InitGPIO(void)
{

}









