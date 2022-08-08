/*
 * Super Relpace
 * Created by Edmond Zhao (edmondzym@gmail.com)
 * 08/04/2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sr.h"

/* defined for test */
#define TEST_FILE_LIST_FILE      "test/file_list.txt"
#define TEST_FIND_LINES_FILE     "test/find_lines.txt"
#define TEST_REPLACE_LINES_FILE  "test/replace_lines.txt"

int main( void )
{
    replace_ent_t rent = { 0 };

    init_replace_ent( &rent, 1 );
    load_param_from_file( &rent, TEST_FILE_LIST_FILE, LOAD_FILE_LIST );
    printf( "load block from file: %s\n", TEST_FILE_LIST_FILE );
    //dump_replace_ent( &rent );

    load_param_from_file( &rent, TEST_FIND_LINES_FILE, LOAD_FIND_LINES );
    printf( "load block from file: %s\n", TEST_FIND_LINES_FILE );
    //dump_replace_ent( &rent );

    load_param_from_file( &rent, TEST_REPLACE_LINES_FILE, LOAD_REPLACE_LINES );
    printf( "load block from file: %s\n", TEST_REPLACE_LINES_FILE );
    dump_replace_ent( &rent );

 
/*    
    for( i = 0; i < gi_file_path_count; ++i )
    {
        printf( "Finding in file:  %s\r", s_file_path_list[ i ] );
        find_in_file( s_file_path_list[ i ], &rent );
        if( rent.i_matched_flag )
        {
            printf( "Match lines section: %d -- %d\n", 
                rent.i_in_file_match_begain, rent.i_in_file_match_end );
            replace_in_file( &rent );
        }
        init_replace_ent( &rent, 0 );
    }
    printf( "%*s\r", 150, "" );*/
    uninit_replace_ent( &rent );
    printf( "Done!\n" );

    return 0;
}

