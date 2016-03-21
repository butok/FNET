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
* @file fapp_tftp.c
*
* @author Andrey Butok
*
* @brief TFTP file loader implementation.
*
***************************************************************************/

#include "fapp.h"
#include "fapp_prv.h"
#include "fapp_tftp.h"
#include "fapp_mem.h"

/************************************************************************
*     Definitions.
*************************************************************************/
#define FAPP_TFTP_FAILED            "\nFailed!"
#define FAPP_TFTP_ERR               "\nTFTP Error: Code %d \"%s\"."
#define FAPP_TFTP_CHECKSUM_ERR      "\nChecksum error."
#define FAPP_TFTP_RECORD_ERR        "\nInvalid record received, ignoring."

#define FAPP_TFTP_RX_HEADER_STR     "TFTP downloading \'%s\' (%s) from %s :  "
#define FAPP_TFTP_TX_HEADER_STR     "TFTP uploading \'%s\' (%s) to %s :  "
#define FAPP_TFTP_ENTRYPOINT_STR    "\nEntry point set to 0x%08X"
#define FAPP_TFTP_COMPLETED_STR     "\nTFTP completed (%d bytes)"

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD

    /************************************************************************
    *     Function Prototypes
    *************************************************************************/
    #if FAPP_CFG_TFTP_RX_RAW && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        static fnet_return_t fapp_tftp_rx_handler_raw (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size);
    #endif
    #if FAPP_CFG_TFTP_TX_RAW && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        static fnet_int32_t fapp_tftp_tx_handler_raw (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size);
    #endif
    #if FAPP_CFG_TFTP_RX_BIN && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        static fnet_return_t fapp_tftp_rx_handler_bin (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size);
    #endif
    #if FAPP_CFG_TFTP_TX_BIN && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        static fnet_int32_t fapp_tftp_tx_handler_bin (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size);
    #endif
    #if FAPP_CFG_TFTP_RX_SREC && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        static fnet_return_t fapp_tftp_rx_handler_srec (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size);
    #endif
    #if FAPP_CFG_TFTP_TX_SREC && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        static fnet_int32_t fapp_tftp_tx_handler_srec (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size);
    #endif
    #if FAPP_CFG_TFTP_RX_HEX && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        static fnet_return_t fapp_tftp_rx_handler_hex (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size);
    #endif
    #if FAPP_CFG_TFTP_TX_HEX && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        static fnet_int32_t fapp_tftp_tx_handler_hex (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size);
    #endif
#endif


#if FAPP_CFG_TFTP_TX_BIN && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
    static void fapp_tftp_tx_bin_gen (struct fapp_tftp_tx_handler_bin_context *tx_bin);
#endif
#if FAPP_CFG_TFTP_TX_RAW && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
    static  void fapp_tftp_tx_raw_gen (struct fapp_tftp_tx_handler_raw_context *tx_raw);
#endif
#if FAPP_CFG_TFTP_TX_SREC && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
    static void fapp_tftp_tx_srec_gen (struct fapp_tftp_tx_handler_srec_context *tx_srec);
#endif
#if FAPP_CFG_TFTP_TX_HEX && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
    static void fapp_tftp_tx_hex_gen (struct fapp_tftp_tx_handler_hex_context *tx_hex);
#endif

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD
    static fnet_int32_t fapp_tftp_handler (fnet_tftp_request_t request_type, fnet_uint8_t *data_ptr, fnet_size_t data_size, fnet_return_t result, void *shl_desc);
#endif

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD
    static void fapp_tftp_tx_image_begin_end(fnet_uint8_t *FNET_COMP_PACKED_VAR *data_begin_p, fnet_uint8_t *FNET_COMP_PACKED_VAR *data_end_p);
#endif

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD
static fapp_tftp_handler_control_t fapp_tftp_handler_control;

/* Progress variables. */
const static fnet_uint8_t   progress[] = {'\\', '|', '/', '-'};
static fnet_index_t progress_counter = 0u;

static void fapp_tftp_on_ctrlc(fnet_shell_desc_t desc);
#endif


#if FAPP_CFG_TFTPS_CMD

/******* TFTP Firmware Server handler control structure. ******/
static fapp_tftp_handler_control_t fapp_tftps_handler_control;

static fnet_tftp_srv_desc_t fapp_tftp_srv_desc = 0; /* TFTP server descriptor. */

static fnet_return_t fapp_tftps_request_handler(fnet_tftp_request_t request_type,
        const struct sockaddr *address,
        fnet_char_t *filename,
        fnet_char_t *mode,
        fnet_tftp_error_t *error_code,
        fnet_char_t * *error_message,
        void *shl_desc);

static fnet_int32_t fapp_tftps_data_handler(fnet_tftp_request_t request,
        fnet_uint8_t *data_ptr,
        fnet_size_t data_size,
        fnet_tftp_error_t *error_code,
        fnet_char_t * *error_message,
        void *shl_desc);

#endif /* FAPP_CFG_TFTPS_CMD*/


#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD || FAPP_CFG_SETGET_CMD_TYPE

#define FAPP_PARAMS_TFTP_FILE_TYPE_RAW_STR      "raw"   /* Raw binary file. */
#define FAPP_PARAMS_TFTP_FILE_TYPE_BIN_STR      "bin"   /* CodeWarrior binary file. */
#define FAPP_PARAMS_TFTP_FILE_TYPE_SREC_STR     "srec"  /* Motorola SREC file. */
#define FAPP_PARAMS_TFTP_FILE_TYPE_HEX_STR      "hex"   /* Intel Hexadecimal Object file. */


const struct image_type image_types[] =
{
    {
        FAPP_PARAMS_TFTP_FILE_TYPE_RAW, FAPP_PARAMS_TFTP_FILE_TYPE_RAW_STR,
#if FAPP_CFG_TFTP_RX_RAW && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        fapp_tftp_rx_handler_raw
#else
        FNET_NULL
#endif
        ,
#if FAPP_CFG_TFTP_TX_RAW && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        fapp_tftp_tx_handler_raw
#else
        FNET_NULL
#endif
    },
    {
        FAPP_PARAMS_TFTP_FILE_TYPE_BIN, FAPP_PARAMS_TFTP_FILE_TYPE_BIN_STR,
#if FAPP_CFG_TFTP_RX_BIN && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        fapp_tftp_rx_handler_bin
#else
        FNET_NULL
#endif
        ,
#if FAPP_CFG_TFTP_TX_BIN && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        fapp_tftp_tx_handler_bin
#else
        FNET_NULL
#endif
    },
    {
        FAPP_PARAMS_TFTP_FILE_TYPE_SREC, FAPP_PARAMS_TFTP_FILE_TYPE_SREC_STR,
#if FAPP_CFG_TFTP_RX_SREC && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        fapp_tftp_rx_handler_srec
#else
        FNET_NULL
#endif
        ,
#if FAPP_CFG_TFTP_TX_SREC && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        fapp_tftp_tx_handler_srec
#else
        FNET_NULL
#endif
    },
    {
        FAPP_PARAMS_TFTP_FILE_TYPE_HEX, FAPP_PARAMS_TFTP_FILE_TYPE_HEX_STR,
#if FAPP_CFG_TFTP_RX_HEX && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        fapp_tftp_rx_handler_hex
#else
        FNET_NULL
#endif
        ,
#if FAPP_CFG_TFTP_TX_HEX && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
        fapp_tftp_tx_handler_hex
#else
        FNET_NULL
#endif
    }
};


