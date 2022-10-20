/**
 ******************************************************************************
 * @file    usbd_composite.c
 * @author  MCD Application Team
 * @brief   This file provides the HID core functions.
 *
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
 * @verbatim
 *
 *          ===================================================================
 *                                COMPOSITE Class  Description
 *          ===================================================================
 *
 *
 *
 *
 *
 *
 * @note     In HS mode and when the DMA is used, all variables and data structures
 *           dealing with the DMA during the transaction process should be 32-bit aligned.
 *
 *
 *  @endverbatim
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usbd_composite.h"
#include "usbd_ctlreq.h"

#include "usbd_cdc_if.h"
#include "usbd_custom_hid_if.h"

/** @addtogroup STM32_USB_DEVICE_LIBRARY
 * @{
 */

/** @defgroup USBD_COMPOSITE
 * @brief usbd core module
 * @{
 */

/** @defgroup USBD_COMPOSITE_Private_TypesDefinitions
 * @{
 */
/**
 * @}
 */

/** @defgroup USBD_COMPOSITE_Private_Defines
 * @{
 */
void *pClassDataCDC = NULL;
void *pClassDataHID = NULL;

/**
 * @}
 */

/** @defgroup USBD_COMPOSITE_Private_Macros
 * @{
 */

/**
 * @}
 */

/** @defgroup USBD_COMPOSITE_Private_FunctionPrototypes
 * @{
 */

