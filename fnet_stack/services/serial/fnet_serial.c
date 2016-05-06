/**************************************************************************
*
* Copyright 2011-2015 by Andrey Butok. FNET Community.
* Copyright 2008-2010 by Andrey Butok. Freescale Semiconductor, Inc.
* Copyright 2003 by Andrey Butok. Motorola SPS.
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
* @file fnet_serial.c
*
* @author Andrey Butok
*
* @brief FNET Serial Input and Output Library implementation.
*
***************************************************************************/

#include "fnet.h"

static fnet_size_t fnet_serial_printk_mknumstr( fnet_char_t *numstr, void *nump, fnet_bool_t neg, fnet_size_t radix );
static void fnet_serial_printk_pad( fnet_uint8_t c, fnet_serial_stream_t stream, fnet_size_t curlen, fnet_size_t field_width, fnet_size_t *count );
static void fnet_serial_buffer_putchar( fnet_index_t p_dest, fnet_char_t character );


/******************************************************************************
 * Stream descriptors associated with the serial ports.
 ******************************************************************************/
const struct fnet_serial_stream fnet_serial_stream_port0 =
{
    0,
    fnet_cpu_serial_putchar,
    fnet_cpu_serial_getchar,
    0
};

const struct fnet_serial_stream fnet_serial_stream_port1 =
{
    1,
    fnet_cpu_serial_putchar,
    fnet_cpu_serial_getchar,
    0
};

const struct fnet_serial_stream fnet_serial_stream_port2 =
{
    2,
    fnet_cpu_serial_putchar,
    fnet_cpu_serial_getchar,
    0
};

const struct fnet_serial_stream fnet_serial_stream_port3 =
{
    3,
    fnet_cpu_serial_putchar,
    fnet_cpu_serial_getchar,
    0
};

const struct fnet_serial_stream fnet_serial_stream_port4 =
{
    4,
    fnet_cpu_serial_putchar,
    fnet_cpu_serial_getchar,
    0
};

const struct fnet_serial_stream fnet_serial_stream_port5 =
{
    5,
    fnet_cpu_serial_putchar,
    fnet_cpu_serial_getchar,
    0
};

/********************************************************************/
void fnet_serial_putchar(fnet_serial_stream_t stream, fnet_char_t character)
{
    stream->putchar(stream->id, character);
}

/********************************************************************/
fnet_int32_t fnet_serial_getchar(fnet_serial_stream_t stream)
{
    return stream->getchar(stream->id);
}

/********************************************************************/
void fnet_serial_flush(fnet_serial_stream_t stream)
{
    if(stream->flush)
    {
        stream->flush(stream->id);
    }
}

/*********************************************************************
 * fnet_prinf & fnet_sprintf staff
 *
 ********************************************************************/

/********************************************************************/

#define FNET_SERIAL_FLAGS_MINUS         (0x01u)
#define FNET_SERIAL_FLAGS_PLUS          (0x02u)
#define FNET_SERIAL_FLAGS_SPACE         (0x04u)
#define FNET_SERIAL_FLAGS_ZERO          (0x08u)
#define FNET_SERIAL_FLAGS_POUND         (0x10u)

#define FNET_SERIAL_IS_FLAG_MINUS(a)    (((a) & FNET_SERIAL_FLAGS_MINUS)!=0u)
#define FNET_SERIAL_IS_FLAG_PLUS(a)     (((a) & FNET_SERIAL_FLAGS_PLUS)!=0u)
#define FNET_SERIAL_IS_FLAG_SPACE(a)    (((a) & FNET_SERIAL_FLAGS_SPACE)!=0u)
#define FNET_SERIAL_IS_FLAG_ZERO(a)     (((a) & FNET_SERIAL_FLAGS_ZERO)!=0u)
#define FNET_SERIAL_IS_FLAG_POUND(a)    (((a) & FNET_SERIAL_FLAGS_POUND)!=0u)

