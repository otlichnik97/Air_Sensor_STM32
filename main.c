
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
	RCC->CR |= (1<<RCC_CR_HSEON_Pos); // запускаем кварцевый резонатор
	int StartUpCounter=0;
	  for(StartUpCounter=0; ; StartUpCounter++)
  {
    if(RCC->CR & (1<<RCC_CR_HSERDY_Pos)) // ждем готовности резонатора
      break;
    if(StartUpCounter > 0x1000)
    {
      RCC->CR &= ~(1<<RCC_CR_HSEON_Pos); // если время вышло, останавливаем резонатор
      return;
    }
  }
	 RCC->CFGR |= (0x05<<RCC_CFGR_PLLMULL_Pos) //PLL множитель равен 9
            | (0x01<<RCC_CFGR_PLLSRC_Pos); //Тактирование PLL от HSE
  
  RCC->CR |= (1<<RCC_CR_PLLON_Pos); //Запускаем PLL

  for(StartUpCounter=0; ; StartUpCounter++)
  {
    if(RCC->CR & (1<<RCC_CR_PLLRDY_Pos))
      break;
    
    if(StartUpCounter > 0x1000)
    {
      RCC->CR &= ~(1<<RCC_CR_HSEON_Pos); //Останавливаем HSE
      RCC->CR &= ~(1<<RCC_CR_PLLON_Pos); //Останавливаем PLL
      return;
    }
  }
  
  //Устанавливаем 2 цикла ожидания для Flash
  //так как частота ядра у нас будет 48 MHz < SYSCLK <= 72 MHz
  FLASH->ACR |= (0x02<<FLASH_ACR_LATENCY_Pos); 
  
  RCC->CFGR |= (0x00<<RCC_CFGR_PPRE2_Pos) //Делитель шины APB2 отключен
            | (0x04<<RCC_CFGR_PPRE1_Pos) //Делитель нишы APB1 равен 2
            | (0x00<<RCC_CFGR_HPRE_Pos); //Делитель AHB отключен
  
  RCC->CFGR |= (0x02<<RCC_CFGR_SW_Pos); //Переключаемся на работу от PLL
  
  while((RCC->CFGR & RCC_CFGR_SWS_Msk) != (0x02<<RCC_CFGR_SWS_Pos))  //Ждем, пока переключимся
  {
  }
  
  RCC->CR &= ~(1<<RCC_CR_HSION_Pos);
  
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









