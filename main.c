#include "csapp.h"
#include "mysh.h"

int main(int argc,char**argv)
{
    pid_t pid;
    task_struct *head,*node;
    int bg;
    char buf[MAXBUF];
    char *arg[MAXARGC];
    char c;
    int n = 0;

    head = (task_struct*)Malloc(sizeof(task_struct));
    if(argv[argc] && !strcmp(argv[argc],"&")) head->bg = EXECVE_IN_BG;
    else head->bg = EXECVE_IN_FG;
    head->pid = getpid();
    head->next = head;
    head->pre = head;
    strcpy(head->name,argv[0]);

    c = getchar();

    while(c != EOF)
    {
        if(c == '\n' && n != 0)
        {
            bg = parseline(buf,arg);
            if((pid = Fork()) == 0)
            {
                Execve(arg[0],arg,environ);
                memset(buf,0,MAXBUF);
                n = 0;
            }
            node = (task_struct*)Malloc(sizeof(task_struct));
            taskop(arg[0],pid,bg,node);
            add_tail_to_task(head,node);
        }
        buf[n] = c;
        n ++;
        c = getchar();
    }
    waitpid(-1,NULL,0);

    exit(0);

}
