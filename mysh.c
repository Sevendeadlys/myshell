#include "mysh.h"


void* Malloc(unsigned int size)
{
    void *p;

    p = malloc(size);
    if(!p)
    {
        printf("malloc error\n");
        exit(0)
    }
    return p;
}

int parseline(char* line,char** argv)
{
    char *p,*q,*pos;
    int count;

    p = line;
    q = *argv;

    while(*p && *p == ' ') p++;

    for(; *p && *p != '\n'; p++)
    {
        pos = strchr(p,' ');
        *pos = '\0';
        strcpy(q,p);

        while(*p && *p == ' ')p++;
        
    }

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
