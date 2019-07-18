//使用t2 定时10ms
void TIM2_int(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = 99; 	//10ms定时
	TIM_TimeBaseInitStructure.TIM_Prescaler=7199;  //10KHz 
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //??????
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化T2
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //T2中断控制
	TIM_Cmd(TIM2,ENABLE); //使能t2
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //中断号设置
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //主优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
}

//中断处理
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //
	{
		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断
}