/********************************************************************/
static fnet_size_t fnet_serial_printk_mknumstr( fnet_char_t *numstr, void *nump, fnet_bool_t neg, fnet_size_t radix )
{
    fnet_int32_t    a, b, c;
    fnet_uint32_t   ua, ub, uc;
    fnet_size_t     nlen;
    fnet_char_t     *nstrp;

    nlen = 0u;
    nstrp = numstr;
    *nstrp++ = '\0';

    if(neg == FNET_TRUE)
    {
        a = *(fnet_int32_t *)nump;

        if(a == 0)
        {
            *nstrp = '0';
            ++nlen;
            goto DONE;
        }

        while(a != 0)
        {
            b = a / (fnet_int32_t)radix;
            c = a - (b * (fnet_int32_t)radix);

            if(c < 0)
            {
                c = (fnet_int32_t)(~(fnet_uint32_t)c + 1u + '0');
            }
            else
            {
                c = c + (fnet_int32_t)'0';
            }

            a = b;
            *nstrp++ = (fnet_uint8_t)c;
            ++nlen;
        }
    }
    else
    {
        ua = *(fnet_uint32_t *)nump;

        if(ua == 0u)
        {
            *nstrp = '0';
            ++nlen;
            goto DONE;
        }

        while(ua != 0u)
        {
            ub = ua / radix;
            uc = ua - (ub * radix);

            if(uc < 10u)
            {
                uc = uc + '0';
            }
            else
            {
                uc = uc - 10u + 'a';
            }

            ua = ub;
            *nstrp++ = (fnet_uint8_t)uc;
            ++nlen;
        }
    }

DONE:
    return nlen;
}

/********************************************************************/
static void fnet_serial_printk_pad(fnet_uint8_t c, fnet_serial_stream_t stream, fnet_size_t curlen, fnet_size_t field_width, fnet_size_t *count )

{
    fnet_size_t i;

    for (i = curlen; i < field_width; i++)
    {
        (*count)++;
        fnet_serial_putchar(stream, c);
    }
}

