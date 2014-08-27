#ifndef  _MY_SHELL_H__
#define _MY_SHELL_H__

#include "csapp.h"

#define EXECVE_IN_BG 1
#define EXECVE_IN_FG 0
#define MAXNAME 200
#define MAXARGC 100
#define MAXFUNC 100

#define INIT_TASK_STRUCT(name) (&name)
#define for_each(root,p,func,sig) do{ \
   for(;p != root;p = p->next) func(p->pid,sig); \
}while(0)

typedef struct task_node{
    char name[MAXNAME];
    pid_t pid;
    int bg;
    struct task_node *pre;
    struct task_node *next;
}task_struct;

typedef struct shfunc
{
    char *name;
    int (*func)(char** argv);
    struct shfunc *next;
}task_func;

extern task_func*   froot;
extern task_struct* root;

int printPid(char** argv);
void ptask(task_struct* node);
void handlerSigint();
void handlerSigtstp();
int parseline(char* line,char** argv);
int getcmdline(char* cmdline);
int quit(char** argv);
int fgbg(task_struct* head,char** argv);
task_struct* jid(task_struct* head,char* idstring);
void eval(char *cmdline,task_struct* head);
void taskop(char *name,pid_t pid,int bg,task_struct* node);
void add_tail_to_task(task_struct* head,task_struct* node);
void delete_node(task_struct* node);
task_struct* search_task_by_pid(task_struct* head,int pid);
task_struct* search_task_by_index(task_struct* head,int index);

#endif
