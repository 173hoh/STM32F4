    #include "usart2.h"
		#include "pid.h"
    void USART2_Config(uint32_t MyBaudRate)
    {
            GPIO_InitTypeDef GPIO_InitStructure;                         
            USART_InitTypeDef USART_InitStructure;
            NVIC_InitTypeDef NVIC_InitStructure;
            
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE); 
            
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;             
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
            GPIO_Init(GPIOA , &GPIO_InitStructure);
            GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;               
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
            GPIO_Init(GPIOA , &GPIO_InitStructure);
            
            USART_InitStructure.USART_BaudRate = MyBaudRate;           
            USART_InitStructure.USART_WordLength = USART_WordLength_8b;
            USART_InitStructure.USART_StopBits = USART_StopBits_1;
            USART_InitStructure.USART_Parity = USART_Parity_No;
            USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
            USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
            USART_Init(USART2,&USART_InitStructure);                   
            
            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;       
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;       
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         
            NVIC_Init(&NVIC_InitStructure);                         
            
            USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);           
            USART_Cmd(USART2,ENABLE);
    }
    
    void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
    {
            USART_SendData(pUSARTx,ch);              
            while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);        
    }
  
    void Usart_SendArray( USART_TypeDef * pUSARTx, uint8_t *array, uint16_t num)
    {
            uint8_t i;        
            for(i=0; i<num; i++)
            {
                    Usart_SendByte(pUSARTx,array[i]);         
            }                        
            while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
    }

    void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
    {
            unsigned int k=0;
            do 
            {
                    Usart_SendByte( pUSARTx, *(str + k) );
                    k++;
            } while(*(str + k)!='\0');
            while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);             
    }
 
    void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
    {
            uint8_t temp_h, temp_l;        
            temp_h = (ch&0XFF00)>>8;          
            temp_l = ch&0XFF;                             
            USART_SendData(pUSARTx,temp_h);         
            while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);        
            USART_SendData(pUSARTx,temp_l);        
            while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);        
    }

    
    void USART2_IRQHandler(void)
    {
            uint8_t ucTemp,a[3],i=0;
            if(USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET)
            {       if(i==0)    
                    {
											a[0]= USART_ReceiveData(USART2);
											if(a[0]==0)
											i=2;
										}
									else if(i==2)
										{
										a[1]= USART_ReceiveData(USART2);
											if(a[1]=='A')
												Kd=Kd+0.01;
											else if(a[1]=='B')
												Kd=Kd-0.01;
											
										i=3;
										}
										
										else if(i==3)
										{
										a[2]= USART_ReceiveData(USART2);
											if(a[2]==9)
										i=0;
										}								

            }         
    }
    
    