static uint8_t USBD_COMPOSITE_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_COMPOSITE_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx);
static uint8_t USBD_COMPOSITE_Setup(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static uint8_t USBD_COMPOSITE_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_COMPOSITE_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_COMPOSITE_EP0_RxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_COMPOSITE_EP0_TxReady(USBD_HandleTypeDef *pdev);
static uint8_t USBD_COMPOSITE_SOF(USBD_HandleTypeDef *pdev);
static uint8_t USBD_COMPOSITE_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_COMPOSITE_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t *USBD_COMPOSITE_GetCfgDesc(uint16_t *length);
static uint8_t *USBD_COMPOSITE_GetDeviceQualifierDesc(uint16_t *length);
/**
 * @}
 */

/** @defgroup USBD_COMPOSITE_Private_Variables
 * @{
 */

USBD_ClassTypeDef USBD_COMPOSITE_ClassDriver =
    {
        USBD_COMPOSITE_Init,
        USBD_COMPOSITE_DeInit,
        USBD_COMPOSITE_Setup,
        USBD_COMPOSITE_EP0_TxReady,
        USBD_COMPOSITE_EP0_RxReady,
        USBD_COMPOSITE_DataIn,
        USBD_COMPOSITE_DataOut,
        USBD_COMPOSITE_SOF,
        USBD_COMPOSITE_IsoINIncomplete,
        USBD_COMPOSITE_IsoOutIncomplete,
        USBD_COMPOSITE_GetCfgDesc,
        USBD_COMPOSITE_GetCfgDesc,
        USBD_COMPOSITE_GetCfgDesc,
        USBD_COMPOSITE_GetDeviceQualifierDesc,
};

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif /* __ICCARM__ */
/* USB COMPOSITE device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_COMPOSITE_CfgDesc[USB_COMPOSITE_CONFIG_DESC_SIZ] __ALIGN_END =
    {
        /* 配置描述 */
        0x09,                          /* bLength: Configuration Descriptor size */
        USB_DESC_TYPE_CONFIGURATION,   /* bDescriptorType: Configuration */
        USB_COMPOSITE_CONFIG_DESC_SIZ, /* wTotalLength: Bytes returned */
        0x00,
        0x03, /* bNumInterfaces: 1 interface */
        0x01,               /* bConfigurationValue: Configuration value */
        0x00,               /* iConfiguration: Index of string descriptor
                                describing the configuration */
#if (USBD_SELF_POWERED == 1U)
        0xC0, /* bmAttributes: Bus Powered according to user configuration */
#else
        0x80, /* bmAttributes: Bus Powered according to user configuration */
#endif                  /* USBD_SELF_POWERED */
        USBD_MAX_POWER, /* MaxPower (mA) */

/* 第一个设备 CDC设备 设备描述符 66*/
#if 1
        /*IAD 描述*/
        USBD_IAD_DESC_SIZE,       /* IAD Descriptor Size bLength IAD描述符大小*/
        USBD_IAD_DESCRIPTOR_TYPE, /* IAD Descriptor Type bDescriptorType IAD描述符类型*/
        0x00,                     /* bFirstInterface 接口描述符位置*/
        0x02,                     /* bInterfaceCount 两个接口*/
        0x02,                     /* bFunctionClass */
        0x02,                     /* bFnctionSubClass */
        0x01,                     /* bInterfaceProtocol */
        0x00,
        /* 08 */

       /*---------------------------------------------------------------------------*/

        /* Interface Descriptor */
        0x09,                    /* bLength: Interface Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: Interface */
        /* Interface descriptor type */
        0x00, /* bInterfaceNumber: Number of Interface */
        0x00, /* bAlternateSetting: Alternate setting */
        0x01, /* bNumEndpoints: One endpoint used */
        0x02, /* bInterfaceClass: Communication Interface Class */
        0x02, /* bInterfaceSubClass: Abstract Control Model */
        0x01, /* bInterfaceProtocol: Common AT commands */
        0x00, /* iInterface */

        /* Header Functional Descriptor */
        0x05, /* bLength: Endpoint Descriptor size */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x00, /* bDescriptorSubtype: Header Func Desc */
        0x10, /* bcdCDC: spec release number */
        0x01,

        /* Call Management Functional Descriptor */
        0x05, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x01, /* bDescriptorSubtype: Call Management Func Desc */
        0x00, /* bmCapabilities: D0+D1 */
        0x01, /* bDataInterface */

        /* ACM Functional Descriptor */
        0x04, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x02, /* bDescriptorSubtype: Abstract Control Management desc */
        0x02, /* bmCapabilities */

        /* Union Functional Descriptor */
        0x05, /* bFunctionLength */
        0x24, /* bDescriptorType: CS_INTERFACE */
        0x06, /* bDescriptorSubtype: Union func desc */
        0x00, /* bMasterInterface: Communication class interface */
        0x01, /* bSlaveInterface0: Data Class Interface */

        /* Endpoint 2 Descriptor */
        0x07,                        /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,      /* bDescriptorType: Endpoint */
        CDC_CMD_EP,                  /* bEndpointAddress */
        0x03,                        /* bmAttributes: Interrupt */
        LOBYTE(CDC_CMD_PACKET_SIZE), /* wMaxPacketSize */
        HIBYTE(CDC_CMD_PACKET_SIZE),
        CDC_FS_BINTERVAL, /* bInterval */
        /*---------------------------------------------------------------------------*/

        /* Data class interface descriptor */
        0x09,                    /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_INTERFACE, /* bDescriptorType: */
        0x01,                    /* bInterfaceNumber: Number of Interface */
        0x00,                    /* bAlternateSetting: Alternate setting */
        0x02,                    /* bNumEndpoints: Two endpoints used */
        0x0A,                    /* bInterfaceClass: CDC */
        0x00,                    /* bInterfaceSubClass */
        0x00,                    /* bInterfaceProtocol */
        0x00,                    /* iInterface */

        /* Endpoint OUT Descriptor */
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
        CDC_OUT_EP,                          /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00, /* bInterval */

        /* Endpoint IN Descriptor */
        0x07,                                /* bLength: Endpoint Descriptor size */
        USB_DESC_TYPE_ENDPOINT,              /* bDescriptorType: Endpoint */
        CDC_IN_EP,                           /* bEndpointAddress */
        0x02,                                /* bmAttributes: Bulk */
        LOBYTE(CDC_DATA_FS_MAX_PACKET_SIZE), /* wMaxPacketSize */
        HIBYTE(CDC_DATA_FS_MAX_PACKET_SIZE),
        0x00, /* bInterval */
#endif
/* 第二个设备 HID设备 设备描述符 41*/
#if 1
        /************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */
  0x09,                                               /* bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,                            /* bDescriptorType: Interface descriptor type */
  0x02,                                               /* bInterfaceNumber: Number of Interface */
  0x00,                                               /* bAlternateSetting: Alternate setting */
  0x02,                                               /* bNumEndpoints*/
  0x03,                                               /* bInterfaceClass: CUSTOM_HID */
  0x00,                                               /* bInterfaceSubClass : 1=BOOT, 0=no boot */
  0x00,                                               /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
  0x00,                                               /* iInterface: Index of string descriptor */
  /******************** Descriptor of CUSTOM_HID *************************/
  /* 18 */
  0x09,                                               /* bLength: CUSTOM_HID Descriptor size */
  CUSTOM_HID_DESCRIPTOR_TYPE,                         /* bDescriptorType: CUSTOM_HID */
  0x11,                                               /* bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number */
  0x01,
  0x00,                                               /* bCountryCode: Hardware target country */
  0x01,                                               /* bNumDescriptors: Number of CUSTOM_HID class descriptors
                                                         to follow */
  0x22,                                               /* bDescriptorType */
  USBD_CUSTOM_HID_REPORT_DESC_SIZE,                   /* wItemLength: Total length of Report descriptor */
  0x00,
  /******************** Descriptor of Custom HID endpoints ********************/
  /* 27 */
  0x07,                                               /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,                             /* bDescriptorType: */

  CUSTOM_HID_EPIN_ADDR,                               /* bEndpointAddress: Endpoint Address (IN) */
  0x03,                                               /* bmAttributes: Interrupt endpoint */
  CUSTOM_HID_EPIN_SIZE,                               /* wMaxPacketSize: 2 Bytes max */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,                            /* bInterval: Polling Interval */
  /* 34 */

  0x07,                                               /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,                             /* bDescriptorType: */
  CUSTOM_HID_EPOUT_ADDR,                              /* bEndpointAddress: Endpoint Address (OUT) */
  0x03,                                               /* bmAttributes: Interrupt endpoint */
  CUSTOM_HID_EPOUT_SIZE,                              /* wMaxPacketSize: 2 Bytes max  */
  0x00,
  CUSTOM_HID_FS_BINTERVAL,                            /* bInterval: Polling Interval */
  /* 41 */
#endif

};

