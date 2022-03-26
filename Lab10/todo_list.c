typedef struct todo_list {
    size_t max_size;
    size_t cur_size;
    list_t* head = NULL;
} todo_list_t;

typedef struct list {
    const int32_t priority;
    const char* task;
    list_t* next;
} list_t;

todo_list_t init_todo_list(size_t max_size)
{
    todo_list_t todo_list = malloc(sizeof(todo_list_t);
    todo_list.max_size = max_size;
  
    return todo_list;
}

void finalize_todo_list(todo_list_t* todo_list)
{
    while() {
        free();
        free();
    }
    
    free(todo_list);
}

bool add_todo(todo_list_t* todo_list, const int32_t priority, const char* task)
{
    if (todo_list->cur_size >= todo_list->max_size) {
        return false;    
    }

    list_t list = malloc(sizeof(list_t));
    
    list.priority = priority;
    list.task = task;
    
    if (todo_list->cur_size == 0) {
        todo_list->head = &list;
        list.next = NULL;
        ++(todo_list->cur_size);

        return true;
    }

    list_t* next_node = todo_list->head;
    list_t* prev_node = todo_list->head;
 
    while (next_node->next != NULL) {


        if (list.priority > next_node->priority) {
            list.next = next_node;
            
            prev_node = &list;
        } 

        next_node = next_node->next;
    }    

    head = list;

    return true;
}

bool complete_todo(todo_list_t* todo_list);

const char* peek_or_null(const todo_list_t* todo_list);

size_t get_count(const todo_list_t* todo_list);

bool is_empty(const todo_list_t* todo_list);