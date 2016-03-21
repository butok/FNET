/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
*
***************************************************************************
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may
*  not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
*  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
**********************************************************************/
/*!
*
* @file fapp_tftp.h
*
* @author Andrey Butok
*
* @brief FNET Shell Demo API.
*
***************************************************************************/

#ifndef _FAPP_TFTP_H_

#define _FAPP_TFTP_H_

#if defined(__cplusplus)
extern "C" {
#endif

#include "fapp_config.h"

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD || FAPP_CFG_SETGET_CMD_TYPE


/********************** RAW **************************************/
/* RAW Rx handler control structure. */
struct fapp_tftp_rx_handler_raw_context
{
    fnet_uint32_t dest; /* Destination address */
};

/* RAW Tx handler control structure. */
#define FAPP_TFTP_RAW_DATA_MAX     (128u)
struct fapp_tftp_tx_handler_raw_context
{
    fnet_uint8_t  data[FAPP_TFTP_RAW_DATA_MAX];
    fnet_size_t   data_size;
    fnet_uint8_t  *data_cur;
    fnet_uint8_t  *data_start;
    fnet_uint8_t  *data_end;
};

/******************************* BIN *****************************/
/* Binary file format is address (4
 * bytes), byte count (4 bytes), and
 * data bytes (variable length).
 */
typedef enum
{
    FAPP_TFTP_HANDLER_BIN_STATE_INIT = 0,
    FAPP_TFTP_HANDLER_BIN_STATE_GETHEADER,
    FAPP_TFTP_HANDLER_BIN_STATE_GETDATA
}
fapp_tftp_handler_bin_state_t;


#define FAPP_TFTP_BIN_HEADER_SIZE (8u)


/* TFTP BIN handler control structure. */
FNET_COMP_PACKED_BEGIN
struct fapp_tftp_rx_handler_bin_context
{
    union
    {
        struct
        {
            fnet_uint32_t address	FNET_COMP_PACKED;
            fnet_uint32_t size 		FNET_COMP_PACKED;
        } header 					FNET_COMP_PACKED;

        fnet_uint8_t header_bytes[FAPP_TFTP_BIN_HEADER_SIZE]	FNET_COMP_PACKED;
    }	FNET_COMP_PACKED;
    fnet_index_t header_index 		    FNET_COMP_PACKED;
    fapp_tftp_handler_bin_state_t state FNET_COMP_PACKED;
};
FNET_COMP_PACKED_END

/* TFTP SREC TX handler control structure. */
#define FAPP_TFTP_BIN_DATA_MAX     (252u)

FNET_COMP_PACKED_BEGIN
struct fapp_tftp_tx_handler_bin_context
{
    struct
    {
        fnet_uint32_t address 	FNET_COMP_PACKED;
        fnet_uint32_t count 	FNET_COMP_PACKED;
        fnet_uint8_t  data[FAPP_TFTP_BIN_DATA_MAX]	FNET_COMP_PACKED;
    } bin_line 					FNET_COMP_PACKED;
    fnet_size_t     bin_line_size   FNET_COMP_PACKED;
    fnet_uint8_t    *bin_line_cur   FNET_COMP_PACKED;
    fnet_uint8_t    *data_start 	FNET_COMP_PACKED;
    fnet_uint8_t    *data_end 	    FNET_COMP_PACKED;
};
FNET_COMP_PACKED_END



/********************** SREC **************************************/

/* SREC Tx Handler states */
typedef enum
{
    FAPP_TFTP_RX_HANDLER_SREC_STATE_INIT = 0,
    FAPP_TFTP_RX_HANDLER_SREC_STATE_GETSTART,
    FAPP_TFTP_RX_HANDLER_SREC_STATE_GETTYPE,
    FAPP_TFTP_RX_HANDLER_SREC_STATE_GETCOUNT,
    FAPP_TFTP_RX_HANDLER_SREC_STATE_GETDATA
}
fapp_tftp_rx_handler_srec_state_t;

/* SREC Rx Handler states */
typedef enum
{
    FAPP_TFTP_TX_HANDLER_SREC_STATE_INIT = 0,
    FAPP_TFTP_TX_HANDLER_SREC_STATE_DATA,
    FAPP_TFTP_TX_HANDLER_SREC_STATE_EOB,
    FAPP_TFTP_TX_HANDLER_SREC_STATE_END
}
fapp_tftp_tx_handler_srec_state_t;


/*
+-------------------//------------------//-----------------------+
| type | count | address  |            data           | checksum |
+-------------------//------------------//-----------------------+
*/


/* TFTP SREC handler control structure. */
FNET_COMP_PACKED_BEGIN
struct fapp_tftp_rx_handler_srec_context
{
    fapp_tftp_rx_handler_srec_state_t   state               FNET_COMP_PACKED;
    fnet_uint8_t                        type 			    FNET_COMP_PACKED;	/* Record type */
    fnet_index_t                        record_hex_index    FNET_COMP_PACKED;
    union
    {
        struct
        {
            fnet_uint8_t count 	FNET_COMP_PACKED;	/* Byte count */
            fnet_uint8_t data[255] FNET_COMP_PACKED; 	/* Address + Data */
        } record	FNET_COMP_PACKED;
        fnet_uint8_t record_bytes[255 + 1]	FNET_COMP_PACKED;
    }	FNET_COMP_PACKED;
};
FNET_COMP_PACKED_END

/* TFTP SREC TX handler control structure. */
#define FAPP_TFTP_SREC_DATA_MAX     (28u)
FNET_COMP_PACKED_BEGIN
struct fapp_tftp_tx_handler_srec_context
{
    fapp_tftp_tx_handler_srec_state_t state	FNET_COMP_PACKED;
    struct
    {
        fnet_uint8_t        S 			    FNET_COMP_PACKED;
        fnet_uint8_t        type 		    FNET_COMP_PACKED;
        fnet_uint8_t        count[2] 	    FNET_COMP_PACKED;
        fnet_uint8_t        address[8]      FNET_COMP_PACKED;
        fnet_uint16_t       data[FAPP_TFTP_SREC_DATA_MAX + 1u/*checksum*/ + 1u/*\r\n*/ + 1u/*eol*/] FNET_COMP_PACKED;
    } srec_line 		            FNET_COMP_PACKED;
    fnet_size_t     srec_line_size 	FNET_COMP_PACKED;
    fnet_uint8_t    *srec_line_cur  FNET_COMP_PACKED;
    fnet_uint8_t    *data_start 	FNET_COMP_PACKED;
    fnet_uint8_t    *data_end 	    FNET_COMP_PACKED;
};
FNET_COMP_PACKED_END

/********************** HEX **************************************/

/* HEX Rx Handler states */
typedef enum
{
    FAPP_TFTP_RX_HANDLER_HEX_STATE_INIT = 0,
    FAPP_TFTP_RX_HANDLER_HEX_STATE_GETCOLON,
    FAPP_TFTP_RX_HANDLER_HEX_STATE_GETHEAD,
    FAPP_TFTP_RX_HANDLER_HEX_STATE_GETDATA,
    FAPP_TFTP_RX_HANDLER_HEX_STATE_IGNOREALL
}
fapp_tftp_rx_handler_hex_state_t;

/* HEX Tx Handler states */
typedef enum
{
    FAPP_TFTP_TX_HANDLER_HEX_STATE_INIT = 0,
    FAPP_TFTP_TX_HANDLER_HEX_STATE_EXADDR,
    FAPP_TFTP_TX_HANDLER_HEX_STATE_DATA,
    FAPP_TFTP_TX_HANDLER_HEX_STATE_EOB,
    FAPP_TFTP_TX_HANDLER_HEX_STATE_END
}
fapp_tftp_tx_handler_hex_state_t;


/*
DATA RECORD:
+-----------------------------------------//--------------------------+
| : | count=XX | address=XXXX  | 00 |         data      | checksum=XX |
+-----------------------------------------//--------------------------+

EOF RECORD:
+-------------------------------------------------+
| : | count=00 | address=0000  | 01 | checksum=FF |
+-------------------------------------------------+

EXTENDED SEGMENT ADDRESS RECORD (offset=bits[19-4] of 20-bit segment base address added to subsequent data addresses)
+---------------------------------------------------------------+
| : | count=02 | address=0000  | 02 | offset=XXXX | checksum=XX |
+---------------------------------------------------------------+

EXTENDED LINEAR ADDRESS RECORD (offset=bits[31-16] of 32-bit linear base address added to subsequent data addresses)
+---------------------------------------------------------------+
| : | count=02 | address=0000  | 04 | offset=XXXX | checksum=XX |
+---------------------------------------------------------------+
*/

/* TFTP HEX handler control structure. */
FNET_COMP_PACKED_BEGIN
struct fapp_tftp_rx_handler_hex_context
{
    fapp_tftp_rx_handler_hex_state_t state FNET_COMP_PACKED;
    fnet_index_t index           					FNET_COMP_PACKED;
    fnet_index_t todo            					FNET_COMP_PACKED;
    fnet_uint8_t bcount					FNET_COMP_PACKED;	/* data byte count */
    fnet_uint8_t type 						FNET_COMP_PACKED;	/* record type */
    fnet_uint8_t csum 						FNET_COMP_PACKED;   /* checksum */
    fnet_uint16_t addr   					FNET_COMP_PACKED;   /* 16bit addr from the record */
    fnet_uint32_t addr_base 				FNET_COMP_PACKED;   /* 32bit addr base */
    fnet_uint8_t data[255]					FNET_COMP_PACKED;   /* data buffer */
};
FNET_COMP_PACKED_END
/* TFTP HEX TX handler control structure. */
#define FAPP_TFTP_HEX_DATA_MAX     (16u)
FNET_COMP_PACKED_BEGIN
struct fapp_tftp_tx_handler_hex_context
{
    fapp_tftp_tx_handler_hex_state_t state	FNET_COMP_PACKED;
    struct
    {
        fnet_uint8_t  head[9]  	FNET_COMP_PACKED;
        fnet_uint16_t data[FAPP_TFTP_HEX_DATA_MAX + 1u/*checksum*/ + 1u/*\r\n*/ + 1u/*eol*/] FNET_COMP_PACKED;
    } hex_line 		FNET_COMP_PACKED;
    fnet_size_t hex_line_size 	FNET_COMP_PACKED;
    fnet_uint8_t *hex_line_cur  FNET_COMP_PACKED;
    fnet_uint8_t *data_start FNET_COMP_PACKED;
    fnet_uint8_t *data_end 	 FNET_COMP_PACKED;
};
FNET_COMP_PACKED_END


/******* TFTP handler control structure. ******/
typedef struct
{
    union
    {
        struct fapp_tftp_rx_handler_raw_context  rx_raw;
        struct fapp_tftp_tx_handler_raw_context  tx_raw;
        struct fapp_tftp_rx_handler_bin_context  rx_bin;
        struct fapp_tftp_tx_handler_bin_context  tx_bin;
        struct fapp_tftp_rx_handler_srec_context rx_srec;
        struct fapp_tftp_tx_handler_srec_context tx_srec;
        struct fapp_tftp_rx_handler_hex_context  rx_hex;
        struct fapp_tftp_tx_handler_hex_context  tx_hex;
    };
    const struct image_type *current_type;
    fnet_size_t image_size;
} fapp_tftp_handler_control_t;

/* Image type. */
struct image_type
{
    fapp_params_tftp_file_type_t    index;
    fnet_char_t                     *name;
    fnet_return_t(* rx_handler)(fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t n);
    fnet_int32_t(* tx_handler)(fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t n);
};

const struct image_type *fapp_tftp_image_type_by_index (fapp_params_tftp_file_type_t index);
const struct image_type *fapp_tftp_image_type_by_name (fnet_char_t *name);
#endif

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD
/* Region for TFTP upload.*/
#define FAPP_TFTP_TX_MEM_REGION (&fapp_mem_regions[0])
#endif

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD
void fapp_tftp_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
#endif

#if FAPP_CFG_TFTPS_CMD
void fapp_tftps_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
void fapp_tftps_info( fnet_shell_desc_t desc );
void fapp_tftps_release( void );
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _FAPP_TFTP_H_ */

