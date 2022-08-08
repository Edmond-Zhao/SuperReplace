/* Created by Edmond  08/05/2022 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sr.h"

int init_replace_ent( replace_ent_t *p_rent, const int i_frist_flag )
{
    if( i_frist_flag )
    {
        memset( p_rent, 0, sizeof( replace_ent_t ) );
        return 0;
    }

    p_rent->i_match_index = 0;
    p_rent->i_matched_flag = 0;
    p_rent->i_in_file_match_begain = 0;
    p_rent->i_in_file_match_end = 0;
    memset( p_rent->s_match_file_path, 0, MAX_FILE_PATH );
    
    return 0;
}

int uninit_replace_ent( replace_ent_t *p_rent )
{
    if( p_rent )
    {
        if( p_rent->p_block_file_list )
            mmg_destroy( p_rent->p_block_file_list );
        if( p_rent->p_block_find )
            mmg_destroy( p_rent->p_block_find );
        if( p_rent->p_block_replace )
            mmg_destroy( p_rent->p_block_replace );
    }

    return 0;
}

int dump_replace_ent( replace_ent_t *p_rent )
{
    if( p_rent )
    {
        printf( "\n------------ DUMP RELACE ENTRY ----------\n" );
        printf( "i_match_index:           %d\n", p_rent->i_match_index );
        printf( "i_matched_flag:          %d\n", p_rent->i_matched_flag );
        printf( "i_in_file_match_begain:  %d\n", p_rent->i_in_file_match_begain );
        printf( "i_in_file_match_end:     %d\n", p_rent->i_in_file_match_end );
        printf( "i_file_path_count:       %d\n", p_rent->i_file_path_count );
        printf( "i_find_lines_count:      %d\n", p_rent->i_find_lines_count );
        printf( "i_replace_lines_count:   %d\n", p_rent->i_replace_lines_count );
        if( p_rent->p_block_file_list )
        {
            printf( "\n**DUMP p_block_file_list:\n" );
            mmg_block_db_dump( p_rent->p_block_file_list );
        }
        else
        {
            printf( "\n**DUMP p_block_file_list: (empty)\n" );
        }
        if( p_rent->p_block_find )
        {
            printf( "\n**DUMP p_block_find:\n" );
            mmg_block_db_dump( p_rent->p_block_find );
        }
        else
        {
            printf( "\n**DUMP p_block_find:   (empty)\n" );
        }
        if( p_rent->p_block_replace )
        {
            printf( "\n**DUMP p_block_replace:\n" );
            mmg_block_db_dump( p_rent->p_block_replace );
        }
        else
        {
            printf( "\n**DUMP p_block_replace: (empty)\n" );
        }

        if( strlen( p_rent->s_match_file_path ) > 0)
            printf( "\ns_match_file_path:       %s\n", p_rent->s_match_file_path );
        else
            printf( "\ns_match_file_path:       (empty)\n" );
        printf( "------------ DUMP RELACE ENTRY ----------\n" );
    }

    return 0;
}

int find_in_file( char *s_file_path, replace_ent_t *p_rent )
{
    FILE *fp = NULL;
    char s_line[ MAX_LINE_LENGTH ] = { 0 };
    char *p_find_line = NULL;
    int i_line_size = 0;
    int i_line_index = 0;
    int i_ret = 0;
    int i_line_len = 0;
    int i_tl_len = 0;

    if( s_file_path == NULL || p_rent == NULL )
        return -1;
    if( p_rent->p_block_find == NULL || p_rent->i_find_lines_count < 1 )
        return -1;

    p_find_line = mmg_get_first_line( p_rent->p_block_find );
    fp = fopen( s_file_path, "r" );
    if( fp == NULL )
    {
        printf( "open file '%s' failed!\n", s_file_path );
        return -1;
    }
    //printf( "Open file for search: %s\n", s_file_path );
    i_line_size = sizeof( s_line );
    while( fgets( s_line, i_line_size - 1, fp ) != NULL && p_find_line )
    {
        ++i_line_index;
        i_line_len = strlen( s_line );
        if( s_line[ i_line_len - 1 ] == '\n' )
            s_line[ i_line_len - 1 ] = '\0';
        i_tl_len = strlen( p_find_line );
        if( i_line_len == i_tl_len )
        {
            if( !strcmp( s_line, p_find_line ) )
            {
                p_rent->i_match_index++;
                //printf( "Match '%d' line: %s\n", p_rent->i_match_index, s_line );
                if( p_rent->i_match_index == 1 )
                {
                    p_rent->i_in_file_match_begain = i_line_index;
                }
                else if( p_rent->i_match_index == p_rent->i_find_lines_count )
                {
                    p_rent->i_matched_flag = 1;
                    p_rent->i_in_file_match_end = i_line_index;
                    strncpy( p_rent->s_match_file_path, 
                        s_file_path, 
                        MAX_FILE_PATH - 1 );
                    printf( "Found in file:    %s\n", s_file_path );
                    i_ret = 1;
                    break;
                }
                p_find_line = mmg_get_next_line( p_rent->p_block_find );
                continue;
            }
        }
        if( p_rent->i_match_index > 0 )
        {
            //printf( "Not match completely, mismatch on line '%d' reset it!\n",
            //    p_rent->i_match_index );
            p_rent->i_match_index = 0;
            p_find_line = mmg_get_first_line( p_rent->p_block_find );
        }
    }
    fclose( fp );

    return i_ret;
}

int replace_in_file( replace_ent_t *p_rent )
{
    int i_ret = 0;
    char s_new_name[ MAX_FILE_PATH ] = { 0 };
    char *p_replace_line = NULL;
    FILE *fp_orig = NULL, *fp_new = NULL;
    char s_line[ MAX_LINE_LENGTH ] = { 0 };
    int i_line_size = 0;
    int i_line_index = 0;
    int i_tl_index = 0;

    if( p_rent == NULL )
        return -1;
    if( p_rent->p_block_replace == NULL 
        || p_rent->i_matched_flag == 0
        || p_rent->i_replace_lines_count == 0
        || strlen( p_rent->s_match_file_path ) == 0 )
        return -1;
    if( p_rent->i_in_file_match_begain == 0 && p_rent->i_in_file_match_end == 0 )
        return -1;

    strcpy( s_new_name, p_rent->s_match_file_path );
    if( strlen( s_new_name ) + strlen( BAK_FILE_SUFFIX ) + 1 > sizeof( s_new_name ) )
    {
        printf( "New file name/path overflows.\n" );
        return -1;
    }
    strcat( s_new_name, BAK_FILE_SUFFIX );
    if( rename( p_rent->s_match_file_path, s_new_name ) )
    {
        printf( "Rename file '%s' to '%s' failed.\n",
            p_rent->s_match_file_path, s_new_name );
        return -1;
    }

    fp_orig = fopen( s_new_name, "r" );
    if( fp_orig == NULL )
    {
        printf( "open file '%s' for read only failed!\n", s_new_name );
        goto failed;
    }
    //printf( "Opened file %s for read.\n", s_new_name );
    fp_new = fopen( p_rent->s_match_file_path, "w" );
    if( fp_new == NULL )
    {
        printf( "open file '%s' for write failed!\n", p_rent->s_match_file_path );
        goto failed;;
    }
    //printf( "Opened file %s for write.\n", p_rent->s_match_file_path );

    p_replace_line = mmg_get_first_line( p_rent->p_block_replace );
    i_line_size = sizeof( s_line );
    while( fgets( s_line, i_line_size - 1, fp_orig ) != NULL )
    {
        ++i_line_index;
        if( i_line_index < p_rent->i_in_file_match_begain
            || i_line_index > p_rent->i_in_file_match_end )
        {
            fputs( s_line, fp_new );
            continue;
        }

        if( i_tl_index < p_rent->i_replace_lines_count && p_replace_line )
        {
            fprintf( fp_new, "%s\n", p_replace_line );
            //printf( "Replaced the line (%d): %s\n",
            //    i_tl_index + 1, p_rent->pp_replace_lines[ i_tl_index ] );
            p_replace_line = mmg_get_next_line( p_rent->p_block_replace );
            ++i_tl_index;
        }
    }
    printf( "Replaced in file: %s\n", p_rent->s_match_file_path );
    fclose( fp_orig );
    fclose( fp_new );
    return i_ret;

failed:
    printf( "Replacing FAILED in file: %s\n", p_rent->s_match_file_path );
    if( fp_orig )
        fclose( fp_orig );
    if( fp_new )
        fclose( fp_new );
    if( rename( s_new_name, p_rent->s_match_file_path ) )
    {
        printf( "Revert file name '%s' to '%s' failed.\n", 
            s_new_name, p_rent->s_match_file_path );
    }
    return -1;
}

static block_db_t *load_block_from_file( const char *s_name )
{
    FILE *fp = NULL;
    size_t st_size = 0;
    block_db_t *p_block_db = NULL;
    char s_line[ MAX_LINE_LENGTH ] = { 0 };
    int i_line_size = 0;
    int i_line_len = 0;

    if( s_name == NULL )
        return NULL;
    
    fp = fopen( s_name, "r" );
    if( fp == NULL )
    {
        printf( "open file '%s' for load failed!\n", s_name );
        return NULL;
    }
    fseek( fp, 0, SEEK_END );
    st_size = ftell( fp );
    printf( "The size of file '%s' is %zu\n", s_name, st_size );
    if( st_size == 0 )
        goto failed;
    
    p_block_db->file_size = st_size;
    p_block_db = mmg_get_block_db();
    if( p_block_db == NULL )
        goto failed;

    fseek( fp, 0, SEEK_SET );
    i_line_size = sizeof( s_line );
    while( fgets( s_line, i_line_size - 1, fp ) != NULL )
    {
        i_line_len = strlen( s_line );
        if( s_line[ i_line_len - 1 ] == '\n' )
            s_line[ i_line_len - 1 ] = '\0';
        i_line_len = strlen( s_line );
        if( i_line_len > 0 )
            mmg_add_a_line( p_block_db, s_line );
    }
    fclose( fp );
    return p_block_db;

failed:
    if( fp )
        fclose( fp );
    if( p_block_db )
        mmg_destroy( p_block_db );
    return NULL;
}

int load_param_from_file( replace_ent_t *p_rent, const char *s_name, const load_type_e load_type )
{
    block_db_t *p_block_db = NULL;

    if( p_rent == NULL || s_name == NULL )
        return -1;
    
    p_block_db = load_block_from_file( s_name );
    if( p_block_db == NULL )
    {
        printf( "Load parameters from file '%s' failed.\n", s_name );
        return -1;
    }

    if( load_type == LOAD_FILE_LIST )
    {
        p_rent->i_file_path_count = p_block_db->i_line_node_count;
        p_rent->p_block_file_list = p_block_db;
    }
    else if( load_type == LOAD_FIND_LINES )
    {
        p_rent->i_find_lines_count = p_block_db->i_line_node_count;
        p_rent->p_block_find = p_block_db;
    }
    else if( load_type== LOAD_REPLACE_LINES )
    {
        p_rent->i_replace_lines_count = p_block_db->i_line_node_count;
        p_rent->p_block_replace = p_block_db;
    }

    return 0;
}