#define FAPP_TFTP_IMAGE_TYPE_SIZE  (sizeof(image_types)/sizeof(struct image_type))
#define FAPP_TFTP_IMAGE_TYPE_DEFAULT   image_types[0]  /* Default image type. */

/************************************************************************
* NAME: fapp_tftp_image_type_by_index
*
* DESCRIPTION:
************************************************************************/
const struct image_type *fapp_tftp_image_type_by_index (fapp_params_tftp_file_type_t index)
{
    const struct image_type   *result = &FAPP_TFTP_IMAGE_TYPE_DEFAULT;
    const struct image_type   *type = image_types;
    fnet_index_t        i;

    for(i = 0u; i < FAPP_TFTP_IMAGE_TYPE_SIZE; i++)
    {
        if( type->index == index )
        {
            result = type;
            break;
        }
        type++;
    }

    return result;
}

/************************************************************************
* NAME: fapp_tftp_image_type_by_index
*
* DESCRIPTION:
************************************************************************/
const struct image_type *fapp_tftp_image_type_by_name (fnet_char_t *name)
{
    const struct image_type   *result = 0;
    const struct image_type   *type = image_types;
    fnet_index_t        i;

    for(i = 0u; i < FAPP_TFTP_IMAGE_TYPE_SIZE; i++)
    {
        if(fnet_strcmp( type->name, name ) == 0)
        {
            result = type;
            break;
        }
        type++;
    }

    return result;
}

#endif

#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD

/************************************************************************
* NAME: fapp_tftp_tx_image_begin_end
*
* DESCRIPTION:
************************************************************************/
static void fapp_tftp_tx_image_begin_end(fnet_uint8_t *FNET_COMP_PACKED_VAR *data_begin_p, fnet_uint8_t *FNET_COMP_PACKED_VAR *data_end_p)
{
    fnet_uint8_t    *data_end = (fnet_uint8_t *)FAPP_TFTP_TX_MEM_REGION->address + FAPP_TFTP_TX_MEM_REGION->size;
    fnet_uint8_t    *data_cur;
    fnet_size_t     step = FAPP_TFTP_TX_MEM_REGION->erase_size;

    /* Find image start. */
    data_cur = (fnet_uint8_t *)FAPP_TFTP_TX_MEM_REGION->address;
    while((data_cur < data_end) && (fapp_mem_region_is_protected((fnet_uint32_t)data_cur, step) == FNET_TRUE) )
    {
        data_cur += step;
    }
    *data_begin_p = data_cur;

    /* Find image end. */
    while((data_cur < data_end) && (fapp_mem_region_is_protected((fnet_uint32_t)data_cur, step) == FNET_FALSE) )
    {
        data_cur += step;
    }
    *data_end_p = data_cur;

}

/*======================== BIN ========================================*/
#if FAPP_CFG_TFTP_RX_BIN && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
/************************************************************************
* NAME: fapp_tftp_rx_handler_bin
*
* DESCRIPTION:
************************************************************************/
static fnet_return_t fapp_tftp_rx_handler_bin (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size)
{
    struct fapp_tftp_rx_handler_bin_context *bin = &tftp_handler->rx_bin;
    fnet_return_t result = FNET_OK;

    while(data_size && (result == FNET_OK))
    {
        switch(bin->state)
        {
            case FAPP_TFTP_HANDLER_BIN_STATE_GETDATA: /* Get data. */
            {
                fnet_size_t copy_size;

                copy_size = bin->header.size;
                if(copy_size > data_size)
                {
                    copy_size = data_size;
                }

                /* Copy to the destination. */
                if( (result = fapp_mem_memcpy (desc, (void *) bin->header.address, data, copy_size )) == FNET_OK )
                {
                    bin->header.address += copy_size;
                    bin->header.size -= copy_size;
                    data_size -= copy_size;
                    data += copy_size;

                    if(bin->header.size == 0u)
                    {
                        bin->state = FAPP_TFTP_HANDLER_BIN_STATE_INIT;
                    }
                }
            }
            break;
            case FAPP_TFTP_HANDLER_BIN_STATE_INIT:
                bin->state = FAPP_TFTP_HANDLER_BIN_STATE_GETHEADER;
                bin->header_index = 0u;
                break;
            case FAPP_TFTP_HANDLER_BIN_STATE_GETHEADER: /* Get header. */
                if(bin->header_index < FAPP_TFTP_BIN_HEADER_SIZE)
                {
                    bin->header_bytes[bin->header_index++] = *data++;
                    data_size--;
                }
                else
                {
                    bin->state = FAPP_TFTP_HANDLER_BIN_STATE_GETDATA;
                }
                break;
            default:
                break;
        }
    }

    return result;
}
#endif

#if FAPP_CFG_TFTP_TX_BIN && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
/************************************************************************
* NAME: fapp_tftp_tx_bin_gen
*
* DESCRIPTION:
************************************************************************/
static void fapp_tftp_tx_bin_gen (struct fapp_tftp_tx_handler_bin_context *tx_bin)
{
    fnet_size_t send_size;

    send_size = (fnet_size_t)(tx_bin->data_end - tx_bin->data_start);

    if(send_size > FAPP_TFTP_BIN_DATA_MAX)
    {
        send_size = FAPP_TFTP_BIN_DATA_MAX;
    }

    if(send_size)
    {
        /* Byte count.*/
        tx_bin->bin_line.count = send_size;
        /* Address.*/
        tx_bin->bin_line.address = (fnet_uint32_t)tx_bin->data_start;
        /* Data. */
        fnet_memcpy(tx_bin->bin_line.data, tx_bin->data_start, (fnet_size_t)send_size);
        tx_bin->data_start += send_size;

        tx_bin->bin_line_size = send_size + 4u + 4u; /* Save line size. */
    }
    else
    {
        tx_bin->bin_line_size = 0u; /* End of image. */
    }

    /* Reset line_cur.*/
    tx_bin->bin_line_cur = (fnet_uint8_t *) & (tx_bin->bin_line);

}

