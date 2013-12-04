#include"WarmVol.h"
#include<stm32f10x_dac.h>
#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

void rt_hw_dac_init(void)

{

	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

       DAC_InitTypeDef            DAC_InitStructure;

       DAC_DeInit();


      DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;

      DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;

      DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;

      DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = 0;

      DAC_Init(DAC_Channel_1, &DAC_InitStructure);


       DAC_Cmd(DAC_Channel_1, ENABLE);

     //  DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);


}

static void PlayVol(const unsigned char *buf ,unsigned short lenth)
{
	unsigned short i,n;
	for( i =0;i<lenth ;i++)
	{
		DAC_SetChannel1Data(DAC_Align_8b_R,buf[i]);
		for( n =0;n<150 ;n++);
		for( n =0;n<150 ;n++);
		for( n =0;n<150 ;n++);
	}

}
void playVolWarm1()
{
	PlayVol(Warm1,sizeof(Warm1));
}
void playVolWarm2()
{
	PlayVol(Warm2,sizeof(Warm2));
}

void playVolWarm3()
{
	PlayVol(Warm3,sizeof(Warm3));
}
void playVolWarm4()
{
	PlayVol(Warm4,sizeof(Warm4));
}