#if defined(__ICCARM__) /*!< IAR Compiler */
#pragma data_alignment = 4
#endif /* __ICCARM__ */
/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_COMPOSITE_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
    {
        USB_LEN_DEV_QUALIFIER_DESC,
        USB_DESC_TYPE_DEVICE_QUALIFIER,
        0x00,
        0x02,
        0x00,
        0x00,
        0x00,
        0x40,
        0x01,
        0x00,
};

/**
 * @}
 */

/** @defgroup USBD_COMPOSITE_Private_Functions
 * @{
 */

/**
 * @brief  USBD_COMPOSITE_Init
 *         Initialize the COMPOSITE interface
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_COMPOSITE_Init(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  uint8_t res = 0;
  /* CDC */
  *pdev->pUserData = (void *)&USBD_CDC_fops_FS;
  res += USBD_CDC.Init(pdev, cfgidx);
  pClassDataCDC = pdev->pClassData;

  /* HID */
  *pdev->pUserData = (void *)&USBD_CustomHID_fops_FS;
  res += USBD_CUSTOM_HID.Init(pdev, cfgidx);
  pClassDataHID = pdev->pClassData;
  return (uint8_t)USBD_OK;
}

/**
 * @brief  USBD_COMPOSITE_Init
 *         DeInitialize the COMPOSITE layer
 * @param  pdev: device instance
 * @param  cfgidx: Configuration index
 * @retval status
 */
static uint8_t USBD_COMPOSITE_DeInit(USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
  pdev->pClassData = pClassDataCDC;
  *pdev->pUserData = (void *)&USBD_CDC_fops_FS;
  USBD_CDC.DeInit(pdev, cfgidx);

  pdev->pClassData = pClassDataHID;
  *pdev->pUserData = (void *)&USBD_CustomHID_fops_FS;
  USBD_CUSTOM_HID.DeInit(pdev, cfgidx);

  return (uint8_t)USBD_OK;
}

/**
 * @brief  USBD_COMPOSITE_Setup
 *         Handle the COMPOSITE specific requests
 * @param  pdev: instance
 * @param  req: usb requests
 * @retval status
 */
static uint8_t USBD_COMPOSITE_Setup(USBD_HandleTypeDef *pdev,
                                    USBD_SetupReqTypedef *req)
{
  USBD_StatusTypeDef ret = USBD_OK;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_RECIPIENT_INTERFACE:
    switch (req->wIndex)
    {
    case USBD_INTERFACE_CDC:
    case USBD_INTERFACE_CDC_CMD:
      pdev->pClassData = pClassDataCDC;
      pdev->pUserData[0] = &USBD_CDC_fops_FS;
      USBD_CDC.Setup(pdev, req);
      break;
    case USBD_INTERFACE_HID:
      pdev->pClassData = pClassDataHID;
      pdev->pUserData[0] = (void *)&USBD_CustomHID_fops_FS;
      USBD_CUSTOM_HID.Setup(pdev, req);
      break;
    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      // printf("[%s] %d: req->wIndex = %x\r\n", __func__, __LINE__, req->wLength);
      break;
    }
    break;
  case USB_REQ_RECIPIENT_ENDPOINT:
    switch (req->wIndex)
    {
    case CDC_IN_EP:
    case CDC_OUT_EP:
    case CDC_CMD_EP:
      pdev->pClassData = pClassDataCDC;
      pdev->pUserData[0] = &USBD_CDC_fops_FS;
      USBD_CDC.Setup(pdev, req);
      break;
    case CUSTOM_HID_EPIN_ADDR:
    case CUSTOM_HID_EPOUT_ADDR:
      pdev->pClassData = pClassDataHID;
      pdev->pUserData[0] = (void *)&USBD_CustomHID_fops_FS;
      USBD_CUSTOM_HID.Setup(pdev, req);
      break;
    default:
      USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
      // printf("[%s] %d: req->wIndex = %x\r\n", __func__, __LINE__, req->wIndex);
      break;
    }
    break;
  default:
    USBD_CtlError(pdev, req);
    ret = USBD_FAIL;
    // printf("[%s] %d: req->wIndex = %x\r\n", __func__, __LINE__, req->wIndex);
    break;
  }
  return (uint8_t)ret;
}

