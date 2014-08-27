#include "csapp.h"
#include "mysh.h"

int main(int argc,char**argv)
{
    pid_t pid;
    task_struct *head;
    int bg;
    char cmdline[MAXLINE];

    head = (task_struct*)Malloc(sizeof(task_struct));
    if(argv[argc] && !strcmp(argv[argc],"&")) head->bg = EXECVE_IN_BG;
    else head->bg = EXECVE_IN_FG;
    head->pid = getpid();
    head->next = head;
    head->pre = head;
    strcpy(head->name,argv[0]);
    root = head;

    Signal(SIGINT,handlerSigint);
    Signal(SIGTSTP,handlerSigtstp);

    while(1)
    {
        memset(cmdline,'\0',MAXLINE);
        if(getcmdline(cmdline) < 1) continue;
        eval(cmdline,head);
    }
    exit(0);
}
