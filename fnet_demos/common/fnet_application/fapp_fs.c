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
* @file fapp_fs.c
*
* @author Andrey Butok
*
* @brief FNET Shell Demo (File System Explorer).
*
***************************************************************************/

#include "fnet.h"
#include "fapp.h"
#include "fapp_fs.h"
#include "fapp_prv.h"


#if FAPP_CFG_EXP_CMD || FAPP_CFG_HTTP_CMD

    /************************************************************************
    *     Definitions.
    *************************************************************************/
    #define FAPP_FS_FSMOUNT_ERR  ("Error: FS mount is failed!\n")
    #define FAPP_FS_FSINIT_ERR   ("Error: FS init is failed!\n")

#endif

#if FAPP_CFG_EXP_CMD && FNET_CFG_FS

/* Error mesages */
#define FAPP_FS_DIR_ERR      ("Error: DIR is failed!")
#define FAPP_FS_CD_ERR       ("Error: CD %s is failed!")
#define FAPP_FS_VIEW_ERR     ("Error: VIEW %s is failed!")

#define FAPP_FS_DIR_STR      "%7s %6d Bytes  -  %s"

/************************************************************************
*     Function Prototypes
*************************************************************************/
static void fapp_fs_init( fnet_shell_desc_t desc );
static void fapp_fs_dir_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
static void fapp_fs_cd_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
static void fapp_fs_view_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );
static void fapp_fs_exit_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv );

/************************************************************************
*     File Explorer definitions.
*************************************************************************/
/* maximum path length */
#define FAPP_FS_DIR_PATH_MAX     (50U)

/* Explorer shell prompt */
static const fnet_char_t FAPP_FS_PROMPT_STR_HEADER[] = "EXP:";
static const fnet_char_t FAPP_FS_PROMPT_STR_TRAILER[] = "> ";
static fnet_char_t FAPP_FS_PROMPT_STR [FAPP_FS_DIR_PATH_MAX +
                                       sizeof(FAPP_FS_PROMPT_STR_HEADER) +
                                       sizeof(FAPP_FS_PROMPT_STR_TRAILER)];


/* Current path */
static fnet_char_t fapp_fs_current_path[FAPP_FS_DIR_PATH_MAX + 1U] = {FNET_FS_SPLITTER, '\0', 0};

/************************************************************************
*     The table of the File Explorer commands.
*************************************************************************/
static const struct fnet_shell_command fapp_fs_cmd_tab [] =
{
    { "help", 0, 0, fapp_help_cmd,      "Display this help message.", ""},
    { "dir", 0, 0, fapp_fs_dir_cmd,     "Display a list of files & directories.", ""},
    { "cd", 1, 1, fapp_fs_cd_cmd,       "Change the current directory.", "<directory>"},
    { "view", 1, 1, fapp_fs_view_cmd,   "View a text file.", "<file>"},
    { "exit", 0, 0, fapp_fs_exit_cmd,   "Exit from the file explorer.", ""},
    { 0, 0, 0, 0, 0, 0}
};

/************************************************************************
*     The File Explorer control data structure.
*************************************************************************/
const struct fnet_shell fapp_fs_shell =
{
    fapp_fs_cmd_tab,                   /* cmd_table */
    FAPP_FS_PROMPT_STR,                /* prompt_str */
    fapp_fs_init,                      /* shell_init */
};

/************************************************************************
* NAME: fapp_fs_init
*
* DESCRIPTION: File Explorer initialization function.
************************************************************************/
static void fapp_fs_init( fnet_shell_desc_t desc )
{
    /* Format exp shell prompt */
    fnet_snprintf( FAPP_FS_PROMPT_STR, sizeof(FAPP_FS_PROMPT_STR), "%s%s%s",
                   &FAPP_FS_PROMPT_STR_HEADER[0],
                   &fapp_fs_current_path[0],
                   &FAPP_FS_PROMPT_STR_TRAILER[0]);

    fnet_shell_println(desc, "\n%s", FAPP_DELIMITER_STR);
    fnet_shell_println(desc, "  File Explorer started.");
    fnet_shell_println(desc, "  Enter 'help' for command list.");
    fnet_shell_println(desc, "%s\n", FAPP_DELIMITER_STR);
}


