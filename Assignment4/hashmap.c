#include "hashmap.h"
#include "string.h"
#include "stdlib.h"

hashmap_t* init_hashmap_malloc(size_t length, size_t (*p_hash_func)(const char* key))
{
    hashmap_t* pa_ret = (hashmap_t*)malloc(sizeof(hashmap_t));

    pa_ret->plist = (node_t**)calloc(length, sizeof(node_t*));

    pa_ret->length = length;

    pa_ret->hash_func = p_hash_func;

    return pa_ret;
}

int add_key(hashmap_t* hashmap, const char* key, const int value)
{
    size_t hash_val = hashmap->hash_func(key) % hashmap->length;
    
    node_t* pa_node = (node_t*)malloc(sizeof(node_t));
    
    node_t** p = hashmap->plist + hash_val;

    node_t** next_p;

    pa_node->key = (char*)malloc(sizeof(char) * my_strlen(key));
    memcpy(pa_node->key, key, my_strlen(key));    
    pa_node->value = value;
    pa_node->next = NULL;
    
    if (*p == NULL) {
        *p = pa_node;
        return TRUE;      
    } else if (my_strcmp(key, (*p)->key) == 0) {
        goto free_n_ret;
    }

    next_p = &(*p)->next;

    while (*next_p != NULL) {
        if (my_strcmp(key, (*next_p)->key) == 0) {
            goto free_n_ret;
        }

        next_p = &(*next_p)->next;
    }
            
    *next_p = pa_node;

    return TRUE;

free_n_ret:
    free(pa_node->key);
    free(pa_node);

    return FALSE;    
}

int get_value(const hashmap_t* hashmap, const char* key)
{
    size_t hash_val = hashmap->hash_func(key) % hashmap->length;

    node_t** p = hashmap->plist + hash_val;
    
    node_t** next_p;

    if (*p == NULL) {
        return -1;
    } else if (my_strcmp(key, (*p)->key) == 0) {
        return (*p)->value;
    }

    next_p = &(*p)->next;

    while (*next_p != NULL) {
        if (my_strcmp(key, (*next_p)->key) == 0) {
            return (*next_p)->value;
        }

        next_p = &(*next_p)->next;
    }

    return -1;
}

int update_value(hashmap_t* hashmap, const char* key, const int value)
{
    size_t hash_val = hashmap->hash_func(key) % hashmap->length;

    node_t** p = hashmap->plist + hash_val;

    node_t** next_p;

    if (*p == NULL) {
        return FALSE;
    } else if (my_strcmp(key, (*p)->key) == 0) {
        (*p)->value = value;
        return TRUE;
    }

    next_p = &(*p)->next;

    while (*next_p != NULL) {
        if (my_strcmp(key, (*next_p)->key) == 0) {
            (*next_p)->value = value;

            return TRUE;
        }

        next_p = &(*next_p)->next;
    }

    return FALSE;    
}

int remove_key(hashmap_t* hashmap, const char* key)
{
    size_t hash_val = hashmap->hash_func(key) % hashmap->length;

    node_t** p = hashmap->plist + hash_val;

    node_t** next_p;

    if (*p == NULL) {
        return FALSE;
    } else if (my_strcmp(key, (*p)->key) == 0) {
        if ((*p)->next != NULL) {
            node_t* temp = *p;

            *(hashmap->plist + hash_val) = (*p)->next;
            
            free(temp);
        } else {
            *(hashmap->plist + hash_val) = NULL;
        }

        return TRUE;
    }

    next_p = &(*p)->next;

    while (*next_p != NULL) {
        if (my_strcmp(key, (*next_p)->key) == 0) {
            if ((*next_p)->next != NULL) {
                node_t* temp = *p;

                next_p = &(*next_p)->next;
 
                free(temp);
            } else { 
                next_p = NULL;
            }

            return TRUE;
        }

        next_p = &(*next_p)->next;
    }

    return FALSE;        
}

void destroy(hashmap_t* hashmap)
{
    node_t** p = hashmap->plist;
    node_t* next_p;
 
    while (p - hashmap->plist != (int)(hashmap->length)) {
        if (*p == NULL) {
            ++p;
            continue;
        }
        
        next_p = (*p)->next;
  
        while (next_p != NULL) {
            node_t* temp = next_p;

            next_p = next_p->next;
         
            free(temp->key);
            free(temp);        
        }           

        free(*p++);
    }
}

size_t my_strlen(const char* str)
{
    const char* p = str;
 
    while (*p++ != '\0') {
    }
  
    return p - str;
}

int my_strcmp(const char* str1, const char* str2)
{
    const char* p1 = str1;
    const char* p2 = str2;

    while (*p1 != '\0' && *p1 == *p2) {
        ++p1;
        ++p2;
    }
 
    return *p1 - *p2;
}
