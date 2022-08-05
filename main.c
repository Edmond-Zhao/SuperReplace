/*
 * Super Relpace
 * Created by Edmond Zhao (edmondzym@gmail.com)
 * 08/04/2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "sr.h"

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
    printf( "target line count: %d\n", i_pgh_count );

    rent.i_target_lines_count = i_pgh_count;
    rent.pp_target_lines = s_target_pgh;

    i_pgh_count = 0;
    pp_line = s_replace_with;
    while( *pp_line != NULL )
    {
        ++i_pgh_count;
        pp_line++;
    }
    printf( "replace line count: %d\n", i_pgh_count );
    rent.i_replace_lines_count = i_pgh_count;
    rent.pp_replace_lines = s_replace_with;
    
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
    printf( "%*s\r", 150, "" );
    printf( "Done!\n" );

    return 0;
}

