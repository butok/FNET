/**************************************************************************
*
* Copyright 2008-2018 by Andrey Butok. FNET Community.
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
***************************************************************************
*
* Shell service implementation.
*
***************************************************************************/

#include "fnet.h"

#define FNET_SHELL_ERR_SYNTAX   ("Error: Invalid syntax for: %s")
#define FNET_SHELL_ERR_CMD      ("Error: No such command: %s")

#define FNET_SHELL_BACKSPACE    ((fnet_char_t)(0x08))  /* Backspace. */
#define FNET_SHELL_DELETE       ((fnet_char_t)(0x7F))  /* Delete. */
#define FNET_SHELL_CTRLC        ((fnet_char_t)(0x03))  /* Ctrl + C. */
#define FNET_SHELL_CR           ((fnet_char_t)(0x0D))  /* CR. */
#define FNET_SHELL_LF           ((fnet_char_t)(0x0A))  /* LF. */
#define FNET_SHELL_ESC          ((fnet_char_t)(0x1B))  /* Esc. */
#define FNET_SHELL_SPACE        ((fnet_char_t)(0x20))  /* Space. */

#if FNET_CFG_DEBUG_SHELL && FNET_CFG_DEBUG
    #define FNET_DEBUG_SHELL   FNET_DEBUG
#else
    #define FNET_DEBUG_SHELL(...)   do{}while(0)
#endif

#if FNET_CFG_DEBUG_MEMPOOL && FNET_CFG_DEBUG
    #define FNET_DEBUG_MEMPOOL   FNET_DEBUG
#else
    #define FNET_DEBUG_MEMPOOL(...) do{}while(0)
#endif

#if FNET_CFG_DEBUG_STACK && FNET_CFG_DEBUG
    #define FNET_DEBUG_STACK   FNET_DEBUG
    extern fnet_size_t fnet_dbg_stack_max;
#else
    #define FNET_DEBUG_STACK(...)   do{}while(0)
#endif

/************************************************************************
*    Shell interface control structure.
*************************************************************************/
struct fnet_shell_if
{
    const fnet_shell_t *shell;
    const fnet_shell_t *top_shell;                             /* Pointer to the top shell. */
    fnet_shell_state_t      state;                                  /* Current state.*/
    fnet_service_desc_t     service_descriptor;                     /* Descriptor of polling service.*/
    fnet_size_t             pos;
    fnet_char_t             *cmd_line_begin;
    fnet_char_t             *cmd_line_end;
    fnet_char_t             *cmd_line;                              /* Command line buffer.*/
    fnet_size_t             cmd_line_size;
    fnet_index_t            block_index;                            /* >0 Flag that current command is blocked. */
    void                    (*_exit_blocked)(fnet_shell_desc_t shl_desc, void *cookie);/* Pointer to the callback function,
                                                                    * occurring on exit from command blocked
                                                                    * state. It happens when a user press
                                                                    * [Ctrl+C] button all fnet_shell_unblock() called. */
    void                    *_exit_blocked_cookie;
    fnet_serial_stream_t    stream;
    fnet_bool_t             echo;
    fnet_bool_t             is_active;                               /* Flag to avoid nested calling of the shell poll when command is calling fnet_service_poll() */
};

/* The Shell interface structure list */
static struct fnet_shell_if shell_if_list[FNET_CFG_SHELL_MAX];
static void _fnet_shell_echo( struct fnet_shell_if *shell_if, fnet_char_t character );
static fnet_index_t _fnet_shell_make_argv( fnet_char_t *cmdline, fnet_char_t *argv [] );
static void _fnet_shell_poll( void *shell_if_p );
static void _fnet_shell_esc_clear(fnet_char_t *str);

/************************************************************************
* DESCRIPTION:
************************************************************************/
static void _fnet_shell_echo( struct fnet_shell_if *shell_if, fnet_char_t character )
{
    if(shell_if->echo)
    {
        fnet_serial_putchar(shell_if->stream, character);
    }
}