/********************************************************************/
fnet_size_t fnet_serial_vprintf(fnet_serial_stream_t stream, const fnet_char_t *format, va_list arg )
{
    const fnet_char_t   *p;
    fnet_char_t         c;
    fnet_char_t         vstr[33];
    fnet_char_t         *vstrp;
    fnet_size_t         vlen = 0u;
    fnet_bool_t         done;
    fnet_size_t         count = 0u;
    fnet_flag_t         flags_used;
    fnet_size_t         field_width;
    fnet_bool_t         cont_xd = FNET_FALSE;
    fnet_bool_t         cont_u = FNET_FALSE;

#if 0
    fnet_bool_t precision_used;
    fnet_size_t precision_width;
    fnet_int32_t length_modifier;
#endif

    fnet_int32_t        ival;
    fnet_char_t         schar;
    fnet_bool_t         dschar;
    fnet_int32_t        *ivalp;
    fnet_char_t         *sval;
    fnet_char_t         cval;
    fnet_uint32_t       uval;

    /*
     * Start parsing apart the format string and display appropriate
     * formats and data.
     */
    for (p = format; (c = *p) != '\0'; p++)
    {
        /*
         * All formats begin with a '%' marker.  Special chars like
         * '\n' or '\t' are normally converted to the appropriate
         * character by the __compiler__.  Thus, no need for this
         * routine to account for the '\' character.
         */
        if(c != '%')
        {

#if FNET_CFG_SERIAL_PRINTF_N_TO_RN
            if(c == '\n') /* LF.*/
            {
                count++;
                fnet_serial_putchar(stream, '\r' /* CR */);
            }
#endif

            count++;
            fnet_serial_putchar(stream, c);

            continue;
        }

        /*
         * First check for specification modifier flags.
         */
        flags_used = 0u;
        done = FNET_FALSE;

        while(done == FNET_FALSE)
        {
            switch( /* c = */*++p)
            {
                case '-':
                    flags_used |= FNET_SERIAL_FLAGS_MINUS;
                    break;

                case '+':
                    flags_used |= FNET_SERIAL_FLAGS_PLUS;
                    break;

                case ' ':
                    flags_used |= FNET_SERIAL_FLAGS_SPACE;
                    break;

                case '0':
                    flags_used |= FNET_SERIAL_FLAGS_ZERO;
                    break;

                case '#':
                    flags_used |= FNET_SERIAL_FLAGS_POUND;
                    break;

                default:
                    /* we've gone one char too far */
                    --p;
                    done = FNET_TRUE;
                    break;
            }
        }

        /*
         * Next check for minimum field width.
         */
        field_width = 0u;
        done = FNET_FALSE;

        while(done == FNET_FALSE)
        {
            c = *++p;
            if((c >= '0') && (c <= '9'))
            {
                field_width = (field_width * 10u) + (fnet_size_t)(c - '0');
            }
            else
            {
                /* we've gone one char too far */
                --p;
                done = FNET_TRUE;
            }
        }

        /*
         * Next check for the width and precision field separator.
         */
        if( /* (c = *++p) */*++p == '.')
        {
            /*
             * Must get precision field width, if present.
             */
            done = FNET_FALSE;

            while(done == FNET_FALSE)
            {
                c = (*++p);
                if((c >= '0') && (c <= '9'))
                {
                }
                else
                {
                    /* we've gone one char too far */
                    --p;
                    done = FNET_TRUE;
                }
            }
        }
        else
        {
            /* we've gone one char too far */
            --p;
        }

        /*
         * Check for the length modifier.
         */
        switch( *++p)
        {
            case 'h':
                break;
            case 'l':
                break;
            case 'L':
                break;
            default:
                /* we've gone one char too far */
                --p;
                break;
        }

        /*
         * Now we're ready to examine the format.
         */
        switch(c = (*++p))
        {
            case 'd':
            case 'i':
                ival = (fnet_int32_t)va_arg(arg, fnet_int32_t);
                vlen = fnet_serial_printk_mknumstr(vstr, &ival, FNET_TRUE, 10u);
                vstrp = &vstr[vlen];

                if(ival < 0)
                {
                    schar = '-';
                    ++vlen;
                }
                else
                {
                    if(FNET_SERIAL_IS_FLAG_PLUS(flags_used))
                    {
                        schar = '+';
                        ++vlen;
                    }
                    else
                    {
                        if(FNET_SERIAL_IS_FLAG_SPACE(flags_used))
                        {
                            schar = ' ';
                            ++vlen;
                        }
                        else
                        {
                            schar = '\0';
                        }
                    }
                }

                dschar = FNET_FALSE;

                /*
                * do the ZERO pad.
                */
                if(FNET_SERIAL_IS_FLAG_ZERO(flags_used))
                {
                    if(schar)
                    {
                        count++;
                        fnet_serial_putchar(stream, schar);
                    }

                    dschar = FNET_TRUE;

                    fnet_serial_printk_pad('0', stream, vlen, field_width, &count);

                    vlen = field_width;
                }
                else
                {
                    if(!FNET_SERIAL_IS_FLAG_MINUS(flags_used))
                    {
                        fnet_serial_printk_pad(' ', stream, vlen, field_width, &count);

                        if(schar)
                        {
                            count++;
                            fnet_serial_putchar(stream, schar);
                        }

                        dschar = FNET_TRUE;
                    }
                }

                /* the string was built in reverse order, now display in */
                /* correct order */
                if((dschar == FNET_FALSE) && schar)
                {
                    count++;
                    fnet_serial_putchar(stream, schar);
                }

                cont_xd = FNET_TRUE;
                break;
            case 'x':
            case 'X':
                uval = (fnet_uint32_t)va_arg(arg, fnet_uint32_t);
                vlen = fnet_serial_printk_mknumstr(vstr, &uval, FNET_FALSE, 16u);
                vstrp = &vstr[vlen];

                dschar = FNET_FALSE;

                if(FNET_SERIAL_IS_FLAG_ZERO(flags_used))
                {
                    if(FNET_SERIAL_IS_FLAG_POUND(flags_used))
                    {
                        count += 2u;
                        fnet_serial_putchar(stream, '0');
                        fnet_serial_putchar(stream, 'x');

                        dschar = FNET_TRUE;
                    }

                    fnet_serial_printk_pad('0', stream, vlen, field_width, &count);
                    vlen = field_width;
                }
                else
                {
                    if(!FNET_SERIAL_IS_FLAG_MINUS(flags_used))
                    {
                        if(FNET_SERIAL_IS_FLAG_POUND(flags_used))
                        {
                            vlen += 2u;
                        }

                        fnet_serial_printk_pad(' ', stream, vlen, field_width, &count);

                        if(FNET_SERIAL_IS_FLAG_POUND(flags_used))
                        {
                            count += 2u;
                            fnet_serial_putchar(stream, '0');
                            fnet_serial_putchar(stream, 'x');
                            dschar = FNET_TRUE;
                        }
                    }
                }

                if((FNET_SERIAL_IS_FLAG_POUND(flags_used)) && (dschar == FNET_FALSE))
                {
                    count += 2u;
                    fnet_serial_putchar(stream, '0');
                    fnet_serial_putchar(stream, 'x');

                    vlen += 2u;
                }

                cont_xd = FNET_TRUE;
                break;
            case 'o':
                uval = (fnet_uint32_t)va_arg(arg, fnet_uint32_t);
                vlen = fnet_serial_printk_mknumstr(vstr, &uval, FNET_FALSE, 8u);

                cont_u = FNET_TRUE;
                break;
            case 'b':
                uval = (fnet_uint32_t)va_arg(arg, fnet_uint32_t);
                vlen = fnet_serial_printk_mknumstr(vstr, &uval, FNET_FALSE, 2u);

                cont_u = FNET_TRUE;
                break;
            case 'p':
                uval = (fnet_uint32_t)va_arg(arg, void *);
                vlen = fnet_serial_printk_mknumstr(vstr, &uval, FNET_FALSE, 16u);

                cont_u = FNET_TRUE;
                break;
            case 'u':
                uval = (fnet_uint32_t)va_arg(arg, fnet_uint32_t);
                vlen = fnet_serial_printk_mknumstr(vstr, &uval, FNET_FALSE, 10u);

                cont_u = FNET_TRUE;
                cont_xd = FNET_TRUE;
                break;
            case 'c':
                cval = (fnet_char_t)va_arg(arg, fnet_int32_t);
                count++;
                fnet_serial_putchar(stream, cval);
                break;

            case 's':
                sval = (fnet_char_t *)va_arg(arg, fnet_char_t *);

                if(sval)
                {
                    vlen = fnet_strlen(sval);

                    if(!FNET_SERIAL_IS_FLAG_MINUS(flags_used))
                    {
                        fnet_serial_printk_pad(' ', stream, vlen, field_width, &count);
                    }

                    while(*sval)
                    {
                        count++;
                        fnet_serial_putchar(stream, (*sval++));
                    }

                    if(FNET_SERIAL_IS_FLAG_MINUS(flags_used))
                    {
                        fnet_serial_printk_pad(' ', stream, vlen, field_width, &count);
                    }
                }

                break;

            case 'n':
                ivalp = (fnet_int32_t *)va_arg(arg, fnet_int32_t *);
                *ivalp = (fnet_int32_t)count;
                break;

            default:
                count++;
                fnet_serial_putchar(stream, c);
                break;
        }


        if(cont_u == FNET_TRUE)
        {
            vstrp = &vstr[vlen];

            if(FNET_SERIAL_IS_FLAG_ZERO(flags_used))
            {
                fnet_serial_printk_pad('0', stream, vlen, field_width, &count);
                vlen = field_width;
            }
            else
            {
                if(!FNET_SERIAL_IS_FLAG_MINUS(flags_used))
                {
                    fnet_serial_printk_pad(' ', stream, vlen, field_width, &count);
                }
            }
        }

        if(cont_xd == FNET_TRUE)
        {
            while(*vstrp)
            {
                count++;
                fnet_serial_putchar(stream, (*vstrp--));
            }
            if(FNET_SERIAL_IS_FLAG_MINUS(flags_used))
            {
                fnet_serial_printk_pad(' ', stream, vlen, field_width, &count);
            }
        }

    }

    return count;
}

