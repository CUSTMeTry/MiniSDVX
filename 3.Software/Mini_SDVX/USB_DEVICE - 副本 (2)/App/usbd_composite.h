/**
 ******************************************************************************
 * @file    usbd_composite_core.h
 * @author  MCD Application Team
 * @brief   Header file for the usbd_composite_core.c file.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2015 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_COMPOSITE_CORE_H
#define __USB_COMPOSITE_CORE_H

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbd_ioreq.h"

  /** @addtogroup STM32_USB_DEVICE_LIBRARY
   * @{
   */

  /** @defgroup USBD_COMPOSITE
   * @brief This file is the header file for usbd_composite_core.c
   * @{
   */

  /** @defgroup USBD_COMPOSITE_Exported_Defines
   * @{
   */

#define USBD_IAD_DESC_SIZE 0x08
#define USBD_IAD_DESCRIPTOR_TYPE 0x0B

#define COMPOSITE_EPIN_ADDR 0x81U
#define COMPOSITE_EPIN_SIZE 0x10U

#define USB_COMPOSITE_CONFIG_DESC_SIZ 107U

  enum
  {
    USBD_INTERFACE_CDC_CMD = 0x00,
    USBD_INTERFACE_CDC,
    USBD_INTERFACE_HID,
    USBD_INTERFACE_NUM
  };

#define USBD_CDC_fops_FS USBD_Interface_fops_FS
  /**
   * @}
   */

  /** @defgroup USBD_CORE_Exported_TypesDefinitions
   * @{
   */

  /**
   * @}
   */

  /** @defgroup USBD_CORE_Exported_Macros
   * @{
   */

  /**
   * @}
   */

  /** @defgroup USBD_CORE_Exported_Variables
   * @{
   */

  extern USBD_ClassTypeDef USBD_COMPOSITE_ClassDriver;
  /**
   * @}
   */

  /** @defgroup USB_CORE_Exported_Functions
   * @{
   */
  /**
   * @}
   */

#ifdef __cplusplus
}
#endif

#endif /* __USB_COMPOSITE_CORE_H */
/**
 * @}
 */

/**
 * @}
 */
