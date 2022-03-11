#include <stdlib.h>
#include <string.h>

#include "tokenize.h"

char** tokenize_malloc(const char* str, const char* delim)
{
    const char* str_start_p = str;
    const char* str_p = str;

    char** tokens = NULL;
    char** tokens_p = NULL;
   
    size_t i = 0;

    while (*str_p != '\0') {
        const char* delim_p = delim;
  
        while (*delim_p != '\0') {
            if (*str_p == *delim_p) {
                size_t len;
                char* pa_str;

                if (str_p == str_start_p) {
                    str_start_p = str_p + 1;

                    break;
                }    
 
                len = str_p - str_start_p;
                pa_str = malloc(len + 1);

                strncpy(pa_str, str_start_p, len);

                pa_str[len] = '\0';             
               
                tokens_p = (char**) realloc(tokens, sizeof(const char*) * ++i);

                tokens = tokens_p;

                tokens[i - 1] = pa_str;
                
                str_start_p = str_p + 1;          
            }    
        
            ++delim_p;
        }

        ++str_p;
    }    

    if (i == 0 && *str != '\0') {
        size_t len;
        char* pa_str;
       
        tokens_p = (char**) realloc(tokens, sizeof(const char*) * ++i);

        tokens = tokens_p;

        len = strlen(str);

        pa_str = malloc(len + 1);

        memcpy(pa_str, str, len + 1);

        tokens[i - 1] = pa_str;     
    }

    tokens_p = (char**) realloc(tokens, sizeof(const char*) * ++i);

    tokens = tokens_p;

    tokens[i - 1] = NULL;
    
    return tokens;
}
