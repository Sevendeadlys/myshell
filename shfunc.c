#include "shfunc.h"

void init_froot()
{
}

int search_func_by_str(char* str)
{
    task_func* p;
    for(p = froot;p != NULL;p = p->next)
    {
        if(!strcmp(p->name,str))
        {
            return p;
        }
    }
    return p;
}