/********************************************************************/
fnet_size_t fnet_serial_printf(fnet_serial_stream_t stream, const fnet_char_t *format, ... )
{
    va_list ap;
    /*
     * Initialize the pointer to the variable length argument list.
     */
    va_start(ap, format);
    return fnet_serial_vprintf(stream, format, ap);

}

/********************************************************************/
fnet_size_t fnet_printf(const fnet_char_t *format, ... )
{
    va_list ap;
    /*
     * Initialize the pointer to the variable length argument list.
     */
    va_start(ap, format);
    return fnet_serial_vprintf(FNET_SERIAL_STREAM_DEFAULT, format, ap);
}

/************************************************************************
* NAME: fnet_println
*
* DESCRIPTION:
************************************************************************/
fnet_size_t fnet_println(const fnet_char_t *format, ... )
{
    va_list         ap;
    fnet_size_t     result = 0u;

    /*
     * Initialize the pointer to the variable length argument list.
     */
    va_start(ap, format);
    result = fnet_serial_vprintf(FNET_SERIAL_STREAM_DEFAULT, format, ap);
    result += fnet_printf("\n");

    return result;
}

/******************************************************************************
 * Control structure associated with the data buffer stream.
 ******************************************************************************/
struct fnet_serial_buffer_id
{
    fnet_char_t            *dest;      /* Pointer to the destination buffer.*/
    fnet_size_t     dest_size;  /* Maximum number of characters to be written to the buffer.*/
};