/************************************************************************
* NAME: fapp_tftp_tx_handler_bin
*
* DESCRIPTION:
************************************************************************/
static fnet_int32_t fapp_tftp_tx_handler_bin (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size)
{
    struct fapp_tftp_tx_handler_bin_context *tx_bin = &tftp_handler->tx_bin;
    fnet_size_t                             send_size;
    fnet_int32_t                            result = 0;

    FNET_COMP_UNUSED_ARG(desc);

    /* Define start and end address.*/
    if((tx_bin->data_start == 0) && (tx_bin->data_end == 0)) /* Only first time. */
    {
        fapp_tftp_tx_image_begin_end(&tx_bin->data_start, &tx_bin->data_end);
    }

    while(data_size)
    {
        if(tx_bin->bin_line_size == 0u)
        {
            /* Generate CW bin.*/
            fapp_tftp_tx_bin_gen (tx_bin);

            if(tx_bin->bin_line_size == 0u)
            {
                break; /* EOF */
            }
        }

        send_size = (fnet_size_t)tx_bin->bin_line_size;
        if(send_size > data_size)
        {
            send_size = data_size;
        }

        fnet_memcpy(data, tx_bin->bin_line_cur, send_size);

        data_size -= send_size;
        data += send_size;
        tx_bin->bin_line_cur += send_size;
        tx_bin->bin_line_size -= send_size;

        result += (fnet_int32_t)send_size;
    }

    return result;
}
#endif
/*======================== RAW ========================================*/

/************************************************************************
* NAME: fapp_tftp_rx_handler_raw
*
* DESCRIPTION:
************************************************************************/
#if FAPP_CFG_TFTP_RX_RAW && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
static fnet_return_t fapp_tftp_rx_handler_raw (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size)
{
    struct fapp_tftp_rx_handler_raw_context *raw = &tftp_handler->rx_raw;
    fnet_return_t                           result;

    if(raw->dest == 0u) /* Only one time. */
    {
        raw->dest = fapp_params_tftp_config.file_raw_address;
    }

    /* Copy to the destination. */
    result = fapp_mem_memcpy (desc, (void *)raw->dest, data, data_size );

    raw->dest += data_size;

    return result;
}
#endif

#if FAPP_CFG_TFTP_TX_RAW && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
/************************************************************************
* NAME: fapp_tftp_tx_raw_gen
*
* DESCRIPTION:
************************************************************************/
static /* inline */ void fapp_tftp_tx_raw_gen (struct fapp_tftp_tx_handler_raw_context *tx_raw)
{
    fnet_size_t send_size;

    send_size = (fnet_size_t)(tx_raw->data_end - tx_raw->data_start);

    if(send_size > FAPP_TFTP_RAW_DATA_MAX)
    {
        send_size = FAPP_TFTP_RAW_DATA_MAX;
    }

    if(send_size)
    {
        fnet_memcpy(tx_raw->data, tx_raw->data_start, (fnet_size_t)send_size);
        tx_raw->data_start += send_size;
    }

    tx_raw->data_size = send_size; /* Save line size. */

    /* Reset line_cur.*/
    tx_raw->data_cur = tx_raw->data;

}

/************************************************************************
* NAME: fapp_tftp_tx_handler_raw
*
* DESCRIPTION:
************************************************************************/
static fnet_int32_t fapp_tftp_tx_handler_raw (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size)
{
    struct fapp_tftp_tx_handler_raw_context *tx_raw = &tftp_handler->tx_raw;
    fnet_size_t                             send_size;
    fnet_int32_t                            result = 0;

    FNET_COMP_UNUSED_ARG(desc);

    /* Define start and end address.*/
    if((tx_raw->data_start == 0) && (tx_raw->data_end == 0)) /* Only first time. */
    {
        fapp_tftp_tx_image_begin_end(&tx_raw->data_start, &tx_raw->data_end);
    }

    while(data_size)
    {
        /* Generate raw binary.*/
        if(tx_raw->data_size == 0u)
        {
            fapp_tftp_tx_raw_gen (tx_raw);

            if(tx_raw->data_size == 0u)
            {
                break; /* EOF. */
            }
        }

        send_size = (fnet_size_t)tx_raw->data_size;
        if(send_size > data_size)
        {
            send_size = data_size;
        }

        fnet_memcpy(data, tx_raw->data_cur, send_size);

        data_size -= send_size;
        data += send_size;
        tx_raw->data_cur += send_size;
        tx_raw->data_size -= send_size;

        result += (fnet_int32_t)send_size;
    }

    return result;
}
#endif

