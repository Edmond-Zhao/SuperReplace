/* Created by Edmond  08/05/2022 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmanage.h"

static line_node_t *node_get_new( void )
{
    line_node_t *p_node = malloc( sizeof( line_node_t ) );
    if( p_node )
        memset( p_node, 0, sizeof( line_node_t ) );
    
    return p_node;
}

static int node_add_line( line_node_t *p_node, const char *s_line )
{
    int i_length = 0;
    
    if( s_line == NULL || strlen( s_line ) == 0 )
    {
        printf( "The line string is unaviable.\n" );
        return -1;
    }

    i_length = strlen( s_line );
    p_node->p_line = ( char* )malloc( i_length + 1 );
    if( p_node->p_line == NULL )
    {
        printf( "Malloc failed!\n" );
        return -1;
    }
    memset( p_node->p_line, 0, i_length + 1 );
    strncpy( p_node->p_line, s_line, i_length );

    return 0;
}

static int node_release( line_node_t *p_node )
{
    if( p_node )
    {
        if( p_node->p_line )
            free( p_node->p_line );
        free( p_node );
    }
    
    return 0;
}

block_db_t *mmg_get_block_db( void )
{
    block_db_t *p_block_db = NULL;

    p_block_db = ( block_db_t * )malloc( sizeof( block_db_t ) );
    if( p_block_db == NULL )
    {
        printf( "Malloc failed!\n" );
        return NULL;
    }
    memset( p_block_db, 0, sizeof( block_db_t ) );

    return p_block_db;
}

int mmg_add_a_line( block_db_t *p_block_db, char *s_line )
{
    line_node_t *p_node = NULL;

    if( p_block_db == NULL || s_line == NULL )
        return -1;

    p_node = node_get_new();
    if( p_node == NULL )
        return -1;
    if( node_add_line( p_node, s_line ) != 0 )
    {
        node_release( p_node );
        p_node = NULL;
        return -1;
    }
    
    p_block_db->i_line_node_count++;
    if( p_block_db->p_node_head == NULL )
    {
        p_node->i_index = 1;
        p_block_db->p_node_head = p_node;
        p_block_db->p_node_end = p_node;
    }
    else
    {
        p_node->i_index = p_block_db->p_node_end->i_index + 1;
        p_block_db->p_node_end->p_next = p_node;
    }

    return 0;
}

char *mmg_get_first_line( block_db_t *p_block_db )
{
    if( p_block_db == NULL )
        return NULL;
    
    p_block_db->p_node_curr = p_block_db->p_node_head;

    return p_block_db->p_node_curr->p_line;
}

char *mmg_get_next_line( block_db_t *p_block_db )
{
    if( p_block_db == NULL )
        return NULL;

    if( p_block_db->p_node_curr != NULL )
        p_block_db->p_node_curr = p_block_db->p_node_curr->p_next;
    
    return p_block_db->p_node_curr->p_line;
}

int mmg_destroy( block_db_t *p_block_db )
{
    line_node_t *p_node = NULL;
    line_node_t *p_temp = NULL;

    if( p_block_db )
    {
        p_node = p_block_db->p_node_head;
        while( p_node )
        {
            p_temp = p_node;
            p_node = p_node->p_next;
            node_release( p_temp );
        }

        free( p_block_db );
    }

    return 0;
}

int mmg_block_db_dump( block_db_t *p_block_db )
{
    line_node_t *p_node = NULL;

    if( p_block_db )
    {
        printf( "Node count:     %d\n", p_block_db->i_line_node_count );
        printf( "Para file size: %zu\n", p_block_db->file_size );
        printf( "Block content: \n" );
        p_node = p_block_db->p_node_head;
        while( p_node )
        {
            printf( "    %s\n", p_node->p_line );
            p_node = p_node->p_next;
        }
    }

    return 0;
}