/************************************************************************
* DESCRIPTION: Shell state machine.
************************************************************************/
static void _fnet_shell_poll( void *shell_if_p )
{
    struct fnet_shell_if    *shell_if = (struct fnet_shell_if *)shell_if_p;
    const fnet_shell_t      *shell = ((struct fnet_shell_if *)shell_if_p)->shell;
    fnet_int32_t            ch;
    fnet_index_t            argc;
    fnet_char_t             *argv[FNET_CFG_SHELL_ARGS_MAX + 1u] = {0}; /* One extra for 0 terminator.*/

    if(shell_if->is_active == FNET_FALSE)
    {
        shell_if->is_active = FNET_TRUE;

        switch(shell_if->state)
        {
            case FNET_SHELL_STATE_INIT:
                /* Print the shell prompt. */
                fnet_shell_printf((fnet_shell_desc_t)shell_if_p, "%s", shell->prompt_str);


                /* Debug: Prints mempool free memery (max posible allocated chunk) on every enter. */
                FNET_DEBUG_MEMPOOL("MAIN pool = %d (%d), FNET pool = %d (%d)", _fnet_free_mem_status(), _fnet_malloc_max(), _fnet_free_mem_status_netbuf(), _fnet_malloc_max_netbuf());
                /* Debug: Prints maximum stack usage. */
                FNET_DEBUG_STACK("Max Stack usage = %d", fnet_dbg_stack_max);

                shell_if->state = FNET_SHELL_STATE_GET_USER_INPUT;
                break;
            /*-------------------------------------*/
            case FNET_SHELL_STATE_GET_USER_INPUT:
                while( (ch = fnet_serial_getchar(shell_if->stream)) != FNET_ERR) /* Read all buffered characters from serial port. */
                {
                    /* CR or Buffer is full. */
                    if(((fnet_char_t)ch != FNET_SHELL_CR) && (shell_if->pos < shell_if->cmd_line_size))
                    {
                        switch(ch)
                        {
                            case FNET_SHELL_BACKSPACE:
                            case FNET_SHELL_DELETE:
                                if(shell_if->pos > 0u)
                                {
                                    shell_if->pos -= 1u;
                                    _fnet_shell_echo(shell_if, FNET_SHELL_BACKSPACE);
                                    fnet_serial_putchar(shell_if->stream, ' ');
                                    fnet_serial_putchar(shell_if->stream, FNET_SHELL_BACKSPACE);
                                }
                                break;
                            default:
                                if((shell_if->pos + 1u) < shell_if->cmd_line_size)
                                {
                                    /* Only printable characters. */
                                    if(((fnet_char_t)ch >= FNET_SHELL_SPACE) && ((fnet_char_t)ch <= FNET_SHELL_DELETE))
                                    {
                                        shell_if->cmd_line[shell_if->pos++] = (fnet_uint8_t)ch;
                                        _fnet_shell_echo(shell_if, (fnet_char_t)ch);
                                    }
                                }
                                break;
                        }
                    }
                    else
                    {
                        shell_if->cmd_line[shell_if->pos] = '\0';

                        _fnet_shell_echo(shell_if, FNET_SHELL_CR);
                        _fnet_shell_echo(shell_if, FNET_SHELL_LF);

                        shell_if->state = FNET_SHELL_STATE_EXEC_CMD;
                        /* Reset pointers */
                        shell_if->cmd_line_begin = shell_if->cmd_line;
                        shell_if->cmd_line_end = shell_if->cmd_line;
                    }
                }
                break;
            /*-------------------------------------------*/
            case FNET_SHELL_STATE_EXEC_CMD:

                shell_if->cmd_line_begin = shell_if->cmd_line_end;

                do
                {
                    if(*shell_if->cmd_line_end != '\0')
                    {
                        shell_if->cmd_line_end++;
                    }

                    shell_if->cmd_line_end = fnet_strchr( shell_if->cmd_line_end, FNET_SHELL_COMMAND_SPLITTER );

                    if((shell_if->cmd_line_end != 0) && (shell_if->cmd_line_begin != shell_if->cmd_line_end) && (shell_if->cmd_line_end[-1] != FNET_SHELL_ESCAPE_SYMBOL)) /* Found new command symbol.*/
                    {
                        *shell_if->cmd_line_end++ = '\0'; /* Set of end of line */
                        break;
                    }
                }
                while(shell_if->cmd_line_end);

                fnet_strlcpy(shell_if->cmd_line, shell_if->cmd_line_begin, shell_if->cmd_line_size);

                argc = _fnet_shell_make_argv(shell_if->cmd_line, argv);

                if(argc)
                {
                    const fnet_shell_command_t *cur_command = fnet_shell_get_command_by_name(shell_if, argv[0]);

                    if(cur_command)
                    {
                        if(((argc - 1u) >= cur_command->min_args) && ((argc - 1u) <= cur_command->max_args))
                        {
                            /* Shell command. */
                            {
                                if(cur_command->cmd_ptr)
                                {
                                    ((void(*)(fnet_shell_desc_t desc, fnet_index_t cmd_ptr_argc, fnet_char_t **cmd_ptr_argv))(cur_command->cmd_ptr))((fnet_shell_desc_t)shell_if, argc, argv);
                                }

                                /* In case shell switch*/
                                shell = shell_if->shell; /* Update current shell pointer. */
                                cur_command = shell->cmd_table;/* => to avoid wrong command message. */

                                /* Check if the shell was released during command execution.*/
                                if(shell_if->state == FNET_SHELL_STATE_DISABLED)
                                {
                                    return;
                                }
                            }
                        }
                        else /* Wrong command syntax. */
                        {
                            fnet_shell_println((fnet_shell_desc_t)shell_if_p, FNET_SHELL_ERR_SYNTAX, argv[0]);
                        }
                    }

                    if(cur_command->name == 0)
                    {
                        fnet_shell_println((fnet_shell_desc_t)shell_if_p, FNET_SHELL_ERR_CMD, argv[0]);
                    }

                }

                if(shell_if->block_index)
                {
                    shell_if->state = FNET_SHELL_STATE_BLOCKED;
                }
                else if(shell_if->cmd_line_end == 0)
                {
                    shell_if->state = FNET_SHELL_STATE_END_CMD;
                }
                else
                {}

                break;
            /*----------------------------------*/
            case FNET_SHELL_STATE_BLOCKED:
                if(shell_if->block_index)
                {
                    if(fnet_shell_is_ctrlc ((fnet_shell_desc_t)shell_if_p))
                    {
                        if(shell_if->_exit_blocked)
                        {
                            shell_if->_exit_blocked((fnet_shell_desc_t) shell_if, shell_if->_exit_blocked_cookie);
                        }
                        shell_if->block_index--;
                    }
                }
                else if(shell_if->cmd_line_end == 0)
                {
                    shell_if->state = FNET_SHELL_STATE_END_CMD;
                }
                else
                {
                    shell_if->state = FNET_SHELL_STATE_EXEC_CMD;
                }
                break;
            /*----------------------------------*/
            case FNET_SHELL_STATE_END_CMD:
                shell_if->state = FNET_SHELL_STATE_INIT;
                shell_if->pos = 0u;
                shell_if->cmd_line[0] = 0u;
                break;
            default:
                break;
        }
        shell_if->is_active = FNET_FALSE;
    }
    else
    {
        if(shell_if->block_index) /* If blocked */
        {
            if(fnet_shell_is_ctrlc ((fnet_shell_desc_t)shell_if_p))
            {
                if(shell_if->_exit_blocked)
                {
                    shell_if->_exit_blocked((fnet_shell_desc_t) shell_if, shell_if->_exit_blocked_cookie);
                }
                shell_if->block_index--;
            }
        }
    }
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
fnet_shell_desc_t fnet_shell_init( fnet_shell_params_t *params)
{
    fnet_index_t i;

    struct fnet_shell_if *shell_if = 0;

    /* Check input parameters. */
    if((params == 0) || (params->cmd_line_buffer == 0 ) || (params->cmd_line_buffer_size == 0u ) )
    {
        FNET_DEBUG_SHELL("Shell: Wrong init parameters.");
        goto ERROR;
    }

    /* Try to find free Shell service. */
    for(i = 0u; i < FNET_CFG_SHELL_MAX; i++)
    {
        if(shell_if_list[i].state == FNET_SHELL_STATE_DISABLED)
        {
            shell_if = &shell_if_list[i];
            break;
        }
    }

    /* Is Shell already initialized. */
    if(shell_if == 0)
    {
        FNET_DEBUG_SHELL("SHELL: No free Sheell service.");
        goto ERROR;
    }

    fnet_memset_zero(shell_if, sizeof(*shell_if));

    shell_if->shell = params->shell;
    shell_if->top_shell = shell_if->shell;
    shell_if->cmd_line = params->cmd_line_buffer;
    shell_if->cmd_line_size = params->cmd_line_buffer_size;
    shell_if->echo = params->echo;

    if(params->stream)
    {
        shell_if->stream = params->stream;
    }
    else
    {
        shell_if->stream = FNET_SERIAL_STREAM_DEFAULT;
    }

    shell_if->service_descriptor = fnet_service_register(_fnet_shell_poll, (void *) shell_if);
    if(shell_if->service_descriptor == 0)
    {
        FNET_DEBUG_SHELL("Shell: Service registration error.");
        goto ERROR;
    }

    /* Reset parameters. */
    shell_if->state = FNET_SHELL_STATE_INIT;
    shell_if->pos = 0u;
    shell_if->cmd_line[0] = 0u;

    if(shell_if->shell->shell_init)
    {
        shell_if->shell->shell_init((fnet_shell_desc_t)shell_if);
    }

    return (fnet_shell_desc_t)shell_if;
ERROR:
    return FNET_NULL;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
void fnet_shell_release(fnet_shell_desc_t desc)
{
    struct fnet_shell_if *shell_if = (struct fnet_shell_if *) desc;

    fnet_service_mutex_lock();

    if(shell_if && (shell_if->state != FNET_SHELL_STATE_DISABLED))
    {
        fnet_service_unregister(shell_if->service_descriptor); /* Delete service.*/
        shell_if->state = FNET_SHELL_STATE_DISABLED;
    }

    fnet_service_mutex_unlock();
}

/************************************************************************
* DESCRIPTION: Eliminate escape symbols.
************************************************************************/
static void _fnet_shell_esc_clear(fnet_char_t *str)
{
    fnet_char_t *dest = str;
    fnet_char_t *src = str;

    while(*src != 0u)
    {
        if((*src == FNET_SHELL_ESCAPE_SYMBOL) && (src[1] != FNET_SHELL_ESCAPE_SYMBOL))
        {
            src++;
        }
        else
        {
            *dest++ = *src++;
        }
    }
    *dest = '\0';
}

/************************************************************************
* DESCRIPTION: Calculates the quantity of arguments and splits them array
*              argv[]
************************************************************************/
static fnet_index_t _fnet_shell_make_argv( fnet_char_t *cmdline, fnet_char_t *argv [] )
{
    fnet_index_t    argc;
    fnet_index_t    i;
    fnet_bool_t     in_text_flag;
    fnet_bool_t     qouted;

    /* Break cmdline into strings.*/
    argc = 0u;
    in_text_flag = FNET_FALSE;
    qouted = FNET_FALSE;

    if(cmdline && argv)
    {

        for(i = 0u; cmdline[i] != '\0'; i++)
        {
            if(((qouted == FNET_FALSE) && ((cmdline[i] == ' ') || (cmdline[i] == '\t')))
               || ((qouted == FNET_TRUE) && ((cmdline[i] == FNET_SHELL_QUOTE_SYMBOL) && ((i == 0u) || (cmdline[i - 1u] != FNET_SHELL_ESCAPE_SYMBOL))  )   ) )
            {
                if(in_text_flag == FNET_TRUE)
                {
                    /* Set end of command line argument.*/
                    cmdline[i] = '\0';
                    in_text_flag = FNET_FALSE;
                    qouted = FNET_FALSE;
                    _fnet_shell_esc_clear(argv[argc - 1u]); /* Clear escape symbols. */
                }
            }
            else
            {
                /* Got non-whitespace character. */
                if(in_text_flag == FNET_FALSE)
                {
                    /* Start of an argument. */
                    in_text_flag = FNET_TRUE;

                    if(argc < FNET_CFG_SHELL_ARGS_MAX)
                    {
                        if((cmdline[i] == FNET_SHELL_QUOTE_SYMBOL) && ((i == 0u) || (cmdline[i - 1u] != FNET_SHELL_ESCAPE_SYMBOL)))
                        {
                            qouted = FNET_TRUE;
                        }

                        argv[argc] = &cmdline[i + (fnet_index_t)qouted];

                        argc++;
                    }
                    else /* Return argc.*/
                    {
                        break;
                    }
                }
            }
        }

        argv[argc] = 0;
    }

    return argc;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
fnet_size_t fnet_shell_printf(fnet_shell_desc_t desc, const fnet_char_t *format, ... )
{
    va_list                 ap;
    fnet_size_t             result = 0u;
    struct fnet_shell_if    *shell_if = (struct fnet_shell_if *) desc;

    if(shell_if)
    {
        /* Initialize the pointer to the variable length argument list. */
        va_start(ap, format);
        result = fnet_serial_vprintf(shell_if->stream, format, ap);
        va_end(ap);

        fnet_serial_flush(shell_if->stream);
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
fnet_size_t fnet_shell_println(fnet_shell_desc_t desc, const fnet_char_t *format, ... )
{
    va_list                 ap;
    fnet_size_t             result = 0u;
    struct fnet_shell_if    *shell_if = (struct fnet_shell_if *) desc;

    if(shell_if)
    {
        /* Initialize the pointer to the variable length argument list. */
        va_start(ap, format);
        result = fnet_serial_vprintf(shell_if->stream, format, ap);
        va_end(ap);
        result += fnet_shell_printf(desc, "\n"); /* Add new line.*/

        fnet_serial_flush(shell_if->stream);
    }

    return result;
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
void fnet_shell_putchar(fnet_shell_desc_t desc, fnet_char_t character)
{
    struct fnet_shell_if *shell_if = (struct fnet_shell_if *) desc;

    if(shell_if)
    {
        fnet_serial_putchar(shell_if->stream, character);
        fnet_serial_flush(shell_if->stream);
    }
}

/************************************************************************
* DESCRIPTION:
************************************************************************/
fnet_int32_t fnet_shell_getchar(fnet_shell_desc_t desc)
{
    fnet_int32_t            result;
    struct fnet_shell_if    *shell_if = (struct fnet_shell_if *) desc;

    if(shell_if)
    {
        result = fnet_serial_getchar(shell_if->stream);
    }
    else
    {
        result = FNET_ERR;
    }

    return result;
}

/************************************************************************
* DESCRIPTION: Executes command-line script.
************************************************************************/
fnet_return_t fnet_shell_script(fnet_shell_desc_t desc, fnet_char_t *script )
{
    struct fnet_shell_if    *shell_if = (struct fnet_shell_if *) desc;
    fnet_bool_t             previous_script = FNET_FALSE;
    fnet_size_t             script_size;
    fnet_return_t           result = FNET_ERR;

    if(shell_if && script)
    {
        fnet_service_mutex_lock();

        fnet_shell_unblock(desc);

        script_size = fnet_strlen(script);

        if(script_size)
        {
            if(script_size + 1/*splitter*/ + 1/*size*/ > shell_if->cmd_line_size)
            {
                goto ERROR;
            }

            if(shell_if->cmd_line_end) /* If previous script is not finished.*/
            {
                fnet_size_t     cmd_line_end_size = fnet_strlen(shell_if->cmd_line_end);

                if(cmd_line_end_size)
                {
                    fnet_int32_t    cmd_line_shift;
                    fnet_index_t    i;

                    if(cmd_line_end_size + script_size + 1/*splitter*/ + 1/*size*/ > shell_if->cmd_line_size)
                    {
                        goto ERROR;
                    }

                    cmd_line_shift = (shell_if->cmd_line + script_size + 1) - shell_if->cmd_line_end;
                    if(cmd_line_shift < 0) /* Shift left */
                    {
                        for(i = 0; i <= cmd_line_end_size; i++)
                        {
                            shell_if->cmd_line_end[cmd_line_shift + i] = shell_if->cmd_line_end[i];
                        }
                    }
                    else if(cmd_line_shift > 0) /* Shift right */
                    {
                        for(i = 0; i <= cmd_line_end_size; i++)
                        {
                            shell_if->cmd_line_end[cmd_line_end_size + cmd_line_shift - i] = shell_if->cmd_line_end[cmd_line_end_size - i];
                        }

                    }
                    /* else - no shifting */
                    previous_script = FNET_TRUE;
                }
            }

            /* Add new script */
            fnet_strlcpy( shell_if->cmd_line, script, shell_if->cmd_line_size);
            if(previous_script == FNET_TRUE)
            {
                /* Add splitter between scripts.*/
                shell_if->cmd_line[script_size] = FNET_SHELL_COMMAND_SPLITTER;
            }

            shell_if->state = FNET_SHELL_STATE_EXEC_CMD;
            /* Reset pointers */
            shell_if->cmd_line_begin = shell_if->cmd_line;
            shell_if->cmd_line_end = shell_if->cmd_line;
        }
        result = FNET_OK;
        fnet_service_mutex_unlock();
    }
ERROR:
    return result;
}

/************************************************************************
* DESCRIPTION: Stops execution of the shell command line.
************************************************************************/
void fnet_shell_script_stop( fnet_shell_desc_t desc)
{
    struct fnet_shell_if *shell_if = (struct fnet_shell_if *) desc;

    if(shell_if)
    {
        fnet_service_mutex_lock();
        fnet_memset_zero(shell_if->cmd_line, shell_if->cmd_line_size );
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Shows command shell help.
************************************************************************/
void fnet_shell_help( fnet_shell_desc_t desc)
{
    const fnet_shell_t *shell = ((struct fnet_shell_if *)desc)->shell;
    const fnet_shell_command_t *cur_command = shell->cmd_table;

    while(cur_command->name)
    {
        fnet_shell_println(desc, FNET_CFG_SHELL_HELP_FORMAT,
                           cur_command->name,
                           cur_command->syntax,
                           cur_command->description);
        cur_command++;
    }
}

/***************************************************************************/ /*!
 * @internal
 * @brief    Detects if the [Ctrl]+[c] is received.
 *
 *
 * @return This function returns:
 *   - @c 0 if [Ctrl]+[c] is not received/pressed.
 *   - @c 1 if [Ctrl]+[c] is received/pressed.
 *
 ******************************************************************************
 *
 * This function detects if the [Ctrl]+[c] command is received.@n
 * It can be used by blocking shell commands to detect that a user wants to
 * terminate the command activity.
 *
 ******************************************************************************/
fnet_bool_t fnet_shell_is_ctrlc (fnet_shell_desc_t desc)
{
    fnet_bool_t             res;
    struct fnet_shell_if    *shell_if = (struct fnet_shell_if *) desc;
    fnet_char_t             ch = (fnet_char_t)fnet_serial_getchar(shell_if->stream);

    if(ch == FNET_SHELL_CTRLC)
    {
        res = FNET_TRUE;
    }
    else
    {
        res = FNET_FALSE;
    }

    return res;
}

/************************************************************************
* DESCRIPTION: Blocks the shell to ignore user commands.
************************************************************************/
fnet_return_t fnet_shell_block( fnet_shell_desc_t desc, void (*on_ctrlc)(fnet_shell_desc_t shl_desc, void *cookie), void *cookie)
{
    struct fnet_shell_if    *shell_if = (struct fnet_shell_if *) desc;
    fnet_return_t           res;

    if(shell_if && on_ctrlc )
    {
        fnet_service_mutex_lock();
        shell_if->block_index++;
        shell_if->_exit_blocked = on_ctrlc;
        shell_if->_exit_blocked_cookie = cookie;
        res = FNET_OK;
        fnet_service_mutex_unlock();
    }
    else
    {
        res = FNET_ERR;
    }

    return res;
}

/************************************************************************
* DESCRIPTION:  Unblocks the shell to accept user commands.
************************************************************************/
void fnet_shell_unblock( fnet_shell_desc_t desc)
{
    struct fnet_shell_if *shell_if = (struct fnet_shell_if *) desc;

    if(shell_if)
    {
        fnet_service_mutex_lock();
        if(shell_if->block_index)
        {
            shell_if->block_index--;
        }
        fnet_service_mutex_unlock();
    }
}

/************************************************************************
* DESCRIPTION: Switch to other command line set.
************************************************************************/
fnet_return_t fnet_shell_switch( fnet_shell_desc_t desc, const fnet_shell_t *switch_shell)
{
    struct fnet_shell_if    *shell_if = (struct fnet_shell_if *) desc;
    fnet_return_t           res;

    if(shell_if)
    {
        fnet_service_mutex_lock();
        shell_if->cmd_line[0] = '\0'; /* Clear command line. */

        if(switch_shell) /* Switch to new shell.*/
        {
            shell_if->shell = switch_shell;

            if(shell_if->shell->shell_init)
            {
                shell_if->shell->shell_init(desc);
            }
        }
        else /* Switch to previous shell.*/
        {
            shell_if->shell = shell_if->top_shell;
        }

        res = FNET_OK;
        fnet_service_mutex_unlock();
    }
    else
    {
        res = FNET_ERR;
    }

    return res;
}

/************************************************************************
* Looks for a shell command by its name.
************************************************************************/
const fnet_shell_command_t *fnet_shell_get_command_by_name(fnet_shell_desc_t desc, const fnet_char_t *name)
{
    struct fnet_shell_if        *shell_if = (struct fnet_shell_if *)desc;
    const fnet_shell_command_t  *result = FNET_NULL;

    if(shell_if && shell_if->shell)
    {
        const fnet_shell_command_t *cur_command;

        for(cur_command = shell_if->shell->cmd_table; cur_command->name; cur_command++)
        {
            if(fnet_strcasecmp(cur_command->name, name) == 0) /* Command is found. */
            {
                result = cur_command;
                break;
            }
        }
    }

    return result;
}
