#ifdef  _MY_SHELL_H__
#define _MY_SHELL_H__ 

#include "csapp.h"

#define EXECVE_IN_BG 1
#define EXECVE_IN_FG 0
#define MAXNAME 20
#define MAXARGC 100

#define INIT_TASK_STRUCT(name) (&name)
#define for_each(root,p,func,sig) do{ \
   for(;p != root;p = p->next) func(p->pid,sig); \
}while(0)

typedef struct task_stuct
{
    char name[MAXNAME];
    pid_t pid;
    int bg;
    struct task_stuct *pre;
    struct task_stuct *next;
}task_stuct,*ptask_struct;

void taskop(pid_t pid,int bg,task_struct* node);
void add_tail_to_task(task_struct* head,task_struct* node);
void delete_node(task_struct* node);
task_struct* search_task_by_pid(task_struct* head,int pid);
task_struct* search_task_by_index(task_struct* head,int index);

#endif
