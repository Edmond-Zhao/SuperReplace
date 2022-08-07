/* Created by Edmond  08/05/2022 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mmanage.h"

block_db_t *mmg_get_block_db( const size_t ul_mem_size )
{
    block_db_t *p_block_db = NULL;
    char *p_block = NULL;

    if( ul_mem_size == 0 || ul_mem_size > MAX_BLOCK_SIZE )
        return NULL;

    p_block_db = ( block_db_t * )malloc( sizeof( block_db_t ) );
    if( p_block_db == NULL )
    {
        printf( "Malloc failed!\n" );
        return NULL;
    }
    memset( p_block_db, 0, sizeof( block_db_t ) );
    p_block = ( char * )malloc( ul_mem_size );
    if( p_block == NULL )
    {
        printf( "Failed to malloc for block!\n" );
        free( p_block_db );
        return NULL;
    }
    memset( p_block, 0, ul_mem_size );
    p_block_db->block_size = ul_mem_size;
    p_block_db->p_block = p_block;
    p_block_db->p_block_end = p_block + ul_mem_size - 1;
    p_block_db->p_w_end = p_block;

    return p_block_db;
}

static line_node_t *get_node( void )
{
    line_node_t *p_node = malloc( sizeof( line_node_t ) );
    if( p_node )
        memset( p_node, 0, sizeof( line_node_t ) );
    
    return p_node;
}

int mmg_add_a_line( block_db_t *p_block_db, char *s_line )
{
    line_node_t *p_node = NULL;
    int i_line_len = 0;

    if( p_block_db == NULL || s_line == NULL )
        return -1;

    i_line_len = strlen( s_line );
    if( p_block_db->p_w_end + i_line_len > p_block_db->p_block_end )
    {
        printf( "The mem db is full, can't be added new lines.\n" );
        return -1;
    }

    p_node = get_node();
    if( p_node == NULL )
        return -1;
    strcpy( p_block_db->p_w_end, s_line );
    p_node->p_line = p_block_db->p_w_end;
    p_block_db->p_w_end = p_block_db->p_w_end + i_line_len + 1;
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
        if( p_block_db->p_block )
            free( p_block_db->p_block );
        
        p_node = p_block_db->p_node_head;
        while( p_node )
        {
            p_temp = p_node;
            p_node = p_node->p_next;
            free( p_temp );
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
        printf( "Node count:  %d\n", p_block_db->i_line_node_count );
        printf( "Block size:  %zu bytes\n", p_block_db->block_size );
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