#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM
#include "stm32f10x_adc.h"              // Keil::Device:StdPeriph Drivers:ADC
#include "stm32f10x_dac.h"              // Keil::Device:StdPeriph Drivers:DAC

uint16_t adcValue = 0;
uint16_t adc_to_pwm = 0;

void  gpioConfig(){
	
      GPIO_InitTypeDef GPIOInitStructure;
	
	    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE); 
	    
	    //led
	    GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF_PP;               
	    GPIOInitStructure.GPIO_Pin = GPIO_Pin_4;      
	    GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	    GPIO_Init(GPIOA,&GPIOInitStructure);
	
	    //button
	    GPIOInitStructure.GPIO_Mode = GPIO_Mode_AIN;               
	    GPIOInitStructure.GPIO_Pin = GPIO_Pin_4;      
	    GPIO_Init(GPIOA,&GPIOInitStructure);
	

}

void timerConfig(){
  
      TIM_TimeBaseInitTypeDef TIMERInitStructure;
	
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	    TIMERInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;                   
	    TIMERInitStructure.TIM_CounterMode = TIM_CounterMode_Up;    
	    TIMERInitStructure.TIM_Period = 2399;                       
	    TIMERInitStructure.TIM_Prescaler = 10;                    
	    TIMERInitStructure.TIM_RepetitionCounter = 0;
	
	    TIM_TimeBaseInit(TIM2,&TIMERInitStructure);
      TIM_Cmd(TIM2,ENABLE);
	
}

void pwmConfig(uint32_t timPulse){

      TIM_OCInitTypeDef TIMER_OCInitStructure;
	    
	    TIMER_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                  //PWM1 %75 high ile baslar , PWM2 ise %25 high ile baslar . 
      TIMER_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High;         //yukardakiyle ayni mantik biz yüksek baslamasini sagladik.
      TIMER_OCInitStructure.TIM_OutputState =  TIM_OutputState_Enable;
	    TIMER_OCInitStructure.TIM_Pulse = timPulse;
	
	    TIM_OC1Init(TIM2,&TIMER_OCInitStructure);
	    TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable);
	
}

void adcConfig(){

  ADC_InitTypeDef ADCInitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);                //pot ve led clock aktif

  ADCInitStructure.ADC_ContinuousConvMode = ENABLE;
	ADCInitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADCInitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADCInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADCInitStructure.ADC_NbrOfChannel = 1;
	ADCInitStructure.ADC_ScanConvMode = DISABLE;
	
	ADC_Init(ADC1,&ADCInitStructure);
	ADC_Cmd(ADC1,ENABLE);
	
}

uint16_t readADC(){

   ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);

	 ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	 while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
	
   return ADC_GetConversionValue(ADC1);
		
}

void dacConfig(){
    
  DAC_InitTypeDef DACInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC,ENABLE);
	
	DACInitStructure.DAC_LFSRUnmask_TriangleAmplitude = DISABLE;      
	DACInitStructure.DAC_OutputBuffer = ENABLE;                         //empedansi düsürmek için kullaniliyor
	DACInitStructure.DAC_Trigger = DAC_Trigger_None;
	DACInitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  
	DAC_Init(DAC_Channel_1,&DACInitStructure);
		
	DAC_Cmd(DAC_Channel_1,ENABLE);
	
}

void delay(uint32_t time){
 
   while(time--);

}

float map(float adcValue, float max, float min, float conMax, float conMin){

     return adcValue*((conMax-conMin)/(max-min));

}


int main(){
	
	  gpioConfig();
	  timerConfig();
	  adcConfig();
	  readADC();
	  dacConfig();

    while(1){
			
		adcValue = readADC();

		pwmConfig(2399);
		delay(3600);  	  
				
		
				
		}

}
	


