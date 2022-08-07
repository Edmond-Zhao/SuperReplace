/* Created by Edmond  08/05/2022 */

#ifndef _SR_H_
#define _SR_H_

#include "mmanage.h"

#define MAX_LINE_LENGTH        1024
#define MAX_LINE_COUNT         10
#define MAX_FILE_PATH          512
#define BAK_FILE_SUFFIX        ".bak"
#define BAK_FILE_FOLDER        "backup"

typedef enum
{
    LOAD_FILE_LIST,
    LOAD_FIND_LINES,
    LOAD_REPLACE_LINES
} load_type_e;

typedef struct replace_ent_s
{
    int i_match_index;
    int i_matched_flag;
    int i_in_file_match_begain;
    int i_in_file_match_end;
    int i_file_path_count;
    int i_find_lines_count;
    int i_replace_lines_count;

    block_db_t *p_block_file_list;
    block_db_t *p_block_find;
    block_db_t *p_block_replace;
    char s_match_file_path[ MAX_FILE_PATH ];
} replace_ent_t;

int init_replace_ent( replace_ent_t *p_rent, const int i_frist_flag );
int uninit_replace_ent( replace_ent_t *p_rent );
int dump_replace_ent( replace_ent_t *p_rent );
int find_in_file( char *s_file_path, replace_ent_t *p_rent );
int replace_in_file( replace_ent_t *p_rent );
int load_param_from_file( replace_ent_t *p_rent, const char *s_name, const load_type_e load_type );

#endif /* _SR_H_ */

