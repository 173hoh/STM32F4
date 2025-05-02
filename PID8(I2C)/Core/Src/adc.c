#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_adc.h"
#include "adc.h"

ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

void MX_ADC1_Init(void) {
    ADC_ChannelConfTypeDef sConfig = {0};

		__HAL_RCC_ADC1_CLK_ENABLE();
		
    hadc1.Instance = ADC1;
    hadc1.Init.ContinuousConvMode = DISABLE;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.ScanConvMode = ENABLE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START; // Èí¼þ´¥·¢
    hadc1.Init.NbrOfConversion = 2;
    hadc1.Init.DiscontinuousConvMode = DISABLE;

    if (HAL_ADC_Init(&hadc1) != HAL_OK) {
        Error_Handler();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_5| GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void MX_ADC2_Init(void) {
    ADC_ChannelConfTypeDef sConfig = {0};

    __HAL_RCC_ADC2_CLK_ENABLE();

    hadc2.Instance = ADC2;
    hadc2.Init.ContinuousConvMode = DISABLE;
    hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc2.Init.ScanConvMode = DISABLE;
    hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc2.Init.NbrOfConversion = 2;
    hadc2.Init.DiscontinuousConvMode = DISABLE;

    if (HAL_ADC_Init(&hadc2) != HAL_OK) {
        Error_Handler();
    }
		__HAL_RCC_GPIOA_CLK_ENABLE();
    
		GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin =GPIO_PIN_3| GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}