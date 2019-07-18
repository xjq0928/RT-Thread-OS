//ʹ��t2 ��ʱ10ms
void TIM2_int(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///ʹ��ʱ��
	
    TIM_TimeBaseInitStructure.TIM_Period = 99; 	//10ms��ʱ
	TIM_TimeBaseInitStructure.TIM_Prescaler=7199;  //10KHz 
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//��ʼ��T2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //T2�жϿ���
	TIM_Cmd(TIM2,ENABLE); //ʹ��t2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //�жϺ�����
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
}

//�жϴ���
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //
	{
		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //����ж�
}


