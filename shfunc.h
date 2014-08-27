#ifndef _SHFUNC_H__
#define _SHFUNC_H__

#include "csapp.h"
#include "mysh.h"

#define QUIT_STR "quit"
#define QUIT_FUNC quit_func

#define TASK_STR "task"
#define TASK_FUNC task_func

#define FG_STR "fg"
#define FG_FUNC fgbg_func

#define BG_STR "bg"
#define BG_FUNC fgbg_func

#define 

void init_froot();
int search_func_by_str(char* str);

#endif