/**
 * @brief  USBD_COMPOSITE_GetCfgDesc
 *         return configuration descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
static uint8_t *USBD_COMPOSITE_GetCfgDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_COMPOSITE_CfgDesc);
  return USBD_COMPOSITE_CfgDesc;
}

/**
 * @brief  USBD_COMPOSITE_GetDeviceQualifierDesc
 *         return Device Qualifier descriptor
 * @param  length : pointer data length
 * @retval pointer to descriptor buffer
 */
uint8_t *USBD_COMPOSITE_GetDeviceQualifierDesc(uint16_t *length)
{
  *length = (uint16_t)sizeof(USBD_COMPOSITE_DeviceQualifierDesc);
  return USBD_COMPOSITE_DeviceQualifierDesc;
}

/**
 * @brief  USBD_COMPOSITE_DataIn
 *         handle data IN Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_COMPOSITE_DataIn(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  switch (epnum)
  {
  case (CDC_IN_EP & 0x0f):
    pdev->pClassData = pClassDataCDC;
    pdev->pUserData[0] = &USBD_CDC_fops_FS;
    USBD_CDC.DataIn(pdev, epnum);
    break;
  case (CUSTOM_HID_EPIN_ADDR & 0x0f):
    pdev->pClassData = pClassDataHID;
    pdev->pUserData[0] = (void *)&USBD_CustomHID_fops_FS;
    USBD_CUSTOM_HID.DataIn(pdev, epnum);
    break;
  default:
    break;
  }

  return (uint8_t)USBD_OK;
}

/**
 * @brief  USBD_COMPOSITE_EP0_RxReady
 *         handle EP0 Rx Ready event
 * @param  pdev: device instance
 * @retval status
 */
static uint8_t USBD_COMPOSITE_EP0_RxReady(USBD_HandleTypeDef *pdev)
{
  pdev->pClassData = pClassDataCDC;
  pdev->pUserData[0] = &USBD_CDC_fops_FS;
  USBD_CDC.EP0_RxReady(pdev);
  return (uint8_t)USBD_OK;
}

/**
 * @brief  USBD_COMPOSITE_EP0_TxReady
 *         handle EP0 TRx Ready event
 * @param  pdev: device instance
 * @retval status
 */
static uint8_t USBD_COMPOSITE_EP0_TxReady(USBD_HandleTypeDef *pdev)
{

  return (uint8_t)USBD_OK;
}

/**
 * @brief  USBD_COMPOSITE_SOF
 *         handle SOF event
 * @param  pdev: device instance
 * @retval status
 */
static uint8_t USBD_COMPOSITE_SOF(USBD_HandleTypeDef *pdev)
{

  return (uint8_t)USBD_OK;
}

/**
 * @brief  USBD_COMPOSITE_IsoINIncomplete
 *         handle data ISO IN Incomplete event
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_COMPOSITE_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return (uint8_t)USBD_OK;
}

/**
 * @brief  USBD_COMPOSITE_IsoOutIncomplete
 *         handle data ISO OUT Incomplete event
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_COMPOSITE_IsoOutIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{

  return (uint8_t)USBD_OK;
}
/**
 * @brief  USBD_COMPOSITE_DataOut
 *         handle data OUT Stage
 * @param  pdev: device instance
 * @param  epnum: endpoint index
 * @retval status
 */
static uint8_t USBD_COMPOSITE_DataOut(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
  switch (epnum)
  {
  case CDC_OUT_EP:
  case CDC_CMD_EP:
    pdev->pClassData = pClassDataCDC;
    pdev->pUserData[0] = (void *)&USBD_CDC_fops_FS;
    USBD_CDC.DataOut(pdev, epnum);
    break;
  case CUSTOM_HID_EPOUT_ADDR:
    pdev->pClassData = pClassDataHID;
    pdev->pUserData[0] = (void *)&USBD_CustomHID_fops_FS;
    USBD_CUSTOM_HID.DataOut(pdev, epnum);
    break;
  default:
    break;
  }

  return (uint8_t)USBD_OK;
}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
