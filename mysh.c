#include "mysh.h"

task_func   *froot;
task_struct *root;



void handlerSigint()
{
    task_struct *node;

    node = root->next;

    for(;node != root;node = node->next)
    {
        if(node->bg == EXECVE_IN_FG)
        {
            Kill(node->pid,SIGINT);
            if(waitpid(node->pid,NULL,0) < 0) unix_error("waitpid error");

            delete_node(node);
        }
    }
    return;
}

void handlerSigtstp()
{
    task_struct *node;

    node = root->next;

    for(;node != root;node = node->next)
    {
        if(node->bg == EXECVE_IN_FG)
        {
            Kill(node->pid,SIGTSTP);
        }
    }
    return;
}

int parseline(char* line,char** argv)
{
    char *p,*pos;
    int count = 0;
    int bg = 0;

    p = line;

    while(*p && *p == ' ') p++;

    while((pos = strchr(p,' ')))
    {
        *pos = '\0';
        argv[count] = p;
        count ++;
        p = pos + 1;
        while(*p && *p == ' ')p++;
    }
    if(*p)
    {
        argv[count] = p;
        count ++;
    }

    if(count == 0)
    {
        printf("error input!\n");
        return -1;
    }

    if(argv[count - 1][0] == '&')
    {
        bg = 1;
        count = count - 1;
    }
    argv[count] = NULL;

    return bg;
}

int getcmdline(char* cmdline)
{
    int count = 0;

    printf("mysh> ");
    Fgets(cmdline,MAXLINE,stdin);

    if(feof(stdin))
    {
        printf("\n");
        exit(0);
    }
    count = strlen(cmdline);
    if(*(cmdline + count - 1) = '\n'){
        *(cmdline + count - 1) = '\0';
        count --;
    }
    return count;
}

int quit(char** argv)
{
    if(!strcmp(argv[0],"quit")) exit(0);

    if(!strcmp(argv[0],"&")) return 1;

    return 0;
}

int printPid(char** argv)
{
    task_struct* node;
    if(!strcmp(argv[0],"task"))
    {
        ptask(root);
        node = root->next;
        for(;node != root;node = node->next)
        {
            ptask(node);
        }
        return 1;
    }
    return 0;
}

void ptask(task_struct* node)
{
    printf("**************************************\n");
    printf("name = %s\n",node->name);
    printf("pid = %d\n",node->pid);
    if(node->bg == EXECVE_IN_BG)
    {
        printf("the program is execve in background\n");
    }
    else
    {
        printf("the program is not execve in background\n");
    }
    printf("**************************************\n");
}

int fgbg(task_struct* head,char** argv)
{
    task_struct* node;

    if(!strcmp(argv[0],"fg") || !strcmp(argv[0],"bg"))
    {
        node = jid(head,argv[1]);
        if(node == NULL)
        {
            printf("Wrong Input!!!\n");
        }
        else
        {
            Kill(node->pid,SIGCONT);
            node->bg = ((node->bg == EXECVE_IN_BG)? EXECVE_IN_FG:EXECVE_IN_BG);
        }
        return 1;
    }

    return 0;
}

task_struct* jid(task_struct* head,char* idstring)
{
    int pos;
    pid_t pid;
    task_struct* node = NULL;
    char* p = idstring;

    if(p == NULL)
    {
        node = search_task_by_index(head,1);
    }
    else if(*p == '%')
    {
        p++;
        pos = (int)atoi(p);
        node = search_task_by_index(head,pos);
    }
    else
    {
        pid = (pid_t)atoi(p);
        node = search_task_by_pid(head,pid);
    }

    return node;
}

void eval(char *cmdline,task_struct* head)
{
    pid_t pid;
    task_struct* node;
    int bg;
    char *arg[MAXARGC];
    char buf[MAXBUF];

    strcpy(buf,cmdline);
    bg = parseline(buf,arg);

    if(!quit(arg))
    {
        if(fgbg(head,arg) != 0)
        {
            return ;
        }
        if(printPid(arg) != 0)
        {
            return;
        }
        pid = Fork();
        if(pid != 0)
        {
            node = (task_struct*)Malloc(sizeof(task_struct));
            taskop(arg[0],pid,bg,node);
            add_tail_to_task(head,node);
        }
        if(pid == 0)
        {
            if(execve(arg[0],arg,environ) < 0)
            {
                printf("%s: command not found\n",arg[0]);
                exit(8);
            }
        }
        if(!bg)
        {
            int status;
            if(waitpid(pid,&status,0) < 0) unix_error("waitpid error");
            if(WIFEXITED(status))
            {
                printf("%d is removed\n",node->pid);
                delete_node(node);
            }
        }
        else
        {
            printf("%d %s\n",pid,cmdline);
        }

    }
    return ;
}

void taskop(char *name,pid_t pid,int bg,task_struct* node)
{
    strcpy(node->name,name);
    node->pid = pid;
    node->bg = bg;
    node->pre = NULL;
    node->next = NULL;
}

void add_tail_to_task(task_struct* head,task_struct* node)
{
    head->pre->next = node;
    node->pre = head->pre;
    node->next = head;
    head->pre = node;
}

void delete_node(task_struct* node)
{
    node->next->pre = node->pre;
    node->pre->next = node->next;
    free(node);
}

task_struct* search_task_by_pid(task_struct* head,int pid)
{
    task_struct* p;

    for(p = head->next;p != head;p = p->next)
    {
        if(p->pid == pid && p->bg == EXECVE_IN_BG)
        {
            return p;
        }
    }
    return NULL;
}

task_struct* search_task_by_index(task_struct* head,int index)
{
    task_struct* p;
    int count = 0;

    for(p = head->next;p != head;p = p->next)
    {
        if(p->bg == EXECVE_IN_BG)
        {
            count ++;
            if(count == index)
            {
                return p;
            }
        }
    }
    return NULL;
}
