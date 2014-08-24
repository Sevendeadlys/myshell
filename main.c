#include "csapp.h"
#include "mysh.h"

int main(int argc,char**argv)
{
    pid_t pid;
    task_struct *head,*node;
    int n;
    int bg;
    char buf[MAXBUF];
    char *arg[MAXARGC];

    head = (task_struct*)Malloc(sizeof(task_struct));
    if(argv[argc] && !strcmp(argv[argc],"&")) head->bg = EXECVE_IN_BG;
    else head->bg = EXECVE_IN_FG;
    head->pid = getpid();
    head->next = head;
    head->pre = head;
    strcpy(head->name,argv[0]);

    while((n = read(STDIN_FILENO,buf,MAXBUF)) > 0)
    {
        bg = parseline(buf,arg);

       if(bg < 0) continue;

       if((pid = Fork()) == 0)
       {
           Execve(arg[0],argv,environ);
           exit(0);
       }

       node = (task_struct*)Malloc(sizeof(task_struct));
       taskop(arg[0],pid,bg,node);
       add_tail_to_task(head,node);
       waitpid(-1,NULL,0);
    }

    exit(0);

}