/************************************************************************
* NAME: fapp_fs_dir_cmd
*
* DESCRIPTION: Displays a list of files and subdirectories in a directory.
*************************************************************************/
static void fapp_fs_dir_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    struct fnet_fs_dirent   ep;
    fnet_fs_dir_t           dir;
    FNET_COMP_UNUSED_ARG(argc);
    FNET_COMP_UNUSED_ARG(argv);

    /* Open dir */
    dir = fnet_fs_opendir(fapp_fs_current_path);

    if (dir)
    {
        /* Print the dir content. */
        while ((fnet_fs_readdir (dir, &ep)) == FNET_OK)
        {
            fnet_shell_println(desc, FAPP_FS_DIR_STR, (ep.d_type == DT_DIR) ? "<DIR>" : "<FILE>", ep.d_size, ep.d_name);
        }

        /* Close dir */
        fnet_fs_closedir(dir);
    }
    else
    {
        fnet_shell_println(desc, FAPP_FS_DIR_ERR);
    }
}


/************************************************************************
* NAME: fapp_fs_cd_cmd
*
* DESCRIPTION: Change the current directory.
*************************************************************************/
static void fapp_fs_cd_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_fs_dir_t  dir;
    fnet_char_t    *path = argv[1];
    fnet_char_t    *path_end;
    fnet_size_t     size_cd = fnet_strlen (fapp_fs_current_path);
    fnet_size_t     size_path;
    fnet_char_t    splitter[] = {FNET_FS_SPLITTER, '\0'};

    FNET_COMP_UNUSED_ARG(argc);

    if (*path != FNET_FS_SPLITTER) /* Realative path.*/
    {
        /* Add splitter if not yet.*/
        if(fapp_fs_current_path[size_cd - 1U] != FNET_FS_SPLITTER)
        {
            fnet_strncat( &fapp_fs_current_path[0], splitter, FAPP_FS_DIR_PATH_MAX);
        }

        fnet_strncat( &fapp_fs_current_path[0], path, FAPP_FS_DIR_PATH_MAX);
        path = fapp_fs_current_path;
    }
    else /* Full path. */
    {
        /* Strip possible repetitive leading slashes. */
        while ((path[0] == FNET_FS_SPLITTER) && (path[1] == FNET_FS_SPLITTER))
        {
            path++;
        }
    }

    /* Strip possible ending slashes. */
    if((size_path = fnet_strlen(path)) > 0U)
    {
        path_end = &path[size_path - 1U];
        while(*path_end == FNET_FS_SPLITTER)
        {
            *path_end = '\0';
            path_end--;
        }
    }

    /* Open dir. */
    dir = fnet_fs_opendir(path);

    if (dir)
    {
        /* Update cur path. */
        fnet_strncpy( &fapp_fs_current_path[0], path, FAPP_FS_DIR_PATH_MAX);
        if(fapp_fs_current_path[0] == '\0') /* root dir */
        {
            fnet_strncat( &fapp_fs_current_path[0], splitter, FAPP_FS_DIR_PATH_MAX);
        }

        /* Change shell prompt. */
        fnet_sprintf( FAPP_FS_PROMPT_STR, "%s%s%s",
                      &FAPP_FS_PROMPT_STR_HEADER[0],
                      &fapp_fs_current_path[0],
                      &FAPP_FS_PROMPT_STR_TRAILER[0]);

        /* Close dir. */
        fnet_fs_closedir(dir);
    }
    else
    {
        /* Restore cur path. */
        fapp_fs_current_path[size_cd] = '\0';
        fnet_shell_println(desc, FAPP_FS_CD_ERR, argv[1]);
    }

}

