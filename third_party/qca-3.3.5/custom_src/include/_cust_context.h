////------------------------------------------------------------------------------
//// <copyright file="cust_context.h" company="Atheros">
//// Copyright (c) 2011-2015 Qualcomm Atheros, Inc.
//// All Rights Reserved.
//// Qualcomm Atheros Confidential and Proprietary.
////------------------------------------------------------------------------------
////==============================================================================
//// Author(s): ="Atheros"
////==============================================================================
//#ifndef _CUST_CONTEXT_H_
//#define _CUST_CONTEXT_H_
///* PORT_NOTE: This file is intended to define the A_CUSTOM_DRIVER_CONTEXT
// *  structure. This structure is the same as the pCxt object which can be
// *  found throughout the driver source.  It is the main driver context which
// *  is expected to change from system to system.  It keeps a pointer to the
// *  complimentary A_DRIVER_CONTEXT which is the common context that should
// *  remain constant from system to system. The porting effort requieres that
// *  the A_CUSTOM_DRIVER_CONTEXT be populated with system specific elements. */
//
//#include "fsl_os_abstraction.h"
//#include "cust_spi_hw.h"
//
////typedef struct {
////	QOSAL_UINT32 pin;
////} A_GPIO_CTX;
////
//
// typedef struct _a_custom_hcd
//{
//    /* PORT_NOTE: These three elements are used to provide access
//     *  to the chip HW interface.  Use system appropriate element types. */
//    A_SPI_CTX   spi_cxt;  /* used for device SPI access */
//    //A_GPIO_CTX  int_cxt;  /* used for device interrupt */
//} A_CUSTOM_HCD_CXT;
//
///* PORT_NOTE: Include any System header files here to resolve any types used
// *  in A_CUSTOM_DRIVER_CONTEXT */
//
// typedef struct _a_custom_driver_context
//{
//    /* PORT_NOTE: utility_mutex is a mutex used to protect resources that
//     *  might be accessed by multiple task/threads in a system.
//     *  It is used by the ACQUIRE/RELEASE macros below.  If the target
//     *  system is single threaded then this mutex can be omitted and
//     *  the macros should be blank.*/
//#if DRIVER_CONFIG_MULTI_TASKING
//    A_MUTEX_T       utility_mutex;
//#endif
//    /* PORT_NOTE: connectStateCB allows an app to get feedback/notification
//     *  as the WIFI connection state changes. If used it should be populated
//     *  with a function pointer by an IOCTL or system equivalent. */
//    QOSAL_VOID          *connectStateCB;
//    /* PORT_NOTE: pScanOut stores scan results in a buffer so that they
//     *  can be retrieved aysnchronously by a User task. */
//    //QOSAL_UINT8*		pScanOut; /* callers buffer to hold results. */
//    //QOSAL_UINT16 		scanOutSize;/* size of pScanOut buffer */
//    //QOSAL_UINT16		scanOutCount;/* current fill count of pScanOut buffer */
//
//#if DRIVER_CONFIG_MULTI_TASKING
//    /* PORT_NOTE: These 2 events are used to block & wake their respective
//     *  tasks. */
//    event_t  driverWakeEvent;     /* wakes the driver thread */
//    event_t  userWakeEvent;       /* wakes blocked user threads */
//#if T_SELECT_VER1
//    event_t  sockSelectWakeEvent; /* wakes t_select  */
//#endif //T_SELECT_VER1
//#endif
//#if 0
//    FILE_PTR		adc_cxt;	/* used for ADC SENSOR */
//#endif
//    /* PORT_NOTE: These 2 elements provide pointers to system context.
//     *  The pDriverParam is intended provide definition for info such as
//     *  which GPIO's to use for the SPI bus.  The pUpperCxt should be
//     *  populated with a system specific object that is required when the
//     *  driver calls any system API's. In case of multi interface, the
//     *  different enet pointers are stored in the corresponding array index
//     */
//    QOSAL_VOID          *pDriverParam; /* param struct containing initialization params */
//    QOSAL_VOID          *pUpperCxt[WLAN_NUM_OF_DEVICES]; /* back pointer to the MQX enet object. */
//    /* PORT_NOTE: pCommonCxt stores the drivers common context. It is
//     *  accessed by the common source via GET_DRIVER_COMMON() below. */
//    QOSAL_VOID 			*pCommonCxt; /* the common driver context link */
//    QOSAL_BOOL			driverShutdown; /* used to shutdown the driver */
//    QOSAL_VOID*			promiscuous_cb; /* used to feed rx frames in promiscuous mode. */
//#if ENABLE_P2P_MODE
//    QOSAL_VOID          *p2pevtCB;
//    QOSAL_BOOL          p2pEvtState;
//    QOSAL_BOOL          p2pevtflag;
//#endif
//#if MANUFACTURING_SUPPORT
//    QOSAL_UINT32        testCmdRespBufLen;
//#endif
//    A_CUSTOM_HCD_CXT customhcdcxt;
//    QOSAL_VOID* httpPostCb;  /*callback handler for http post events*/
//    QOSAL_VOID* httpPostCbCxt;
//    QOSAL_VOID* otaCB;       /* callback handler for OTA event */
//    QOSAL_VOID* probeReqCB;
//}A_CUSTOM_DRIVER_CONTEXT;
//
//
///* MACROS used by common driver code to access custom context elements */
//#define GET_DRIVER_CXT(pCxt) ((A_CUSTOM_DRIVER_CONTEXT*)(pCxt))
//#define GET_DRIVER_COMMON(pCxt) ((A_DRIVER_CONTEXT*)(GET_DRIVER_CXT((pCxt))->pCommonCxt))
//
//#define GET_DRIVER_HCD_CXT(pCxt) ((A_CUSTOM_HCD_CXT*)(pCxt))   //custom struct
//
//#if DRIVER_CONFIG_MULTI_TASKING
//#define RXBUFFER_ACCESS_INIT(pCxt) A_MUTEX_INIT(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//#define RXBUFFER_ACCESS_ACQUIRE(pCxt) A_MUTEX_ACQUIRE(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//#define RXBUFFER_ACCESS_RELEASE(pCxt) A_MUTEX_RELEASE(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//#define RXBUFFER_ACCESS_DESTROY(pCxt) A_MUTEX_DELETE(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//
//#define TXQUEUE_ACCESS_INIT(pCxt) (A_OK)/* because its the same mutex as RXBUFFER_ACCESS this is NOP */
//#define TXQUEUE_ACCESS_ACQUIRE(pCxt) A_MUTEX_ACQUIRE(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//#define TXQUEUE_ACCESS_RELEASE(pCxt) A_MUTEX_RELEASE(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//#define TXQUEUE_ACCESS_DESTROY(pCxt) /* because its the same mutex as RXBUFFER_ACCESS this is NOP */
//
//#define IRQEN_ACCESS_INIT(pCxt) (A_OK)/* because its the same mutex as RXBUFFER_ACCESS this is NOP */
//#define IRQEN_ACCESS_ACQUIRE(pCxt) A_MUTEX_ACQUIRE(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//#define IRQEN_ACCESS_RELEASE(pCxt) A_MUTEX_RELEASE(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//#define IRQEN_ACCESS_DESTROY(pCxt) /* because its the same mutex as RXBUFFER_ACCESS this is NOP */
//
//#define DRIVER_SHARED_RESOURCE_ACCESS_INIT(pCxt) (A_OK)/* because its the same mutex as RXBUFFER_ACCESS this is NOP */
//#define DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt) A_MUTEX_ACQUIRE(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//#define DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt) A_MUTEX_RELEASE(&(GET_DRIVER_CXT(pCxt)->utility_mutex))
//#define DRIVER_SHARED_RESOURCE_ACCESS_DESTROY(pCxt) /* because its the same mutex as RXBUFFER_ACCESS this is NOP */
//
//#else /* DRIVER_CONFIG_MULTI_TASKING */
//
//#define RXBUFFER_ACCESS_INIT(pCxt) (A_OK)
//#define RXBUFFER_ACCESS_ACQUIRE(pCxt)
//#define RXBUFFER_ACCESS_RELEASE(pCxt)
//#define RXBUFFER_ACCESS_DESTROY(pCxt)
//
//#define TXQUEUE_ACCESS_INIT(pCxt) (A_OK)/* because its the same mutex as RXBUFFER_ACCESS this is NOP */
//#define TXQUEUE_ACCESS_ACQUIRE(pCxt)
//#define TXQUEUE_ACCESS_RELEASE(pCxt)
//#define TXQUEUE_ACCESS_DESTROY(pCxt)
//
//#define IRQEN_ACCESS_INIT(pCxt) (A_OK)/* because its the same mutex as RXBUFFER_ACCESS this is NOP */
//#define IRQEN_ACCESS_ACQUIRE(pCxt)
//#define IRQEN_ACCESS_RELEASE(pCxt)
//#define IRQEN_ACCESS_DESTROY(pCxt)
//
//#define DRIVER_SHARED_RESOURCE_ACCESS_INIT(pCxt) (A_OK)/* because its the same mutex as RXBUFFER_ACCESS this is NOP */
//#define DRIVER_SHARED_RESOURCE_ACCESS_ACQUIRE(pCxt)
//#define DRIVER_SHARED_RESOURCE_ACCESS_RELEASE(pCxt)
//#define DRIVER_SHARED_RESOURCE_ACCESS_DESTROY(pCxt)
//
//#endif /* DRIVER_CONFIG_MULTI_TASKING */
//
//#endif /* _CUST_CONTEXT_H_ */
