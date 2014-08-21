#include "csapp.h"
#include "mysh.h"

int main(int argc,char**argv)
{
    pid_t pid;
    task_struct *head;
    int n;
    char buf[MAXBUF];
    char argv[MAXARGC][MAXLINE];

    head = (task_struct*)Malloc(sizeof(task_struct));
    if(!strcmp(argv[argc],"&")) head->bg = EXECVE_IN_BG;
    else head->bg = EXECVE_IN_FG;

    while((n = read(STDIN_FILENO,buf,MAXBUF)) > 0)
    {

    }

}
