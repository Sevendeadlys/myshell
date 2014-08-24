#include "mysh.h"

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
