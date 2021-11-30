
#include "stm32f1xx.h"
#include "SPI.h"



void SPI1_Init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;				    // Enable clock of SPI1.
	SPI1->CR1 = 0;
	SPI1->CR2 = 0;
	SPI1->CR1 |= SPI_CR1_BR_1;     								// Clock is divide by 8.
	SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;			  // Ignore NSS pin IO value.
	
	SPI1->CR1 |= SPI_CR1_MSTR | SPI_CR1_SPE;			// Master mode, SPI enable.
}

void SPI2_Init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;				    // Enable clock of SPI2.
	SPI2->CR1 = 0;
	SPI2->CR2 = 0;
	SPI2->CR1 |= SPI_CR1_BR_1;     								// Clock is divide by 8.
	SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;			  // Ignore NSS pin IO value.
	
	SPI2->CR1 |= SPI_CR1_MSTR | SPI_CR1_SPE;			// Master mode, SPI enable.		
}
