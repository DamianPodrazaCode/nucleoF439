#include "main.h"
// adres start 0xC0000000 - adres stop 0xc1ffffff - 64Mb, 8MB, 4M half Word, 2M Word
#define SDRAM_BANK_ADDR ((uint32_t)0xC0000000)

void SystemClock_Config(void);
static void GPIO_Init(void);
static void MX_FMC_Init(void);

int main(void) {
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
	MX_FMC_Init();

	// czyszczenie całej pamięci SDRAM
	volatile uint32_t clearTimeSDRAM = HAL_GetTick();
	for (int i = 0; i < 0x2000000; i++)
		*(__IO uint8_t*) (SDRAM_BANK_ADDR + (1 * i)) = 0xa8;
	clearTimeSDRAM = HAL_GetTick() - clearTimeSDRAM;
	__NOP();

	// uzupełnianie pamięci ram
	uint8_t tab[0x100] = { 0 };
	volatile uint32_t timeRAM = HAL_GetTick();
	for (int j = 0; j < 0x10000; j++)
		for (int i = 0; i < 0x100; i++) {
			tab[i] = i;
		}
	timeRAM = HAL_GetTick() - timeRAM;
	__NOP();

	// uzupełnianie SDRAM
	volatile uint32_t timeSDRAM = HAL_GetTick();
	for (int j = 0; j < 0x10000; j++)
		for (int i = 0; i < 0x100; i++) {
			*(__IO uint8_t*) (SDRAM_BANK_ADDR + (1 * i)) = i; //bajt
			//	*(__IO uint16_t*) (SDRAM_BANK_ADDR + (2 * i)) = i; //pół słowo
			//	*(__IO uint32_t*) (SDRAM_BANK_ADDR + (4 * i)) = i; // słowo
		}
	timeSDRAM = HAL_GetTick() - timeSDRAM;
	__NOP();

	while (1) {
	}

}

void SystemClock_Config(void) {
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
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
}

SDRAM_HandleTypeDef hsdram1;
static void MX_FMC_Init(void) {
	hsdram1.Instance = FMC_SDRAM_DEVICE;
	hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
	hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
	hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
	hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
	hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
	hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
	hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
	hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
	hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
	hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
	FMC_SDRAM_TimingTypeDef SdramTiming = { 0 };
	SdramTiming.LoadToActiveDelay = 2;
	SdramTiming.ExitSelfRefreshDelay = 7;
	SdramTiming.SelfRefreshTime = 4;
	SdramTiming.RowCycleDelay = 6;
	SdramTiming.WriteRecoveryTime = 2;
	SdramTiming.RPDelay = 2;
	SdramTiming.RCDDelay = 2;
	HAL_SDRAM_Init(&hsdram1, &SdramTiming);

#define SDRAM_MODEREG_BURST_LENGTH_1            	((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2            	((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4            	((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8            	((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL     	((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED    	((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2             	((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3             	((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD   	((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED	((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE    	((uint16_t)0x0200)
	// SDRAM Initialization Sequence
	FMC_SDRAM_CommandTypeDef cmd;
	cmd.CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = 0;
	HAL_SDRAM_SendCommand(&hsdram1, &cmd, 0xFFFF);
	HAL_Delay(1);
	cmd.CommandMode = FMC_SDRAM_CMD_PALL;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition = 0;
	HAL_SDRAM_SendCommand(&hsdram1, &cmd, 0xFFFF);
	cmd.CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 8;
	cmd.ModeRegisterDefinition = 0;
	HAL_SDRAM_SendCommand(&hsdram1, &cmd, 0xFFFF);
	cmd.CommandMode = FMC_SDRAM_CMD_LOAD_MODE;
	cmd.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;
	cmd.AutoRefreshNumber = 1;
	cmd.ModeRegisterDefinition =
	SDRAM_MODEREG_BURST_LENGTH_1 |
	SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
	SDRAM_MODEREG_CAS_LATENCY_2 |
	SDRAM_MODEREG_OPERATING_MODE_STANDARD |
	SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
	HAL_SDRAM_SendCommand(&hsdram1, &cmd, 0xFFFF);
	HAL_SDRAM_ProgramRefreshRate(&hsdram1, 683);
}

static void GPIO_Init(void) {
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	__HAL_RCC_FMC_CLK_ENABLE();
	/** FMC GPIO Configuration
	 PF0   ------> FMC_A0
	 PF1   ------> FMC_A1
	 PF2   ------> FMC_A2
	 PF3   ------> FMC_A3
	 PF4   ------> FMC_A4
	 PF5   ------> FMC_A5
	 PC0   ------> FMC_SDNWE
	 PC2   ------> FMC_SDNE0
	 PC3   ------> FMC_SDCKE0
	 PF11   ------> FMC_SDNRAS
	 PF12   ------> FMC_A6
	 PF13   ------> FMC_A7
	 PF14   ------> FMC_A8
	 PF15   ------> FMC_A9
	 PG0   ------> FMC_A10
	 PG1   ------> FMC_A11
	 PE7   ------> FMC_D4
	 PE8   ------> FMC_D5
	 PE9   ------> FMC_D6
	 PE10   ------> FMC_D7
	 PE11   ------> FMC_D8
	 PE12   ------> FMC_D9
	 PE13   ------> FMC_D10
	 PE14   ------> FMC_D11
	 PE15   ------> FMC_D12
	 PD8   ------> FMC_D13
	 PD9   ------> FMC_D14
	 PD10   ------> FMC_D15
	 PD14   ------> FMC_D0
	 PD15   ------> FMC_D1
	 PG2   ------> FMC_A12
	 PG4   ------> FMC_BA0
	 PG5   ------> FMC_BA1
	 PG8   ------> FMC_SDCLK
	 PD0   ------> FMC_D2
	 PD1   ------> FMC_D3
	 PG15   ------> FMC_SDNCAS
	 PE0   ------> FMC_NBL0
	 PE1   ------> FMC_NBL1
	 */

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12
			| GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_2 | GPIO_PIN_3;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14
			| GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}