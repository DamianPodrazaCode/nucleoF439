#include "main.h"

void HAL_MspInit(void) {
	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
}

static void HAL_FMC_MspInit(void) {
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef *hsdram) {
	HAL_FMC_MspInit();
}