/*======================== SREC ========================================*/
#if FAPP_CFG_TFTP_RX_SREC && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
/************************************************************************
* NAME: fapp_tftp_rx_handler_srec
*
* DESCRIPTION:
************************************************************************/
static fnet_return_t fapp_tftp_rx_handler_srec (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size)
{
    struct fapp_tftp_rx_handler_srec_context    *srec = &tftp_handler->rx_srec;
    fnet_return_t                               result = FNET_OK;
    fnet_char_t                                 tmp[2];
    fnet_uint8_t                                tmp_data;
    fnet_char_t                                 *p = 0;
    fnet_index_t                                i;
    fnet_uint8_t                                checksum;

    tmp[1] = '\0';

    while(data_size && (result == FNET_OK))
    {
        switch(srec->state)
        {
            case FAPP_TFTP_RX_HANDLER_SREC_STATE_INIT:
                srec->state = FAPP_TFTP_RX_HANDLER_SREC_STATE_GETSTART;
                break;
            case FAPP_TFTP_RX_HANDLER_SREC_STATE_GETSTART:
                if(*data++ == 'S')
                {
                    srec->state = FAPP_TFTP_RX_HANDLER_SREC_STATE_GETTYPE;
                }
                data_size--;
                break;
            case FAPP_TFTP_RX_HANDLER_SREC_STATE_GETTYPE:
                srec->type = *data++;
                data_size--;
                srec->record_hex_index = 0u; /* Reset hex index. */
                srec->record.count = 0xFFu; /* Trick. */
                srec->state = FAPP_TFTP_RX_HANDLER_SREC_STATE_GETDATA;
                break;
            case FAPP_TFTP_RX_HANDLER_SREC_STATE_GETDATA:
                tmp[0] = *data++;
                data_size--;
                tmp_data = (fnet_uint8_t)fnet_strtoul(tmp, &p, 16u); /* Char to integer.*/
                if ((tmp_data == 0u) && (p == tmp))
                {
                    result = FNET_ERR;
                    break;
                }

                srec->record_bytes[srec->record_hex_index >> 1] = (fnet_uint8_t)((srec->record_bytes[srec->record_hex_index >> 1] & (0xFu << (4u * (srec->record_hex_index % 2u))))
                        + (fnet_uint8_t)(tmp_data << (4u * ((srec->record_hex_index + 1u) % 2u))));

                if(srec->record_hex_index > (((fnet_index_t)srec->record.count << 1u)))
                {
                    fnet_uint8_t type;
                    fnet_uint8_t *addr = 0;

                    /* Check checksum. */
                    checksum = 0u;
                    for(i = 0u; i < srec->record.count; i++)
                    {
                        checksum += srec->record_bytes[i];
                    }

                    if(srec->record_bytes[srec->record.count] != (fnet_uint8_t)~checksum)
                    {
                        result = FNET_ERR;
                        fnet_shell_println(desc, FAPP_TFTP_CHECKSUM_ERR);
                        break;
                    }

                    /* Handle S[type].*/
                    type = (fnet_uint8_t)(srec->type - '0'); /* Convert from character to number.*/
                    if((type == 1u) || (type == 9u))
                    {
                        addr = (fnet_uint8_t *)( ((fnet_uint32_t)((srec->record.data[0]) & 0xFFLU) << 8 ) + (fnet_uint32_t)((srec->record.data[1]) & 0xFFLU));

                    }

                    if((type == 2u) || (type == 8u))
                    {
                        addr = (fnet_uint8_t *)( ((fnet_uint32_t)((srec->record.data[0]) & 0xFFLU) << 16) +
                                                 ((fnet_uint32_t)((srec->record.data[1]) & 0xFFLU) << 8 ) + (fnet_uint32_t)((srec->record.data[2]) & 0xFFLU));

                    }

                    if((type == 3u) || (type == 7u))
                    {
                        addr = (fnet_uint8_t *)(((fnet_uint32_t)((srec->record.data[0]) & 0xFFLU) << 24) + ((fnet_uint32_t)((srec->record.data[1]) & 0xFFLU) << 16) +
                                                ((fnet_uint32_t)((srec->record.data[2]) & 0xFFLU) << 8 ) + (fnet_uint32_t)((srec->record.data[3]) & 0xFFLU));
                    }

                    if((type > 0u) && (type < 4u)) /* Data sequence. */
                    {
                        /* Copy data to the destination. */
                        result = fapp_mem_memcpy (desc, (void *)addr, srec->record.data + (1u + type), (fnet_size_t)(srec->record.count - (2u + type)) );
                    }

                    if((type > 6u) && (type < 10u)) /* End of block. */
                    {
                        /* Set entry point. */
                        fapp_params_boot_config.go_address = (fnet_uint32_t)addr;
                        fnet_shell_println(desc, FAPP_TFTP_ENTRYPOINT_STR, addr);
                    }

                    srec->state = FAPP_TFTP_RX_HANDLER_SREC_STATE_GETSTART;
                }

                srec->record_hex_index++;
                break;
            default: /*Wrong state*/
                break;
        }
    }

    return result;
}
#endif

#if FAPP_CFG_TFTP_TX_SREC && (FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD)
/************************************************************************
* NAME: fapp_tftp_tx_srec_gen
*
* DESCRIPTION:
************************************************************************/
static /*inline*/ void fapp_tftp_tx_srec_gen (struct fapp_tftp_tx_handler_srec_context *tx_srec)
{
    fnet_uint8_t    checksum;
    fnet_size_t     send_size;
    fnet_index_t    i;

    switch(tx_srec->state)
    {
        /* === Block header.===*/
        case FAPP_TFTP_TX_HANDLER_SREC_STATE_INIT:
            fnet_sprintf((fnet_char_t *)(&tx_srec->srec_line.S), "S0030000FC\n");
            tx_srec->state = FAPP_TFTP_TX_HANDLER_SREC_STATE_DATA;
            break;
        /* === Data sequence. ===*/
        case FAPP_TFTP_TX_HANDLER_SREC_STATE_DATA:

            send_size = (fnet_size_t)(tx_srec->data_end - tx_srec->data_start);

            fnet_sprintf((fnet_char_t *)(&tx_srec->srec_line.S), "S3");

            if(send_size > FAPP_TFTP_SREC_DATA_MAX)
            {
                send_size = FAPP_TFTP_SREC_DATA_MAX;
            }

            if(send_size)
            {
                /* Byte count.*/
                checksum = (fnet_uint8_t) (send_size + 4u/*Address*/ + 1u/*Checksum.*/);
                fnet_sprintf((fnet_char_t *)tx_srec->srec_line.count, "%02X", checksum);
                /* Address.*/
                fnet_sprintf((fnet_char_t *)tx_srec->srec_line.address, "%08X", tx_srec->data_start);
                /* Calculate checksum on address field. */
                checksum += (fnet_uint8_t)((fnet_uint32_t)tx_srec->data_start + ((fnet_uint32_t)tx_srec->data_start >> 8)
                                           + ((fnet_uint32_t)tx_srec->data_start >> 16) + ((fnet_uint32_t)tx_srec->data_start >> 24));
                /* Data. */
                for(i = 0u; i < send_size; i++)
                {
                    checksum += *tx_srec->data_start;
                    fnet_sprintf((fnet_char_t *)&tx_srec->srec_line.data[i], "%02X", *tx_srec->data_start);
                    tx_srec->data_start++;
                }
                /* Checksum.*/
                fnet_sprintf((fnet_char_t *)&tx_srec->srec_line.data[send_size], "%02X\n", (fnet_uint8_t)~checksum);
            }

            if(tx_srec->data_end == tx_srec->data_start) /* End of image.*/
            {
                tx_srec->state = FAPP_TFTP_TX_HANDLER_SREC_STATE_EOB;
            }
            break;
        /*=== End of block. ===*/
        case FAPP_TFTP_TX_HANDLER_SREC_STATE_EOB:
            fnet_sprintf((fnet_char_t *)(&tx_srec->srec_line.S), "S705");
            checksum = 5u;
            /* Starting address.*/
            fnet_sprintf((fnet_char_t *)tx_srec->srec_line.address, "%08X", fapp_params_boot_config.go_address);
            /* Calculate checksum on address field. */
            checksum += (fnet_uint8_t)(fapp_params_boot_config.go_address + (fapp_params_boot_config.go_address >> 8)
                                       + (fapp_params_boot_config.go_address >> 16) + (fapp_params_boot_config.go_address >> 24));
            /* Checksum.*/
            fnet_sprintf((fnet_char_t *)&tx_srec->srec_line.data[0], "%02X", (fnet_uint8_t)~checksum);
            tx_srec->state = FAPP_TFTP_TX_HANDLER_SREC_STATE_END;
            break;
        /* EOF*/
        case FAPP_TFTP_TX_HANDLER_SREC_STATE_END:
            tx_srec->srec_line.S = 0u;
            break;
        default:
            break;
    }

    /* Save line size. */
    tx_srec->srec_line_size = fnet_strlen((fnet_char_t *)(&tx_srec->srec_line));
    /* Reset line_cur.*/
    tx_srec->srec_line_cur = (fnet_uint8_t *) & (tx_srec->srec_line);
}



