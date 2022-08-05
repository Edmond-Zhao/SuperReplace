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

    char **pp_target_lines;
    char s_match_file_path[ MAX_FILE_PATH ];
} replace_ent_t;

/* ---------- defined for test BEGIN ---------- */
char s_file_path_list[][ 256 ] = {
    "proto/802.11ah.h",
    "proto/802.11e.h",
    "proto/802.11r.h",
    "proto/802.1d.h",
    "proto/802.3.h",
    "proto/bcmarp.h",
    "proto/bcmdhcp.h",
    "proto/bcmeth.h",
    "proto/bcmevent.h",
    "proto/bcmicmp.h",
    "proto/bcmip.h",
    "proto/bcmipv6.h",
    "proto/bcmproto.h",
    "proto/bcmtcp.h",
    "proto/bcmudp.h",
    "proto/eap.h",
    "proto/eapol.h",
    "proto/ethernet.h",
    "proto/event_log_set.h",
    "proto/event_log_tag.h",
    "proto/gspi.h",
    "proto/nan.h",
    "proto/p2p.h",
    "proto/sdspi.h",
    "proto/vlan.h",
    "proto/wpa.h",
    "proto/wps.h"
};

int gi_file_path_count = sizeof( s_file_path_list ) / 256;

char *s_target_pgh[] = {
    "<<<<<<< HEAD",
    " * Copyright 2021 Broadcom",
    "=======",
    " * Copyright 2022 Broadcom",
    ">>>>>>> broadcom_vendor_9.5.0h3",
    NULL
};

/* ---------- defined for test END ---------- */


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
    if( p_rent->pp_target_lines == NULL || p_rent->i_lines_count < 1 )
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

int main( void )
{
    replace_ent_t rent = { 0 };
    char **pp_line = NULL;
    int i_pgh_count = 0;
    int i = 0;

    init_replace_ent( &rent, 1 );
    printf( "file count: %d\n", gi_file_path_count );

    pp_line = s_target_pgh;
    while( *pp_line != NULL )
    {
        ++i_pgh_count;
        pp_line++;
    }
    printf( "line count: %d\n", i_pgh_count );

    rent.i_lines_count = i_pgh_count;
    rent.pp_target_lines = s_target_pgh;
    
    for( i = 0; i < gi_file_path_count; ++i )
    {
        find_in_file( s_file_path_list[ i ], &rent );
        if( rent.i_matched_flag )
        {
            printf( "Match lines section: %d -- %d\n", 
                rent.i_in_file_match_begain, rent.i_in_file_match_end );
        }
        init_replace_ent( &rent, 0 );
    }

    return 0;
}

