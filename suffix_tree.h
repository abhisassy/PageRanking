#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
char choice ; 

/// Data Structures 

#define     DBL_WORD      unsigned long  
#define     MAX_DOC_SIZE  482483
#define     MAX_QUERY_LEN 1000 

DBL_WORD counter;
typedef struct SUFFIXTREENODE
{
   struct SUFFIXTREENODE*   sons;
   struct SUFFIXTREENODE*   right_sibling;
   struct SUFFIXTREENODE*   left_sibling;
   struct SUFFIXTREENODE*   father;
   struct SUFFIXTREENODE*   suffix_link;

   DBL_WORD path_position;
   DBL_WORD edge_label_start;
   DBL_WORD edge_label_end;
} NODE;


typedef struct SUFFIXTREE
{
   DBL_WORD e;
   char*  tree_string;
   DBL_WORD length;
   NODE*  root;
} SUFFIX_TREE;

typedef enum SKIP_TYPE     {skip, no_skip}                 SKIP_TYPE;
typedef enum RULE_2_TYPE   {new_son, split}                RULE_2_TYPE;
typedef enum LAST_POS_TYPE {last_char_in_edge, other_char} LAST_POS_TYPE;

NODE*    suffixless;

typedef struct SUFFIXTREEPATH
{
   DBL_WORD   begin;
   DBL_WORD   end;
} PATH;

typedef struct SUFFIXTREEPOS
{
   NODE*      node;
   DBL_WORD   edge_pos;
}POS;

/// key functions in implementation.c

SUFFIX_TREE* ST_CreateTree(const char*   str, DBL_WORD length);

int          ST_FindSubstring(SUFFIX_TREE* tree, char* W, DBL_WORD P,int index[], int captureSubstring);

void         ST_DeleteTree(SUFFIX_TREE* tree);


///Helper functions in implementation.c

NODE*    find_son(SUFFIX_TREE* tree, NODE* node, char character);
DBL_WORD get_node_label_end(SUFFIX_TREE* tree, NODE* node);
DBL_WORD get_node_label_length(SUFFIX_TREE* tree, NODE* node);
char     is_last_char_in_edge(SUFFIX_TREE* tree, NODE* node, DBL_WORD edge_pos);
void     connect_siblings(NODE* left_sib, NODE* right_sib);

NODE*    apply_extension_rule_2(NODE* node, DBL_WORD edge_label_begin, DBL_WORD edge_label_end, DBL_WORD path_pos, DBL_WORD edge_pos,RULE_2_TYPE type);    
NODE*    trace_single_edge(SUFFIX_TREE*    tree,NODE* node,PATH str,DBL_WORD* edge_pos,DBL_WORD* chars_found, SKIP_TYPE type, int* search_done);
NODE*    trace_string(SUFFIX_TREE* tree,NODE* node,PATH str,DBL_WORD* edge_pos,DBL_WORD* chars_found,SKIP_TYPE type);   

int      findleaves(NODE* node, int index[],int count );
void     follow_suffix_link(SUFFIX_TREE* tree, POS* pos);
void     create_suffix_link(NODE* node, NODE* link);
void     SEA(SUFFIX_TREE*   tree, POS* pos,PATH str,DBL_WORD* rule_applied,char after_rule_3);
void     SPA(SUFFIX_TREE* tree,POS* pos,DBL_WORD phase,DBL_WORD* extension,char* repeated_extension);

void     ST_DeleteSubTree(NODE* node);

char *strlwr(char *str);
//