/************************************************************************
* NAME: fapp_tftp_tx_handler_srec
*
* DESCRIPTION:
************************************************************************/
static fnet_int32_t fapp_tftp_tx_handler_srec (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size)
{
    struct fapp_tftp_tx_handler_srec_context    *tx_srec = &tftp_handler->tx_srec;
    fnet_size_t                                 send_size;
    fnet_int32_t                                result = 0;

    FNET_COMP_UNUSED_ARG(desc);

    /* Define start and end address.*/
    if((tx_srec->data_start == 0) && (tx_srec->data_end == 0)) /* Only first time. */
    {
        fapp_tftp_tx_image_begin_end(&tx_srec->data_start, &tx_srec->data_end);
    }

    while(data_size)
    {
        if(tx_srec->srec_line_size == 0u)
        {
            /* Generate srec.*/
            fapp_tftp_tx_srec_gen (tx_srec);

            if(tx_srec->srec_line_size == 0u)
            {
                break; /*EOF*/
            }
        }

        send_size = (fnet_size_t)tx_srec->srec_line_size;
        if(send_size > data_size)
        {
            send_size = data_size;
        }

        fnet_memcpy(data, tx_srec->srec_line_cur, send_size);

        data_size -= send_size;
        data += send_size;
        tx_srec->srec_line_cur += send_size;
        tx_srec->srec_line_size -= send_size;

        result += (fnet_int32_t)send_size;
    }

    return result;
}
#endif /* FAPP_CFG_TFTP_TX_SREC */

/*======================== HEX ========================================*/
#if FAPP_CFG_TFTP_RX_HEX
/************************************************************************
* NAME: fapp_tftp_rx_handler_hex
*
* DESCRIPTION:
************************************************************************/
static fnet_return_t fapp_tftp_rx_handler_hex(fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size)
{
    struct fapp_tftp_rx_handler_hex_context *hex = &tftp_handler->rx_hex;
    fnet_return_t                           result = FNET_OK;
    fnet_uint32_t                           addr;
    fnet_uint8_t                            tmp_data;
    fnet_char_t                             tmp[2];
    fnet_char_t                             *p = 0;

    tmp[1] = '\0';

    while(data_size && (result == FNET_OK))
    {
        switch(hex->state)
        {
            case FAPP_TFTP_RX_HANDLER_HEX_STATE_INIT:
                hex->addr_base = 0u;
                hex->state = FAPP_TFTP_RX_HANDLER_HEX_STATE_GETCOLON;
                break;
            case FAPP_TFTP_RX_HANDLER_HEX_STATE_GETCOLON:
                /* skip all characters until colon */
                if(*data == ':')
                {
                    /* get ready for 8 bytes of head */
                    hex->csum = 0u;
                    hex->index = 0u;
                    hex->todo = 8u;
                    hex->state = FAPP_TFTP_RX_HANDLER_HEX_STATE_GETHEAD;
                }
                data++;
                data_size--;
                break;

            case FAPP_TFTP_RX_HANDLER_HEX_STATE_GETHEAD:
            case FAPP_TFTP_RX_HANDLER_HEX_STATE_GETDATA:
                tmp[0] = *data;
                tmp_data = (fnet_uint8_t)fnet_strtoul(tmp, &p, 16u); /* Char to integer.*/
                if ((tmp_data == 0u) && (p == tmp))
                {
                    result = FNET_ERR;
                    break;
                }

                if(hex->index % 2u == 0u)
                {
                    hex->data[hex->index / 2u] = tmp_data << 4;
                }
                else
                {
                    hex->data[hex->index / 2u] |= tmp_data;
                    hex->csum += hex->data[hex->index / 2u];
                }

                hex->index++;
                hex->todo--;
                data++;
                data_size--;

                if(hex->todo == 0u)
                {
                    /* head is complete */
                    if(hex->state == FAPP_TFTP_RX_HANDLER_HEX_STATE_GETHEAD)
                    {
                        hex->state = FAPP_TFTP_RX_HANDLER_HEX_STATE_GETDATA;
                        hex->bcount = hex->data[0];     /* data bytes count */
                        hex->todo = (hex->bcount + 1u) * 2u; /* data+checksum, each byte is two characters */
                        hex->type = hex->data[3];       /* type is at head[3] */
                        hex->addr = (fnet_uint16_t)(((fnet_uint16_t)hex->data[1]) << 8u) | (fnet_uint16_t)hex->data[2];
                        /* data part will be received from the beginning */
                        hex->index = 0u;
                    }
                    /* data complete (also with checksum) */
                    else
                    {
                        hex->state = FAPP_TFTP_RX_HANDLER_HEX_STATE_GETCOLON;

                        if(hex->csum != 0u)
                        {
                            result = FNET_ERR;
                            fnet_shell_println(desc, FAPP_TFTP_CHECKSUM_ERR);
                            break;
                        }

                        switch(hex->type)
                        {
                            case 0x00: /* data, copy to destination, apply region offset */
                                addr = (hex->addr_base | hex->addr);
                                result = fapp_mem_memcpy (desc, (void *)addr, hex->data, (fnet_size_t)hex->bcount);
                                break;
                            case 0x01: /* end of file */
                                hex->state = FAPP_TFTP_RX_HANDLER_HEX_STATE_IGNOREALL;
                                break;
                            case 0x04: /* extended linear address */
                                hex->addr_base = (((fnet_uint32_t)hex->data[0]) << 24) | (((fnet_uint32_t)hex->data[1]) << 16);
                                break;
                            default:
                                fnet_shell_println(desc, FAPP_TFTP_RECORD_ERR);
                                break;
                        }
                    }
                }
                break;

            case FAPP_TFTP_RX_HANDLER_HEX_STATE_IGNOREALL:
                data_size = 0u;
                break;
            default:
                break;
        }
    }

    return result;
}
#endif

