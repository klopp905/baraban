#include "delays.h"
#include "periph.h"
static volatile uint32_t TimingDelay, nTimingDelay;

extern void Error_Handler(void);

//==============================================================================
// Configure timer for Mks delays 
void Delays_init(TIM_HandleTypeDef *tim)
{
   //TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

   // TIM clock enable 
   //RCC_TIMMKS_PeriphClockCmd( ENABLE );
   //TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
   //TIM_TimeBaseStructure.TIM_Period = 65535;
   //TIM_TimeBaseInit(TIMMKS, &TIM_TimeBaseStructure);
   //TIM_Cmd(TIMMKS, ENABLE);
	
	//---Base Init--------------------------------------------------------------------------
	TIM_MKS_CLK_ENABLE();
	
  tim->Instance								= TIMMKS;
  tim->Init.Prescaler					= 0;
  tim->Init.CounterMode				= TIM_COUNTERMODE_UP;	
  tim->Init.Period						= 65535;
  tim->Init.ClockDivision			= TIM_CLOCKDIVISION_DIV1;
  tim->Init.RepetitionCounter	= 0;
  tim->Init.AutoReloadPreload	= TIM_AUTORELOAD_PRELOAD_ENABLE;
  if ( HAL_TIM_Base_Init( tim ) != HAL_OK ) { Error_Handler(); }
	
	__HAL_TIM_ENABLE(tim);
	//--------------------------------------------------------------------------------------	
	
}



//================================================================================================
void Mks(uint32_t time)
{

      uint16_t t0 =  TIMMKS->CNT;
      uint16_t us_count_tic =  time * (SystemCoreClock / 1000000);
      while ( (TIMMKS->CNT - t0) < us_count_tic) ;

}

//================================================================================================
/*void Ms(uint16_t time)
{
  uint16_t i;
  
  //time *= 2;
  for(i=0; i<time; i++) 
    Mks(1000);
}*/



//==============================================================================
void Ms(__IO uint32_t nTime)
{
  //TimingDelay = nTime;

  //while(TimingDelay != 0);
	while(nTime--)
	{
		Mks(1000);
	}
}



//================================================================================================
/*void nops(__IO uint32_t nTime)
{
  nTimingDelay = nTime;

  while(nTimingDelay != 0){
    asm("nop");
    nTimingDelay--;
  };
}



//==============================================================================
void nops(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0){
    asm("nop");
    TimingDelay--;
  };
}*/



//==============================================================================
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}


