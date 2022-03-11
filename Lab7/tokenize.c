#include <stdlib.h>

#include "tokenize.h"

size_t my_strlen(const char* str)
{
    const char* p = str;

    while (*p++ != '\0') {
    }
    
    return p - str - 1;
}

char** tokenize_malloc(const char* str, const char* delim)
{
    const char* str_start_p = str;
    const char* str_p = str;
    size_t current_token = 0;
    size_t max_token = 0;
    char** tokens;
    char** tokens_ptr = NULL;

    while (*str_p != '\0') {
        const char* delim_p = delim;
        size_t len;

        while (*delim_p != '\0') {
            if (*str_p == *delim_p) {
                char* temp_str;
                size_t len;

                if (str_p == str_start_p) {
                    str_start_p = str_p + 1;

                    break;
                }    

                len = str_p - str_start_p + 1;
                temp_str = malloc(len);
                strncpy(temp_str, str_start_p, len);	
                temp_str[len - 1] = '\0';                

                if (current_token == max_token) {                    
                    tokens_ptr = realloc(tokens, sizeof(const char*) * ++max_token);
                    
                    if (tokens_ptr == NULL) {
                    }
           
                    tokens = tokens_ptr;
                    max_token++;
                }

                tokens[current_token++] = temp_str;
                
                str_start_p = str_p + 1;          
            }    
        
            ++delim_p;
        }

        ++str_p;
    }    
    
    tokens_ptr = realloc(tokens, sizeof(const char*) * ++max_token);

    if (tokens_ptr == NULL) {
    }

    tokens = tokens_ptr;

    tokens[current_token] = NULL;
    
    return tokens;
}