#if FAPP_CFG_TFTP_TX_HEX
/************************************************************************
* NAME: fapp_tftp_tx_hex_gen
*
* DESCRIPTION:
************************************************************************/
static void fapp_tftp_tx_hex_gen (struct fapp_tftp_tx_handler_hex_context *tx_hex)
{
    fnet_uint8_t checksum;
    fnet_uint32_t send_size;
    fnet_uint32_t max_size;
    fnet_index_t i = 0u;

    switch(tx_hex->state)
    {
        /* === initial (address).===*/
        case FAPP_TFTP_TX_HANDLER_HEX_STATE_INIT:
        case FAPP_TFTP_TX_HANDLER_HEX_STATE_EXADDR:
            fnet_sprintf((fnet_char_t *)tx_hex->hex_line.head, ":02" "0000" "04" "%04X", (fnet_uint32_t)tx_hex->data_start >> 16u);
            checksum = (fnet_uint8_t)(2u + 0u + 4u + (((fnet_uint32_t)tx_hex->data_start >> 16u) & 0xffu) + (((fnet_uint32_t)tx_hex->data_start >> 24u) & 0xffu));
            fnet_sprintf((fnet_char_t *)&tx_hex->hex_line.data[2], "%02X\n", (fnet_uint8_t)(~checksum + 1u));
            tx_hex->state = FAPP_TFTP_TX_HANDLER_HEX_STATE_DATA;
            break;

        /* === Data sequence. ===*/
        case FAPP_TFTP_TX_HANDLER_HEX_STATE_DATA:
            send_size = (fnet_uint32_t)tx_hex->data_end - (fnet_uint32_t)tx_hex->data_start;

            if(send_size > FAPP_TFTP_HEX_DATA_MAX)
            {
                send_size = FAPP_TFTP_HEX_DATA_MAX;
            }
            /* one line should not cross 16bit address segment */
            max_size = 0x10000u - ((fnet_uint32_t)tx_hex->data_start & 0xffffu);
            if(send_size > max_size)
            {
                send_size = max_size;
            }

            fnet_sprintf((fnet_char_t *)tx_hex->hex_line.head, ":%02X" "%04X" "00", send_size, (fnet_uint32_t)tx_hex->data_start & 0xffffu);
            checksum = (fnet_uint8_t)(send_size + (((fnet_uint32_t)tx_hex->data_start >> 8u) & 0xffu) + (((fnet_uint32_t)tx_hex->data_start) & 0xffu) + 0u);

            if(send_size)
            {
                /* Data string. */
                for(i = 0u; i < send_size; i++)
                {
                    fnet_sprintf((char *)&tx_hex->hex_line.data[i], "%02X", *tx_hex->data_start);
                    checksum += *tx_hex->data_start;
                    tx_hex->data_start++;
                }
                fnet_sprintf((char *)&tx_hex->hex_line.data[send_size], "%02X\n", (fnet_uint8_t)(~checksum + 1u));
            }

            if(tx_hex->data_end == tx_hex->data_start) /* End of image.*/
            {
                tx_hex->state = FAPP_TFTP_TX_HANDLER_HEX_STATE_EOB;
            }
            else if((((unsigned long)tx_hex->data_start) & 0xffffu) == 0u) /* Start new segment */
            {
                tx_hex->state = FAPP_TFTP_TX_HANDLER_HEX_STATE_EXADDR;
            }
            else
            {}
            break;

        /*=== EOF block. ===*/
        case FAPP_TFTP_TX_HANDLER_HEX_STATE_EOB:
            fnet_sprintf((char *)tx_hex->hex_line.head, ":00" "0000" "01" "FF\n");
            tx_hex->state = FAPP_TFTP_TX_HANDLER_HEX_STATE_END;
            break;

        /* EOF*/
        case FAPP_TFTP_TX_HANDLER_HEX_STATE_END:
            tx_hex->hex_line.head[0] = 0u;
            break;
        default:
            break;
    }

    /* Save line size. */
    tx_hex->hex_line_size = fnet_strlen((char *)(&tx_hex->hex_line));
    /* Reset line_cur.*/
    tx_hex->hex_line_cur = (fnet_uint8_t *) & (tx_hex->hex_line);
}


/************************************************************************
* NAME: fapp_tftp_tx_handler_hex
*
* DESCRIPTION:
************************************************************************/
static fnet_int32_t fapp_tftp_tx_handler_hex (fapp_tftp_handler_control_t *tftp_handler, fnet_shell_desc_t desc, fnet_uint8_t *data, fnet_size_t data_size)
{
    struct fapp_tftp_tx_handler_hex_context *tx_hex = &tftp_handler->tx_hex;
    fnet_size_t send_size;
    fnet_int32_t result = 0;

    FNET_COMP_UNUSED_ARG(desc);

    /* Define start and end address.*/
    if((tx_hex->data_start == 0) && (tx_hex->data_end == 0)) /* Only first time. */
    {
        fapp_tftp_tx_image_begin_end(&tx_hex->data_start, &tx_hex->data_end);
    }

    while(data_size)
    {
        if(tx_hex->hex_line_size == 0u)
        {
            /* Generate srec.*/
            fapp_tftp_tx_hex_gen (tx_hex);

            if(tx_hex->hex_line_size == 0u)
            {
                break; /*EOF*/
            }
        }

        send_size = (unsigned long)tx_hex->hex_line_size;
        if(send_size > data_size)
        {
            send_size = data_size;
        }

        fnet_memcpy(data, tx_hex->hex_line_cur, send_size);

        data_size -= send_size;
        data += send_size;
        tx_hex->hex_line_cur += send_size;
        tx_hex->hex_line_size -= send_size;

        result += (fnet_int32_t)send_size;
    }

    return result;
}
#endif /* FAPP_CFG_TFTP_TX_HEX */


#endif /* FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD || FAPP_CFG_TFTPS_CMD*/


