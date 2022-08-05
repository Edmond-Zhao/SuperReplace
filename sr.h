/* Created by Edmond  08/05/2022 */

#ifndef _SR_H_
#define _SR_H_

#define MAX_LINE_LENGTH        1024
#define MAX_LINE_COUNT         10
#define MAX_FILE_PATH          512
#define BAK_FILE_SUFFIX        ".bak"
#define BAK_FILE_FOLDER        "backup"

typedef struct replace_ent_s
{
    int i_match_index;
    int i_matched_flag;
    int i_in_file_match_begain;
    int i_in_file_match_end;
    int i_target_lines_count;
    int i_replace_lines_count;

    char **pp_target_lines;
    char **pp_replace_lines;
    char s_match_file_path[ MAX_FILE_PATH ];
} replace_ent_t;

int init_replace_ent( replace_ent_t *p_ent, const int i_frist_flag );
int find_in_file( char *s_file_path, replace_ent_t *p_rent );
int replace_in_file( replace_ent_t *p_rent );

#endif /* _SR_H_ */

