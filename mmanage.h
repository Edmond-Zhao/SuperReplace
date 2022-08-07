/* Created by Edmond  08/05/2022 */

#ifndef _MMANAGE_H_
#define _MMANAGE_H_

#define MAX_BLOCK_SIZE        (1024*1024)

typedef struct line_node_s
{
    int i_index;
    char *p_line;

    struct line_node_s *p_next;
} line_node_t;

typedef struct block_db_s
{
    int i_line_node_count;
    size_t block_size;

    line_node_t *p_node_head;
    line_node_t *p_node_end;
    line_node_t *p_node_curr;
    char *p_block;
    char *p_block_end;
    char *p_w_end;
} block_db_t;

block_db_t *mmg_get_block_db( const size_t ul_mem_size );
int mmg_add_a_line( block_db_t *p_block_db, char *s_line );
char *mmg_get_first_line( block_db_t *p_block_db );
char *mmg_get_next_line( block_db_t *p_block_db );
int mmg_destroy( block_db_t *p_block_db );
int mmg_block_db_dump( block_db_t *p_block_db );

#endif /* _MMANAGE_H_ */
