/*
 * Super Relpace
 * Created by Edmond Zhao (edmondzym@gmail)
 * 08/04/2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH        1024
#define MAX_LINE_COUNT         10
#define MAX_FILE_PATH          512

typedef struct replace_ent_s
{
    int i_lines_count;
    int i_match_index;
    int i_matched_flag;
    int i_in_file_match_begain;
    int i_in_file_match_end;

    char *pp_target_lines;
    char s_match_file_path[ MAX_FILE_PATH ];
} replace_ent_t;

int find_in_file( char *s_file_path, replace_ent_t *p_rent )
{
    FILE *fp = NULL;
    char s_line[ MAX_LINE_LENGTH ] = { 0 };
    char *p_target_line = NULL;
    int i_line_size = 0;
    int i_line_index = 0;
    int i_ret = 0;

    if( s_file_path == NULL || p_rent == NULL )
        return -1;
    if( p_rent->pp_target_lines == NULL || p_rent->i_lines_count < 1 )
        return -1;

    p_target_line = p_rent->pp_target_lines[ 0 ];
    fp = fopen( s_file_path, "r" );
    if( fp == NULL )
    {
        printf( "open file '%s' failed!\n", s_file_path );
        return -1;
    }

    i_line_size = sizeof( s_line );
    while( fgets( s_line, i_line_size - 1, fp ) != NULL )
    {
        ++i_line_index;
        if( strlen( s_line ) == strlen( p_target_line ) )
        {
            if( !strcmp( s_line, p_target_line ) )
            {
                p_rent->i_match_index++;
                if( p_rent->i_match_index == 1 )
                {
                    p_rent->i_in_file_match_begain = i_line_index;
                }
                else if( p_rent->i_match_index == p_rent->i_lines_count )
                {
                    p_rent->i_matched_flag = 1;
                    p_rent->i_in_file_match_end = i_line_index;
                    strncpy( p_rent->s_match_file_path, 
                        s_file_path, 
                        MAX_FILE_PATH - 1 );
                    printf( "Found in: %s\n", s_file_path );
                    i_ret = 1;
                    break;
                }
                ++p_target_line;
                continue;
            }
        }
        if( p_rent->i_match_index > 0 )
        {
            p_rent->i_match_index = 0;
            p_target_line = p_rent->pp_target_lines[ 0 ];
        }
    }
    fclose( fp );

    return i_ret;
}

int main( void )
{
    return 0;
}