/********************************************************************/
static void fnet_serial_buffer_putchar(fnet_index_t p_dest, fnet_char_t character)
{
    struct fnet_serial_buffer_id *buffer_id = (struct fnet_serial_buffer_id *)p_dest;

    if(buffer_id->dest_size)
    {
        *(buffer_id->dest) = (fnet_uint8_t)character;
        buffer_id->dest++;

        --buffer_id->dest_size;
    }
}

/********************************************************************/
fnet_size_t fnet_sprintf( fnet_char_t *str, const fnet_char_t *format, ... )
{
    va_list     ap;
    fnet_size_t result = 0u;

    struct fnet_serial_stream buffer_stream;
    struct fnet_serial_buffer_id buffer_id;

    if(str != 0)
    {
        buffer_id.dest = str;
        buffer_id.dest_size = (fnet_size_t) - 1; /* No limit.*/

        buffer_stream.id = (fnet_index_t)&buffer_id;
        buffer_stream.putchar = fnet_serial_buffer_putchar;

        /*
         * Initialize the pointer to the variable length argument list.
         */
        va_start(ap, format);
        result = fnet_serial_vprintf(&buffer_stream, format, ap);
        *buffer_id.dest = '\0'; /* Trailing null character.*/
    }

    return result;
}

/********************************************************************/
fnet_size_t fnet_snprintf( fnet_char_t *str, fnet_size_t size, const fnet_char_t *format, ... )
{
    va_list                         ap;
    fnet_size_t                     result = 0u;
    struct fnet_serial_stream       buffer_stream;
    struct fnet_serial_buffer_id    buffer_id;


    if((str != 0) && (size != 0u))
    {
        --size; /* Space for the trailing null character.*/
        buffer_id.dest = str;
        buffer_id.dest_size = size;

        buffer_stream.id = (fnet_index_t)&buffer_id;
        buffer_stream.putchar = fnet_serial_buffer_putchar;

        /*
         * Initialize the pointer to the variable length argument list.
         */
        va_start(ap, format);
        result = fnet_serial_vprintf(&buffer_stream, format, ap);
        *buffer_id.dest = '\0'; /* Trailing null character.*/
        if(result > size)
        {
            result = size;
        }
    }
    return result;
}

/********************************************************************/
void fnet_putchar( fnet_char_t character )
{
    fnet_cpu_serial_putchar( FNET_CFG_CPU_SERIAL_PORT_DEFAULT, character);
}

/********************************************************************/
fnet_int32_t fnet_getchar( void )
{
    return fnet_cpu_serial_getchar(FNET_CFG_CPU_SERIAL_PORT_DEFAULT);
}

