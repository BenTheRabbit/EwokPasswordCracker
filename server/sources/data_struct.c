#include "data_struct.h"


/***********/
/** Queue **/
/***********/

void queue_init(queue *q)
{
    q->first = NULL;
    q->last = NULL;
}

void queue_enqueue(queue *q, void* data)
{
    queue_link *new_link;
    new_link = (queue_link*)malloc(sizeof(queue_link));
    new_link->data = data;
    new_link->next = NULL;

    if(q->first == NULL)
    {
        q->first = new_link;
    }
    else
    {
        q->last->next = new_link;
    }
    q->last = new_link;
}

void* queue_dequeue(queue *q)
{
    queue_link *tmp;
    void *data;

    if(q->first == NULL)
    {
        return NULL;
    }
    else
    {
        data = q->first->data;
        tmp = q->first;
        if(q->first->next == NULL)
            q->last = NULL;
        q->first = q->first->next;

        free(tmp);
    }
    return data;
}

int queue_isempty(queue *q)
{
    if(q->first == NULL)
        return 1;
    else
        return 0;
}


/**********/
/** List **/
/**********/

void list_init(list *l)
{
    l->size = 0;
    l->first = NULL;
}

void* list_get(list *l, int n)
{
    if(n < l->size && n >=0)
    {
        int i;
        list_link *tmp = l->first;

        for(i = 0; i < n; i++)
        {
            tmp = tmp->next;
        }
        return tmp->data;
    }
    else
    {
        return NULL;
    }
}

void list_add(list *l, void *data)
{
    list_link *new_link = NULL;

    new_link = (list_link*)malloc(sizeof(list_link));

    new_link->data = data;
    new_link->next = NULL;

    if(l->first != NULL)
    {
        list_link *tmp = l->first;

        while(tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = new_link;
    }
    else
    {
        l->first = new_link;
    }

    l->size++;


}

void list_remove(list *l, int n)
{
    if(n < l->size && n >= 0)
    {
        list_link *tmp = l->first;
        if(n == 0)
        {
            tmp = l->first;
            l->first = tmp->next;
            free(tmp);
        }
        else{
            int i = 0;
            list_link *tmp2;

            for(i = 0; i < n-1; i++)
            {
               tmp = tmp->next;
            }
            tmp2 = tmp->next->next;
            free(tmp->next);
            tmp->next = tmp2;
        }
        l->size--;
    }
}

int list_isempty(list *l)
{
    if(l->size > 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

int list_get_size(list *l)
{
    return l->size;
}