#if FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD
/************************************************************************
* NAME: fapp_tftp_handler
*
* DESCRIPTION: TFTP RX and TX handler.
************************************************************************/
static fnet_int32_t fapp_tftp_handler (fnet_tftp_request_t request_type, fnet_uint8_t *data_ptr, fnet_size_t data_size, fnet_return_t result, void *shl_desc)
{
    fnet_shell_desc_t   desc = (fnet_shell_desc_t)shl_desc;
    fnet_int32_t        result_local;

    if(result == FNET_OK)
    {
        /* Print image download progress. */
        fnet_shell_putchar(desc, '\b');
        fnet_shell_putchar(desc, progress[progress_counter++]); /* Print progress. */
        progress_counter &= 0x3u;

        /* -- FNET_TFTP_REQUEST_READ -- */
        if(request_type == FNET_TFTP_REQUEST_READ)
        {
            result_local = fapp_tftp_handler_control.current_type->rx_handler(&fapp_tftp_handler_control, desc, data_ptr, data_size);
        }
        /* -- FNET_TFTP_REQUEST_WRITE -- */
        else
        {
#if 0
            /* Just Hello World example, only for debug.*/
            result_local = fnet_sprintf((fnet_uint8_t *)data_ptr, "Hello World!\n");
#else
            result_local = (fnet_int32_t)fapp_tftp_handler_control.current_type->tx_handler(&fapp_tftp_handler_control, desc, data_ptr, data_size);
#endif
            data_size = (fnet_size_t)result_local;
        }

        if(result_local == FNET_ERR)
        {
            goto ERROR;
        }
        else
        {
            fapp_tftp_handler_control.image_size += data_size;

            /* Check EOF. */
            if(data_size < FNET_TFTP_DATA_SIZE_MAX)
            {
                fnet_shell_println(desc, FAPP_TFTP_COMPLETED_STR, fapp_tftp_handler_control.image_size);
                fnet_shell_unblock(desc); /* Unblock shell. */
            }
        }
    }
    else
    {
    ERROR:
        fnet_shell_println(desc, FAPP_TFTP_ERR, data_size, data_ptr );
        fnet_shell_unblock(desc);           /* Unblock shell. */
        fnet_shell_script_release(desc);    /* Clear script. */
        result_local = FNET_ERR;
    }

    return result_local;
}

/************************************************************************
* NAME: fapp_tftp_on_ctrlc
*
* DESCRIPTION:
************************************************************************/
static void fapp_tftp_on_ctrlc(fnet_shell_desc_t desc)
{
    /* Release TFTP. */
    fnet_tftp_cln_release();
    fnet_shell_putchar(desc, '\b'); /* Clear progress. */
    fnet_shell_println(desc, FAPP_CANCELLED_STR);
    fnet_shell_script_release(desc);   /* Clear script. */
}

/************************************************************************
* NAME: fapp_tftp_cmd
*
* DESCRIPTION: Start TFTP-client loader/uploader.
************************************************************************/
void fapp_tftp_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_tftp_cln_params     tftp_params;
    fnet_char_t                    ip_str[FNET_IP_ADDR_STR_SIZE];

    /* Set parameters of the TFTP-client service. */

    /* "tftpup" TFTP firmware uploader. */
    if(fnet_strcasecmp("tftpup", argv[0]) == 0)
    {
        tftp_params.request_type = FNET_TFTP_REQUEST_WRITE; /* TFTP write request. */
    }
    else
    {
        /* "tftp" TFTP firmware loader. */
        tftp_params.request_type = FNET_TFTP_REQUEST_READ; /* TFTP read request. */
    }

    tftp_params.handler = fapp_tftp_handler;
    tftp_params.file_name = fapp_params_tftp_config.file_name;

    /* TFTP Server IP address. */
    fnet_memcpy(&tftp_params.server_addr, &fapp_params_tftp_config.server_addr, sizeof(tftp_params.server_addr));

    tftp_params.handler_param = (void *)desc;
    tftp_params.timeout = 0u; /* Default timeout. */

    /* Reset fapp_tftp_handler_control. */
    fnet_memset_zero(&fapp_tftp_handler_control, sizeof(fapp_tftp_handler_control));

    fapp_tftp_handler_control.current_type = fapp_tftp_image_type_by_index(fapp_params_tftp_config.file_type); /* Get type. */

    /* Change default parameters. */
    if(argc >= 2u)
    {
        tftp_params.file_name = argv[1];

        if(argc >= 3u) /* image name */
        {
            if(fnet_inet_ptos(argv[2], &tftp_params.server_addr) == FNET_ERR)
            {
                fnet_shell_println(desc, FAPP_PARAM_ERR, argv[2]);   /* Wrong TFTP Server IP address. */
                fnet_shell_script_release(desc);                    /* Clear script. */
                return;
            }

            if((argc >= 4u) && ((fapp_tftp_handler_control.current_type = fapp_tftp_image_type_by_name (argv[3])) == 0)) /* image type */
            {
                fnet_shell_println(desc, FAPP_PARAM_ERR, argv[3]); /* Wrong image type. */
                fnet_shell_script_release(desc);   /* Clear script. */
                return;
            }
        }
    }

    if((fapp_tftp_handler_control.current_type) &&
       (((tftp_params.request_type == FNET_TFTP_REQUEST_WRITE) && (fapp_tftp_handler_control.current_type->tx_handler)) ||
        ((tftp_params.request_type == FNET_TFTP_REQUEST_READ) && (fapp_tftp_handler_control.current_type->rx_handler))) &&
       (fnet_tftp_cln_init( &tftp_params ) != FNET_ERR))
    {
        fnet_shell_println(desc, FAPP_TOCANCEL_STR);
        fnet_shell_printf(desc, ((tftp_params.request_type == FNET_TFTP_REQUEST_READ) ? FAPP_TFTP_RX_HEADER_STR : FAPP_TFTP_TX_HEADER_STR),
                          tftp_params.file_name, fapp_tftp_handler_control.current_type->name,
                          fnet_inet_ntop(tftp_params.server_addr.sa_family, tftp_params.server_addr.sa_data, ip_str, sizeof(ip_str)) );

        fnet_shell_block(desc, fapp_tftp_on_ctrlc);
    }
    else
    {
        fnet_shell_println(desc, FAPP_INIT_ERR, "TFTP");
        fnet_shell_script_release(desc);   /* Critical error. Clear script. */
    }
}


#endif /*FAPP_CFG_TFTP_CMD || FAPP_CFG_TFTPUP_CMD*/




/********************* TFTP firmware server *****************************************/
#if FAPP_CFG_TFTPS_CMD

