#include "ADC.h"
#include "math.h"

/**
 * @brief MQ2 和 光照 传感器的 ADC 初始化
 */
void ADC_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                  // 修改：禁用多通道扫描
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;           // 单次转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;                       // 修改：每次只读一个通道
    ADC_Init(ADC1, &ADC_InitStructure);

    // 初始配置顺序（无实质影响）
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_55Cycles5);

    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}

/**
 * @brief 获取指定 ADC 通道值，预采样一次清洗残影，确保稳定
 * @param ch ADC_Channel_x 宏定义，如 ADC_Channel_8
 * @return uint16_t 稳定ADC数值
 */
uint16_t ADC_GetStableValue(uint8_t ch)
{
    const uint8_t sample_times = 8;  // 建议4~16次采样平均
    uint32_t sum = 0;

    // 每次采样都先清洗残留
    for (uint8_t i = 0; i < sample_times; i++)
    {
        // 修改：每次都配置当前通道
        ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_55Cycles5);

        // 清洗残影
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        (void)ADC_GetConversionValue(ADC1);

        // 正式采样
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        sum += ADC_GetConversionValue(ADC1);
    }

    return (uint16_t)(sum / sample_times);  // 返回平均值
}

/**
 * @brief 获取 MQ2 模块 ADC 原始值（PB0）
 */
uint16_t mq2_get_adc_value(void)
{
    return ADC_GetStableValue(ADC_Channel_8);
}

/**
 * @brief 获取 光照模块 ADC 原始值（PB1）
 */
uint16_t light_get_adc_value(void)
{
    return ADC_GetStableValue(ADC_Channel_9);
}

/**
 * @brief 获取 MQ2 模块电压
 */
float mq2_get_voltage(void)
{
    uint16_t adc_val = mq2_get_adc_value();
    return ((float)adc_val) * (3.3f / 4096.0f);
}

/**
 * @brief 获取 光照模块电压
 */
float light_get_voltage(void)
{
    uint16_t adc_val = light_get_adc_value();
    return ((float)adc_val) * (3.3f / 4096.0f);
}

/**
 * @brief 获取 MQ2 模块的 ppm 浓度
 * @note 适用于通风环境，基于对Rs的估算
 */
float mq2_get_ppm(void)
{
    float voltage = mq2_get_voltage();
    float rs = (5.0f - voltage) / voltage * 0.5f;
    float ppm = powf(11.5428f * 6.64f / rs, 0.6549f);
    return ppm;
}

/**
 * @brief 获取 TEMT6000 光照传感器的 LUX 值
 * @note 该换算基于经验估算（线性），适合大致环境判断，后续可校准
 */
float light_get_lux(void)
{
    float voltage = light_get_voltage();
    float lux = voltage * 1000.0f; // 简单线性估算
    return lux;
}