/************************************************************************
* NAME: fapp_fs_view_cmd
*
* DESCRIPTION:
*************************************************************************/
static void fapp_fs_view_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    fnet_fs_file_t file;
    fnet_char_t    *path = argv[1];
    fnet_char_t    *path_end;
    fnet_size_t     size_cd = fnet_strlen (fapp_fs_current_path);
    fnet_size_t     size_path;
    fnet_char_t    splitter[] = {FNET_FS_SPLITTER, '\0'};
    fnet_uint8_t    data;
    struct          fnet_fs_dirent dirent;

    FNET_COMP_UNUSED_ARG(desc);
    FNET_COMP_UNUSED_ARG(argc);

    if (*path != FNET_FS_SPLITTER) /* Realative path.*/
    {
        /* Add splitter if not yet.*/
        if(fapp_fs_current_path[size_cd - 1U] != FNET_FS_SPLITTER)
        {
            fnet_strncat( &fapp_fs_current_path[0], splitter, FAPP_FS_DIR_PATH_MAX);
        }

        fnet_strncat( &fapp_fs_current_path[0], path, FAPP_FS_DIR_PATH_MAX);
        path = fapp_fs_current_path;
    }
    else /* Full path. */
    {
        /* Strip possible repetitive leading slashes. */
        while ((path[0] == FNET_FS_SPLITTER) && (path[1] == FNET_FS_SPLITTER))
        {
            path++;
        }
    }

    /* Strip possible ending slashes. */
    if((size_path = fnet_strlen(path)) > 0U)
    {
        path_end = &path[size_path - 1U];
        while(*path_end == FNET_FS_SPLITTER)
        {
            *path_end = '\0';
            path_end--;
        }
    }

    /* Open file. */
    file = fnet_fs_fopen(path, "r");

    if (file)
    {
        /* Print file info.*/
        fnet_fs_finfo (file, &dirent);

        fnet_shell_println(desc, FAPP_FS_DIR_STR, "Content of:", dirent.d_size, dirent.d_name);

        while(fnet_fs_fread(&data, sizeof(data), file))
        {
            fnet_shell_putchar(desc, data);
        }

        /* Close file. */
        fnet_fs_fclose(file);
    }
    else
    {
        fnet_shell_println(desc, FAPP_FS_VIEW_ERR, argv[1]);
    }

    /* Restore cur path. */
    fapp_fs_current_path[size_cd] = '\0';

}

/************************************************************************
* NAME: fapp_fs_exit_cmd
*
* DESCRIPTION: Exit from the file explorer.
*************************************************************************/
static void fapp_fs_exit_cmd( fnet_shell_desc_t desc, fnet_index_t argc, fnet_char_t **argv )
{
    FNET_COMP_UNUSED_ARG(argc);
    FNET_COMP_UNUSED_ARG(argv);

    fnet_shell_switch(desc, FNET_NULL);
}


#endif /* FAPP_CFG_EXP_CMD */



#if (FAPP_CFG_EXP_CMD || FAPP_CFG_HTTP_CMD) && FNET_CFG_FS
/************************************************************************
* NAME: fapp_fs_mount
*
* DESCRIPTION: Mount FS image.
************************************************************************/
void fapp_fs_mount(void)
{
    if( fnet_fs_init( ) == FNET_OK)
    {
        /* Register the FNET ROM FS. */
        fnet_fs_rom_register( );

        /* Mount the FNET ROM FS image. */
        if( fnet_fs_mount( FNET_FS_ROM_NAME, FAPP_FS_MOUNT_NAME, &fnet_fs_image ) == FNET_ERR )
        {
            fnet_printf(FAPP_FS_FSMOUNT_ERR);
        }
    }
    else
    {
        fnet_printf(FAPP_FS_FSINIT_ERR);
    }
}
/************************************************************************
* NAME: fapp_fs_unmount
*
* DESCRIPTION: Unmount FS image.
************************************************************************/
void fapp_fs_unmount(void)
{
    fnet_fs_rom_unregister();   /* Unregister ROM FS. */
    fnet_fs_release();          /* Release FS. */
}

#endif

