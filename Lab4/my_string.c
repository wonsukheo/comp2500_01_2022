#include "my_string.h"

#define NULL ((void*)0)

/* should we do null check? */

size_t my_strlen(const char* str)
{
    const char* str_ptr = str;

    if (str == NULL) {
        return -1;
    }

    while (*str_ptr++ != '\0') {
    }         
    
    return str_ptr - str - 1;    
}

void reverse(char* str)
{
    char* str_end = str + my_strlen(str) - 1;

    if (str == NULL) {
        return;
    }

    while (str_end > str) {
        char c = *str;
        *str = *str_end;
        *str_end = c;
        
        ++str;
        --str_end;
    }
}

int index_of(const char* str, const char* word)  
{
    const char* str_start = str;

    if (str == NULL || word == NULL) {
        return -1;
    }

    if (*word == '\0') {
        return 0;
    }

    while (*str != '\0') {
        if (*str == *word) {
            const char* str_ptr = str;
            const char* word_ptr = word;  

            while (*++word_ptr != '\0') {
                if (*word_ptr != *++str_ptr) {
                    break;
                }
            }       
                
            if (*word_ptr == '\0') {
                return str - str_start;
            }   
        }
        
        ++str;
    }    

    return -1;
}

/* why no !? */

void reverse_by_words(char* str) 
{
    char* str_ptr = str;

    if (str == NULL) {
        return;
    }
 
    while (*str != '\0') {
        if (*str == ' ') {
            *str = '\0';
            reverse(str_ptr);
            
            *str = ' ';
            str_ptr = str + 1;
        }
        
        ++str;
    }
    
    reverse(str_ptr);  
}

char* tokenize(char* str_or_null, const char* delims)
{
    static char* s_str = NULL;
    
    char* str_ptr;
    const char* delims_ptr = delims;

    if (str_or_null != NULL) {
        s_str = str_or_null;
    }

    if (s_str == NULL) {
        return NULL;
    }
    
    if (delims == NULL) {
        return s_str;
    }

    str_ptr = s_str;

    while (*s_str != '\0') {
        while (*delims_ptr != '\0') {
            if (*s_str == *delims_ptr++) {
                if (s_str == str_ptr) {
                    continue;
                }

                *s_str = '\0';

                ++s_str;
                return str_ptr;
            }           
        }

        delims_ptr = delims;       
        ++s_str;
    }
    
    return str_ptr;
}

/* check if block scope static var can have same name */

char* reverse_tokenize(char* str_or_null, const char* delims)
{
    char* str = tokenize(str_or_null, delims);
    reverse(str);
  
    return str;
}
