/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @brief   Main program 
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V. 
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************  
  */
  
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "cmsis_os.h"
#include "MY_FLASH.h"
#include "mbedtls/sha256.h"
#include "flash_if.h"
/* FreeRTOS buffer for static allocation */
int c=0;
#if (configAPPLICATION_ALLOCATED_HEAP == 1)
uint8_t ucHeap[ configTOTAL_HEAP_SIZE ];
#endif
static void SystemClock_Config(void);
static void RNG_Init(void);
static void MainThread(void const * argument);
void myStartupFun (void) __attribute__ ((constructor));
#define read_size 98304
//static void Watchdog(void const * argument);
//static void Flash(void const * argument);
unsigned char sha256_out[32];
unsigned char sha256_out1[32];

int status =0;
RNG_HandleTypeDef RngHandle;
//#define ACCESS_PERMISSION
/*
void myStartupFun (void){
//start write
	//uint32_t  ramsource;
	//ramsource =(uint32_t) &data[0];
	//uint8_t aPacketData[]={0x66,0x66, 0x66,0x66};

	//ramsource= (uint32_t) & aPacketData ;
	  //FLASH_If_Erase((uint32_t)0x08040000);

//FLASH_If_Write((__IO uint32_t)0x08040000, (uint32_t*) ramsource, (uint32_t) 1 );

	//end write

	static unsigned char rData1[read_size];
	const unsigned char *HashBuffer2;
	MY_FLASH_SetSectorAddrs(0, 0x080001c0);

	 MY_FLASH_ReadN(0,rData1,read_size,DATA_TYPE_8);

		  HashBuffer2 = rData1;
		 mbedtls_sha256(HashBuffer2, read_size, sha256_out1, 0);
}
*/
int main()
{
#ifdef USE_LCD
  uint8_t  sdram_status = SDRAM_OK;
#endif

  HAL_Init();

  /* Configure the system clock to 178 MHz */
  SystemClock_Config();

  /* Initialize BSP Led for LED1 and LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);
  RNG_Init();

  /* Set MPU regions */
 	  	MPU_Config();



  BSP_PB_Init(BUTTON_TAMPER, BUTTON_MODE_EXTI);


//		 FLASH_If_Write start

uint32_t  ramsource;
//ramsource =(uint32_t) &data[0];
uint8_t aPacketData[]={0x66,0x66, 0x66,0x66};

ramsource= (uint32_t) & aPacketData ;

//To write to memory
//FLASH_If_Erase((uint32_t)0x08040000);

//FLASH_If_Write((__IO uint32_t)0x08040000, (uint32_t*) ramsource, (uint32_t) 1 );


//			 FLASH_If_Write end





 BSP_LED_Toggle(LED1);


#ifdef USE_LCD

  /* Initialize the LCD */
 /* Initialize the SDRAM */
  sdram_status = BSP_SDRAM_Init();
  if(sdram_status != SDRAM_OK)
  {
          Error_Handler();
  }

  /* Initialize LCD in landscape mode in DSI mode video burst */
  /* Initialize and start the LCD display in mode 'lcd_mode'
   *  Using LCD_FB_START_ADDRESS as frame buffer displayed contents.
   *  This buffer is modified by the BSP (draw fonts, objects depending on BSP calls).
   */

  /* Set Portrait orientation if needed, by default orientation is set to
     Landscape */

  /* Initialize DSI LCD */
  BSP_LCD_Init(); /* Uncomment if default config (landscape orientation) is needed */

  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);

  /* Set LCD Foreground Layer as active one */
  BSP_LCD_SelectLayer(1);

  LCD_LOG_Init();
  LCD_LOG_SetHeader((uint8_t *)("SSL Server Application"));

#endif /* USE_LCD */

   /* Init thread */
  osThreadDef(Start, MainThread, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE * 2);
  osThreadCreate (osThread(Start), NULL);

 // osThreadDef(FlashIAP, Flash, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE * 2);
 // osThreadCreate (osThread(FlashIAP), NULL);


  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  Error_Handler();

  
}

/**
  * @brief  Start Thread 
  * @param  argument not used
  * @retval None
  */
//static void Flash(void const * argument)
//{	  //define 0x08000000 as sector 0
	  //MY_FLASH_SetSectorAddrs(0, 0x08000000);
	  //static unsigned char rData[read_size];
	  //static unsigned char wData[6]= {0x55,0x55,0x85,0x55,0x45,0x55};
	  //const unsigned char *HashBuffer;
	  //static size_t len;


	  //MY_FLASH_WriteN(50,wData,6,DATA_TYPE_8);




	 // HashBuffer = rData;
	 //len = strlen((char*)HashBuffer);

	 // while (1) {
	//	BSP_LED_Toggle(LED4);
		// osDelay(500);
		// MY_FLASH_ReadN(0,rData,read_size,DATA_TYPE_8);
		// HashBuffer = rData;
		// mbedtls_sha256(HashBuffer, read_size, sha256_out, 0);

	 // }

//}

static void MainThread(void const * argument)
{ 
  UNUSED(argument);
#ifdef USE_LCD
  LCD_UsrLog("\r\n Starting Main Thread...\n");
#endif

  /* Start SSL Client task : Connect to SSL server and provide the SSL handshake protocol */
  osThreadDef(Server, SSL_Server, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE * 20);
  osThreadCreate(osThread(Server), NULL);

  osThreadDef(Watchdogtimer, Watchdog, osPriorityRealtime, 0, configMINIMAL_STACK_SIZE * 20);
  osThreadCreate(osThread(Watchdogtimer), NULL);


  for( ;; )
  {
    /* Delete the start Thread */ 
    osThreadTerminate(NULL);
  }
}

static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  
  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
 
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
  clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported  */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}
            
/* RNG init function */

static void RNG_Init(void)
{
  RngHandle.Instance = RNG;
 /* DeInitialize the RNG peripheral */
  if (HAL_RNG_DeInit(&RngHandle) != HAL_OK)
  {
    /* DeInitialization Error */
    Error_Handler();
  }    

  /* Initialize the RNG peripheral */
  if (HAL_RNG_Init(&RngHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

void Error_Handler(void)
{
  while (1)
  {
    BSP_LED_Toggle(LED3);
    osDelay(100);
  }
}

void Success_Handler(void)
{
  while (1)
  {
    BSP_LED_Toggle(LED1);
    osDelay(200);
  }
}
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
  /* WWDG Peripheral clock enable */
  __HAL_RCC_WWDG_CLK_ENABLE();
}
