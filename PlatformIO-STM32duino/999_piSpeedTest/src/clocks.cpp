#include <Arduino.h>

#define _clock168MHz
//#define _clock180MHz

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_clock168MHz) 
    // zegar na 168MHz dla USB 48MHz
    void SystemClock_Config(void) {
        __HAL_RCC_PWR_CLK_ENABLE();
        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
        RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM = 8;
        RCC_OscInitStruct.PLL.PLLN = 336;
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
        RCC_OscInitStruct.PLL.PLLQ = 7;
        HAL_RCC_OscConfig(&RCC_OscInitStruct);
        HAL_PWREx_EnableOverDrive();
        RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
        RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
        HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
        __HAL_RCC_CCMDATARAMEN_CLK_ENABLE(); /* Ensure CCM RAM clock is enabled */
        HAL_RCC_EnableCSS();
        SystemCoreClock = 168000000UL;
        HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq));
    }
#endif

#if defined(_clock180MHz) 
    // zegar na 180MHz, bez USB
    void SystemClock_Config(void) {
        __HAL_RCC_PWR_CLK_ENABLE();
        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
        RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM = 4;
        RCC_OscInitStruct.PLL.PLLN = 180;
        RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
        RCC_OscInitStruct.PLL.PLLQ = 4;
        HAL_RCC_OscConfig(&RCC_OscInitStruct);
        HAL_PWREx_EnableOverDrive();
        RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
        HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
        __HAL_RCC_CCMDATARAMEN_CLK_ENABLE(); /* Ensure CCM RAM clock is enabled */
        HAL_RCC_EnableCSS();
        SystemCoreClock = 180000000UL;
        HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq));
    }
#endif

#ifdef __cplusplus
}
#endif
