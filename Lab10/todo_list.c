#include "todo_list.h"

#include <stdlib.h>
#include <stdio.h>

todo_list_t init_todo_list(size_t max_size)
{
    todo_list_t* pa_todo_list = (todo_list_t)malloc(sizeof(todo_list_t));
    pa_todo_list->max_size = max_size;
    pa_todo_list->cur_size = 0;
    pa_todo_list->next = NULL;

    return *pa_todo_list;
}

void finalize_todo_list(todo_list_t* todo_list)
{
    list_t* p = todo_list->head;

    while(p != NULL) {
        list_t* temp = p->next;
          
        free(p);

        p = temp;
    }
    
    free(todo_list);
}

bool add_todo(todo_list_t* todo_list, const int32_t priority, const char* task)
{
    if (todo_list->cur_size == todo_list->max_size) {
        return false;    
    }

    list_t list = malloc(sizeof(list_t));
    
    list.priority = priority;
    list.task = task;

    list_t** prev_node = &(todo_list->head);
    list_t* next_node = todo_list->head;
 
    while (next_node != NULL) {
        if (list.priority > next_node->priority) {
            list.next = next_node;
            
            *prev_node = &list;
            
            goto ret;
        } 

        next_node = next_node->next;
    }    

    next_node = &list;
    list.next = NULL;

ret: 
    ++(todo_list->cur_size);

    return true;
}

bool complete_todo(todo_list_t* todo_list)
{
    if (todo_list->head == NULL) {
        return false;
    }
  
    list_t* temp = todo_list->head;

    todo_list->head = temp->next;

    free(temp);

    --(todo_list->cur_size);

    return true;
}

const char* peek_or_null(const todo_list_t* todo_list)
{
    return (todo_list->head == NULL) ? NULL : (todo_list->head)->task;
}

size_t get_count(const todo_list_t* todo_list)
{
    return todo_list->cur_size;
}

bool is_empty(const todo_list_t* todo_list)
{
    return (todo_list->cur_size == 0) ? true : false;
}