#define FAPP_TFTPS_ERROR_BUSY "Busy"
/************************************************************************
* NAME: fapp_tftps_request_handler
*
* DESCRIPTION: TFTP server request handler.
*************************************************************************/
static fnet_return_t fapp_tftps_request_handler(fnet_tftp_request_t request_type,
        const struct sockaddr *address,
        fnet_char_t *filename,         /* null-terminated.*/
        fnet_char_t *mode,             /* null-terminated.*/
        fnet_tftp_error_t *error_code,     /* optional default is 0) error code [0-7] RFC783, if result==FNET_ERR*/
        fnet_char_t * *error_message, /* optinal, if result==FNET_ERR*/
        void *shl_desc)
{
    fnet_return_t       result;
    fnet_shell_desc_t   desc = (fnet_shell_desc_t)shl_desc;
    fnet_char_t         ip_str[FNET_IP_ADDR_STR_SIZE];

    FNET_COMP_UNUSED_ARG(mode);
    FNET_COMP_UNUSED_ARG(error_message);

    /* Check Filename for firmware (up)load. */
    if(fnet_strcmp(fapp_params_tftp_config.file_name, filename) == 0)
    {
        /* Reset fapp_tftps_handler_control. */
        fnet_memset_zero(&fapp_tftps_handler_control, sizeof(fapp_tftps_handler_control));

        /* Get  handler type. */
        fapp_tftps_handler_control.current_type = fapp_tftp_image_type_by_index(fapp_params_tftp_config.file_type);

        if((fapp_tftps_handler_control.current_type) &&
           (((request_type == FNET_TFTP_REQUEST_WRITE) && (fapp_tftps_handler_control.current_type->rx_handler)) ||
            ((request_type == FNET_TFTP_REQUEST_READ) && (fapp_tftps_handler_control.current_type->tx_handler))))
        {
            fnet_shell_println(desc, (request_type == FNET_TFTP_REQUEST_READ) ? FAPP_TFTP_TX_HEADER_STR : FAPP_TFTP_RX_HEADER_STR,
                               filename, fapp_tftps_handler_control.current_type->name,
                               fnet_inet_ntop(address->sa_family, (fnet_uint8_t *)(address->sa_data), ip_str, sizeof(ip_str)) );

            /* Do erase all, on WRITE request.*/
            if(request_type == FNET_TFTP_REQUEST_WRITE)
            {
                fnet_shell_script(desc, FAPP_CFG_TFTPS_ON_WRITE_REQUEST_SCRIPT);
            }

            result = FNET_OK;
        }
        else
        {
            *error_code = FNET_TFTP_ERROR_ILLEGAL_OPERATION;
            result = FNET_ERR;
        }

    }
    else
    {
        *error_code = FNET_TFTP_ERROR_FILE_NOT_FOUND;
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* NAME: fapp_tftps_data_handler
*
* DESCRIPTION: TFTP server data handler.
*************************************************************************/
static fnet_int32_t fapp_tftps_data_handler(fnet_tftp_request_t request,
        fnet_uint8_t *data_ptr,
        fnet_size_t data_size,
        fnet_tftp_error_t *error_code,     /* error code [0-7] RFC783, if result==FNET_ERR*/
        fnet_char_t * *error_message, /* optinal, if result==FNET_ERR*/
        void *shl_desc)
{
    fnet_shell_desc_t   desc = (fnet_shell_desc_t)shl_desc;
    fnet_int32_t        result;

    FNET_COMP_UNUSED_ARG(error_code);
    FNET_COMP_UNUSED_ARG(error_message);

    /* REQUEST_WRITE */
    if(request == FNET_TFTP_REQUEST_WRITE)
    {
        if((result = fapp_tftps_handler_control.current_type->rx_handler(&fapp_tftps_handler_control, desc, data_ptr, data_size)) == FNET_ERR)
        {
            fnet_shell_println(desc, FAPP_TFTP_FAILED);
        }
        else
        {
            fapp_tftps_handler_control.image_size += data_size;

            /* Check EOF. */
            if(data_size < FNET_TFTP_DATA_SIZE_MAX)
            {
                fnet_shell_println(desc, FAPP_TFTP_COMPLETED_STR, fapp_tftps_handler_control.image_size);

                /* Set "go" state and save settings..*/
                fnet_shell_script(desc, FAPP_CFG_TFTPS_AFTER_WRITE_REQUEST_SCRIPT);
            }
        }
    }
    /* REQUEST_READ */
    else
    {

#if 0       /* Just Hello World example, only for debug.*/
        result = fnet_sprintf((fnet_uint8_t *)data_ptr, "Hello World!\n");
        fnet_shell_ptintln(desc, "Completed.");
#else
        if((result = (fnet_int32_t)fapp_tftps_handler_control.current_type->tx_handler(&fapp_tftps_handler_control, desc, data_ptr, data_size)) == FNET_ERR)
        {
            fnet_shell_println(desc, FAPP_TFTP_FAILED);
        }
        else /* OK. */
        {
            fapp_tftps_handler_control.image_size += (fnet_size_t)result;

            /* Check EOF. */
            if(result < (fnet_int32_t)FNET_TFTP_DATA_SIZE_MAX)
            {
                fnet_shell_println(desc, FAPP_TFTP_COMPLETED_STR, fapp_tftps_handler_control.image_size);
            }
        }
#endif

    }

    return result;
}

/************************************************************************
* NAME: fapp_tftps_release
*
* DESCRIPTION: Releases TFTP server.
*************************************************************************/
void fapp_tftps_release(void)
{
    fnet_tftp_srv_release(fapp_tftp_srv_desc);
    fapp_tftp_srv_desc = 0;
}

/************************************************************************
* NAME: fapp_tftps_cmd
*
* DESCRIPTION: Run TFTP server.
*************************************************************************/
void fapp_tftps_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_tftp_srv_params params;
    fnet_tftp_srv_desc_t tftp_srv_desc;

    if(argc == 1u) /* By default, the argument is "init".*/
    {
        /* Reset parameters structure. */
        fnet_memset_zero(&params, sizeof(params));

        params.handler_param = (void *)desc;
        params.request_handler = fapp_tftps_request_handler;
        params.data_handler = fapp_tftps_data_handler;

        /* Enable TFTP server */
        tftp_srv_desc = fnet_tftp_srv_init(&params);
        if(tftp_srv_desc != FNET_ERR)
        {
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fnet_shell_println(desc, " TFTP Server started.");
            fapp_print_netif_addr(desc, AF_SUPPORTED, fnet_netif_get_default(), FNET_FALSE);
            fnet_shell_println(desc, FAPP_DELIMITER_STR);
            fapp_tftp_srv_desc = tftp_srv_desc;
        }
        else
        {
            fnet_shell_println(desc, FAPP_INIT_ERR, "TFTP Server");
        }

    }
    else if((argc == 2u) && (fnet_strcasecmp(&FAPP_COMMAND_RELEASE[0], argv[1]) == 0)) /* [release] */
    {
        fapp_tftps_release();
    }
    else
    {
        fnet_shell_println(desc, FAPP_PARAM_ERR, argv[1]);
    }
}

/************************************************************************
* NAME: fapp_tftps_info
*
* DESCRIPTION:
*************************************************************************/
void fapp_tftps_info(fnet_shell_desc_t desc)
{
    fnet_shell_println(desc, FAPP_SHELL_INFO_FORMAT_S, "TFTP Server", fapp_enabled_str[fnet_tftp_srv_enabled(fapp_tftp_srv_desc)]);
}

#endif /* FAPP_CFG_TFTPS_CMD */
