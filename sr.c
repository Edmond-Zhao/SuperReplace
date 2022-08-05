/* Created by Edmond  08/05/2022 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sr.h"

int init_replace_ent( replace_ent_t *p_ent, const int i_frist_flag )
{
    if( i_frist_flag )
    {
        memset( p_ent, 0, sizeof( replace_ent_t ) );
        return 0;
    }

    p_ent->i_match_index = 0;
    p_ent->i_matched_flag = 0;
    p_ent->i_in_file_match_begain = 0;
    p_ent->i_in_file_match_end = 0;
    memset( p_ent->s_match_file_path, 0, MAX_FILE_PATH );
    
    return 0;
}

int find_in_file( char *s_file_path, replace_ent_t *p_rent )
{
    FILE *fp = NULL;
    char s_line[ MAX_LINE_LENGTH ] = { 0 };
    char *p_target_line = NULL;
    int i_line_size = 0;
    int i_line_index = 0;
    int i_ret = 0;
    int i_line_len = 0;
    int i_tl_len = 0;

    if( s_file_path == NULL || p_rent == NULL )
        return -1;
    if( p_rent->pp_target_lines == NULL || p_rent->i_target_lines_count < 1 )
        return -1;

    p_target_line = p_rent->pp_target_lines[ 0 ];
    fp = fopen( s_file_path, "r" );
    if( fp == NULL )
    {
        printf( "open file '%s' failed!\n", s_file_path );
        return -1;
    }
    //printf( "Open file for search: %s\n", s_file_path );
    i_line_size = sizeof( s_line );
    while( fgets( s_line, i_line_size - 1, fp ) != NULL )
    {
        ++i_line_index;
        i_line_len = strlen( s_line );
        if( s_line[ i_line_len - 1 ] == '\n' )
            s_line[ i_line_len - 1 ] = '\0';
        i_line_len = strlen( s_line );
        i_tl_len = strlen( p_target_line );
        if( i_line_len == i_tl_len )
        {
            if( !strcmp( s_line, p_target_line ) )
            {
                p_rent->i_match_index++;
                //printf( "Match '%d' line: %s\n", p_rent->i_match_index, s_line );
                if( p_rent->i_match_index == 1 )
                {
                    p_rent->i_in_file_match_begain = i_line_index;
                }
                else if( p_rent->i_match_index == p_rent->i_target_lines_count )
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
                p_target_line = p_rent->pp_target_lines[ p_rent->i_match_index ];
                continue;
            }
        }
        if( p_rent->i_match_index > 0 )
        {
            //printf( "Not match completely, mismatch on line '%d' reset it!\n",
            //    p_rent->i_match_index );
            p_rent->i_match_index = 0;
            p_target_line = p_rent->pp_target_lines[ 0 ];
        }
    }
    fclose( fp );

    return i_ret;
}

int replace_in_file( replace_ent_t *p_rent )
{
    int i_ret = 0;
    char s_new_name[ MAX_FILE_PATH ] = { 0 };
    FILE *fp_orig = NULL, *fp_new = NULL;
    char s_line[ MAX_LINE_LENGTH ] = { 0 };
    int i_line_size = 0;
    int i_line_index = 0;
    int i_tl_index = 0;

    if( p_rent == NULL )
        return -1;
    if( p_rent->pp_replace_lines == NULL 
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

        if( i_tl_index < p_rent->i_replace_lines_count )
        {
            fprintf( fp_new, "%s\n", p_rent->pp_replace_lines[ i_tl_index ] );
            //printf( "Replaced the line (%d): %s\n",
            //    i_tl_index + 1, p_rent->pp_replace_lines[ i_tl_index ] );
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

char *load_file( const char *s_name )
{
    FILE *fp = NULL;
    unsigned long ul_size = 0;

    if( s_name == NULL )
        return -1;
    
    return NULL;
}