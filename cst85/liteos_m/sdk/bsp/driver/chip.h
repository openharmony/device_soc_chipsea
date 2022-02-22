/*
 * Copyright (c) 2022 CHIPSEA Co., Ltd. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
/**
 ****************************************************************************************
 *
 * @file chip.h
 *
 ****************************************************************************************
 */

#ifndef _CHIP_H_
#define _CHIP_H_

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

typedef enum IRQn
{
/******  Cortex-M4 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  MemoryManagement_IRQn         = -12,      /*!< 4 Memory Management Interrupt                    */
  BusFault_IRQn                 = -11,      /*!< 5 Bus Fault Interrupt                            */
  UsageFault_IRQn               = -10,      /*!< 6 Usage Fault Interrupt                          */
  SVCall_IRQn                   = -5,       /*!< 11 SV Call Interrupt                             */
  DebugMonitor_IRQn             = -4,       /*!< 12 Debug Monitor Interrupt                       */
  PendSV_IRQn                   = -2,       /*!< 14 Pend SV Interrupt                             */
  SysTick_IRQn                  = -1,       /*!< 15 System Tick Interrupt                         */

/******  Chip Specific Interrupt Numbers **********************************************************/
  BTAON_IRQn                    = 0,        /*!< BT AON Interrupt                                 */
  WCNAON_IRQn                   = 1,        /*!< WCN AON Interrupt                                */
  WGEN_IRQn                     = 2,        /*!< WiFi gen Interrupt                               */
  WPROTTRIG_IRQn                = 3,        /*!< WiFi prottrigger Interrupt                       */
  WTXTRIG_IRQn                  = 4,        /*!< WiFi txtrigger Interrupt                         */
  WRXTRIG_IRQn                  = 5,        /*!< WiFi rxtrigger Interrupt                         */
  WTXRXMISC_IRQn                = 6,        /*!< WiFi txrxmisc Interrupt                          */
  WTXRXTIM_IRQn                 = 7,        /*!< WiFi txrtimer Interrupt                          */
  WPHY_IRQn                     = 8,        /*!< WiFi PHY Interrupt                               */
  BLE_IRQn                      = 9,        /*!< BLE Interrupt                                    */
  BT_IRQn                       = 10,       /*!< BT Interrupt                                     */
  BTDM_IRQn                     = 11,       /*!< BTDM Interrupt                                   */
  FREQ_IRQn                     = 12,       /*!< FREQ Interrupt                                   */
  DMACOMB_IRQn                  = 13,       /*!< DMA Comb Interrupt                               */
  SDIO_IRQn                     = 14,       /*!< SDIO Interrupt                                   */
  USBDMA_IRQn                   = 15,       /*!< USB DMA Interrupt                                */
  UART0_IRQn                    = 16,       /*!< UART Interrupt                                   */
  UART1_IRQn                    = 17,       /*!< UART Interrupt                                   */
  UART2_IRQn                    = 18,       /*!< UART Interrupt                                   */
  MCU2WCN0_IRQn                 = 19,       /*!< MCU2WCN Interrupt                                */
  MCU2WCN1_IRQn                 = 20,       /*!< MCU2WCN Interrupt                                */
  WCN2MCU0_IRQn                 = 21,       /*!< WCN2MCU Interrupt                                */
  WCN2MCU1_IRQn                 = 22,       /*!< WCN2MCU Interrupt                                */
  MCU2WCN0_G1_IRQn              = 23,       /*!< MCU2WCN Interrupt                                */
  MCU2WCN1_G1_IRQn              = 24,       /*!< MCU2WCN Interrupt                                */
  WCN2MCU0_G1_IRQn              = 25,       /*!< WCN2MCU Interrupt                                */
  WCN2MCU1_G1_IRQn              = 26,       /*!< WCN2MCU Interrupt                                */
  TIMER20_IRQn                  = 27,       /*!< Timer Interrupt                                  */
  TIMER21_IRQn                  = 28,       /*!< Timer Interrupt                                  */
  TIMER22_IRQn                  = 29,       /*!< Timer Interrupt                                  */
  WDT3_IRQn                     = 30,       /*!< Camera Interrupt                                 */
  ASDMA_COMMON_IRQn             = 31,       /*!< ASDMA Common Interrupt                           */
  ASDMA_GE_SW_IRQn              = 32,       /*!< ASDMA Generic SW Interrupt                       */
  ASDMA_AUDIO_IRQn              = 33,       /*!< ASDMA Audio Channel Interrupt                    */
  ASDMA_GE_HW_IRQn              = 34,       /*!< ASDMA Generic HW Interrupt                       */
  ASDMA_DMAC_IRQn               = 35,       /*!< ASDMA Dmac Interrupt                             */
  AUD_PROC_IRQn                 = 36,       /*!< Audio Process Interrupt                          */
  DMA00_IRQn                    = 37,       /*!< DMA Interrupt                                    */
  DMA01_IRQn                    = 38,       /*!< DMA Interrupt                                    */
  DMA02_IRQn                    = 39,       /*!< DMA Interrupt                                    */
  DMA03_IRQn                    = 40,       /*!< DMA Interrupt                                    */
  DMA04_IRQn                    = 41,       /*!< DMA Interrupt                                    */
  DMA05_IRQn                    = 42,       /*!< DMA Interrupt                                    */
  DMA06_IRQn                    = 43,       /*!< DMA Interrupt                                    */
  DMA07_IRQn                    = 44,       /*!< DMA Interrupt                                    */
  DMA08_IRQn                    = 45,       /*!< DMA Interrupt                                    */
  DMA09_IRQn                    = 46,       /*!< DMA Interrupt                                    */
  DMA10_IRQn                    = 47,       /*!< DMA Interrupt                                    */
  DMA11_IRQn                    = 48,       /*!< DMA Interrupt                                    */
  DMA12_IRQn                    = 49,       /*!< DMA Interrupt                                    */
  DMA13_IRQn                    = 50,       /*!< DMA Interrupt                                    */
  MCU2WCN2_IRQn                 = 51,       /*!< MCU2WCN Interrupt                                */
  MCU2WCN3_IRQn                 = 52,       /*!< MCU2WCN Interrupt                                */
  WCN2MCU2_IRQn                 = 53,       /*!< WCN2MCU Interrupt                                */
  WCN2MCU3_IRQn                 = 54,       /*!< WCN2MCU Interrupt                                */
  MCU2WCN2_G1_IRQn              = 55,       /*!< MCU2WCN Interrupt                                */
  MCU2WCN3_G1_IRQn              = 56,       /*!< MCU2WCN Interrupt                                */
  WCN2MCU2_G1_IRQn              = 57,       /*!< WCN2MCU Interrupt                                */
  WCN2MCU3_G1_IRQn              = 58,       /*!< WCN2MCU Interrupt                                */
  TIMER00_IRQn                  = 59,       /*!< Timer Interrupt                                  */
  TIMER01_IRQn                  = 60,       /*!< Timer Interrupt                                  */
  TIMER02_IRQn                  = 61,       /*!< Timer Interrupt                                  */
  TIMER10_IRQn                  = 62,       /*!< Timer Interrupt                                  */
  TIMER11_IRQn                  = 63,       /*!< Timer Interrupt                                  */
  TIMER12_IRQn                  = 64,       /*!< Timer Interrupt                                  */
  GPIO_IRQn                     = 65,       /*!< GPIO Interrupt                                   */
  WDT0_IRQn                     = 66,       /*!< WDT Interrupt                                    */
  WDT1_IRQn                     = 67,       /*!< WDT Interrupt                                    */
  WDT2_IRQn                     = 68,       /*!< WDT Interrupt                                    */
  SPI_IRQn                      = 69,       /*!< SPI Interrupt                                    */
  TRNG_IRQn                     = 70,       /*!< TRNG Interrupt                                   */
  I2CM_IRQn                     = 71,       /*!< I2C Master Interrupt                             */
  I2CS_IRQn                     = 72,       /*!< I2C Slave Interrupt                              */
  CACHE0_IRQn                   = 73,       /*!< Cache Interrupt                                  */
  CACHE1_IRQn                   = 74,       /*!< Cache Interrupt                                  */
  PSRAM_IRQn                    = 75,       /*!< PSRAM Interrupt                                  */
  SDMMC_IRQn                    = 76,       /*!< SDMMC Interrupt                                  */
  PSIM_IRQn                     = 77,       /*!< PSI Master Interrupt                             */
  PWRCTRL_IRQn                  = 78,       /*!< Power Control Interrupt                          */
  PMIC_IRQn                     = 79,       /*!< PMIC Interrupt                                   */
  NVIC_USER_IRQ_NUMBER                      /*!< User Interrupt Number                            */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M4 Processor and Core Peripherals */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __FPU_PRESENT             1         /*!< FPU present or not                               */
#define __NVIC_PRIO_BITS          6         /*!< Number of Bits used for Priority Levels          */
#define __NVIC_PRIO_LOWEST        0x3F      /*!< The lowest NVIC priority                         */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */


#include "core_cm4.h"                       /* Cortex-M4 processor and core peripherals           */


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
#define CS_ROM_BASE          (0x00000000UL)
#define CS_PMIC_RAM_BASE     (0x01000000UL)
#define CS_FLASH_MEM_BASE    (0x04000000UL)
#define CS_PSRAM_MEM_BASE    (0x06000000UL)
#define CS_CACHE0_MEM_BASE   (0x08000000UL)
#define CS_CACHE1_MEM_BASE   (0x0C000000UL)
#define CS_AHB0_BASE         (0x40000000UL)
#define CS_APB0_BASE         (0x40100000UL)
#define CS_AHB2_BASE         (0x40200000UL)
#define CS_WLANREG_BASE      (0x40300000UL)
#define CS_AHB1_BASE         (0x40500000UL)
#define CS_APB1_BASE         (CS_AHB1_BASE)
#define CS_BTREG_BASE        (0x40600000UL)
#define CS_PMIC_REG_BASE     (0x50000000UL)

/* AHB0 peripherals                                                           */
#define CS_CACHE0_BASE       (CS_AHB0_BASE + 0x00000)
#define CS_CACHE1_BASE       (CS_AHB0_BASE + 0x20000)
#define CS_CPUP_TRAP_BASE    (CS_AHB0_BASE + 0x40000)
#define CS_UART0_BASE        (CS_AHB0_BASE + 0x41000)
#define CS_UART1_BASE        (CS_AHB0_BASE + 0x42000)
#define CS_UART2_BASE        (CS_AHB0_BASE + 0x43000)
#define CS_IPCAPP_BASE       (CS_AHB0_BASE + 0x44000)
#define CS_IPCEMB_BASE       (CS_AHB0_BASE + 0x44100)
#define CS_ASDMA_BASE        (CS_AHB0_BASE + 0x50000)
#define CS_AUDPROC_BASE      (CS_AHB0_BASE + 0x61000)
#define CS_DMA_BASE          (CS_AHB0_BASE + 0x70000)
#define CS_DMA_CE_MEM_BASE   (CS_AHB0_BASE + 0x73000)
#define CS_CPUSB_TRAP_BASE   (CS_AHB0_BASE + 0x80000)
#define CS_CPUSW_TRAP_BASE   (CS_AHB0_BASE + 0x81000)
#define CS_SDMMC_BASE        (CS_AHB0_BASE + 0x82000)

/* APB0 peripherals                                                           */
#define CS_CPUSYSCTRL_BASE   (CS_APB0_BASE + 0x00000)
#define CS_PWM_BASE          (CS_APB0_BASE + 0x01000)
#define CS_I2CM0_BASE        (CS_APB0_BASE + 0x02000)
#define CS_WDT0_BASE         (CS_APB0_BASE + 0x03000)
#define CS_WDT1_BASE         (CS_APB0_BASE + 0x04000)
#define CS_SPI0_BASE         (CS_APB0_BASE + 0x05000)
#define CS_TIM0_BASE         (CS_APB0_BASE + 0x06000)
#define CS_TIM1_BASE         (CS_APB0_BASE + 0x07000)
#define CS_SPINLOCK_BASE     (CS_APB0_BASE + 0x08000)
#define CS_TRNG_BASE         (CS_APB0_BASE + 0x09000)
#define CS_WDT2_BASE         (CS_APB0_BASE + 0x0A000)
#define CS_WDT3_BASE         (CS_APB0_BASE + 0x0C000)
#define CS_PMON_BASE         (CS_APB0_BASE + 0x0D000)
#define CS_ANAR0_BASE        (CS_APB0_BASE + 0x0E000)

/* APB1 peripherals                                                           */
#define CS_AONSYSCTRL_BASE   (CS_APB1_BASE + 0x00000)
#define CS_TIM2_BASE         (CS_APB1_BASE + 0x01000)
#define CS_WDT4_BASE         (CS_APB1_BASE + 0x02000)
#define CS_IOMUX_BASE        (CS_APB1_BASE + 0x03000)
#define CS_GPIO_BASE         (CS_APB1_BASE + 0x04000)
#define CS_PSIM_BASE         (CS_APB1_BASE + 0x05000)
#define CS_PWRCTRL_BASE      (CS_APB1_BASE + 0x06000)
#define CS_ANAR1_BASE        (CS_APB1_BASE + 0x08000)

/* AHB1 peripherals                                                           */
#define CS_I2CS_BASE         (CS_AHB1_BASE + 0x18000)
#define CS_WCNAON_BASE       (CS_AHB1_BASE + 0x80000)

/* AHB2 peripherals                                                           */
#define CS_USB_BASE          (CS_AHB2_BASE + 0x00000)
#define CS_SDIO_BASE         (CS_AHB2_BASE + 0x40000)
#define CS_PSRAMR_BASE       (CS_AHB2_BASE + 0x41000)
#define CS_UHSPSRAMR_BASE    (CS_AHB2_BASE + 0x41800)

#endif // _CHIP_H_
