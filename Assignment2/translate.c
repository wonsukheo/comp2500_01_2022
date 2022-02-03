#include <stdio.h>

#include "translate.h"

#define BUFFER_LEN (512)

int translate(int argc, const char** argv)
{
    const char* set1;
    const char* set1_p;
    const char* set2;
    int range_set_return;
    char c;


    char set1_cpy[512];
    char set2_cpy[512];
    
    size_t set1_len;
    size_t set2_len;

    int flag_i;

    if (argc != 3 && argc != 4) {
        return ERROR_CODE_WRONG_ARGUMENTS_NUMBER;
    }    

    if (argc == 4) { 
        if (my_strcmp(argv[1], "-i") != 0) {
            return ERROR_CODE_INVALID_FLAG;
        }

        set1 = argv[2];
        set2 = argv[3];

        flag_i = TRUE;
    } else {
        set1 = argv[1];
        set2 = argv[2];
    
        flag_i = FALSE;
    }

    set1_len = my_strlen(set1);
    set2_len = my_strlen(set2);    

    if (set1_len > 511 || set2_len > 511) {
        return ERROR_CODE_ARGUMENT_TOO_LONG;
    }    

    my_strncpy(set1_cpy, set1, BUFFER_LEN - 1);
    my_strncpy(set2_cpy, set2, BUFFER_LEN - 1);

    if (replace_escape_char(set1_cpy) == 3 || replace_escape_char(set2_cpy) == 3) {
        return ERROR_CODE_INVALID_FORMAT;
    }

    range_set_return = replace_range_set(set1_cpy);

    if (range_set_return != 0) {
        return range_set_return;
    }

    range_set_return = replace_range_set(set2_cpy);

    if (range_set_return != 0) {
        return range_set_return;
    }

    set1_len = my_strlen(set1_cpy);
    set2_len = my_strlen(set2_cpy);  
 
    set1_p = set1_cpy + set1_len - 1;
    
    c = getchar();
    
    if (flag_i && my_is_alpha(c)) {
        c |= 0x20;
    }
  
    while (c != EOF) {
        while (set1_p >= set1_cpy) {
            if (c == *set1_p || (flag_i && (c & ~0x20) == *set1_p)) {
                if (set1_p - set1_cpy >= (int)set2_len) {
                    c = set2_cpy[set2_len - 1];
                } else {
                    c = set2_cpy[set1_p - set1_cpy];
                }
            }
            
            --set1_p;
        }    
        
        set1_p = set1_cpy + set1_len - 1;
   
        putchar(c);

        c = getchar();
        
        if (flag_i && my_is_alpha(c)) {
            c |= 0x20;
        }
    }
    
    return 0;
}

int replace_escape_char(char* set)
{
    char escape_char[] = { '\\', 'a', 'b', 'f', 'n', 'r', 't', 'v', '\'', '\"', '\0' };
    char* p = set;
    char* char_p = escape_char;

    while (*p != '\0') {
        if (*p == '\\') {
            while (*char_p != '\0') {         
                if (*(p + 1) == *char_p) {    
	            char ch;            
                      
                    switch (*char_p) {
                    case '\\':
                        ch = '\\';
                        break;
                    case 'a':
                        ch = '\a';
                        break;                        
                    case 'b':
                        ch = '\b';
                        break;
                    case 'f':
                        ch = '\f';
                        break;
                    case 'n':
                        ch = '\n';
                        break;
                    case 'r':
                        ch = '\r';
                        break;
                    case 't':
                        ch = '\t';
                        break;
                    case 'v':
                        ch = '\v';
                        break;
                    case '\'':
                        ch = '\'';
                        break;
                    case '\"':
                        ch = '\"';
                        break;
                    default:
                        ch = *p;
                        break;
                    }
                    
                    *p = ch;
                    my_strcat(p + 1, p + 2);
                    goto next_iteration;
                }

                ++char_p;
            }

            return ERROR_CODE_INVALID_FORMAT; 
        }

    next_iteration:
        ++p;   
    }
    
    return 0;        
}

int replace_range_set(char* set)
{
    char* p = set;
    int duplicate = FALSE;

    while (*p != '\0') {
        if (*p == '-') {     
            int diff;
            int i;
            size_t set_len = my_strlen(set);

            if (p == set || *(p + 1) == '\0') {
                ++p;
                continue;
            }
            
            if (duplicate) {
                duplicate = FALSE;
                ++p;
                continue;
            }

            if (*(p - 1) > *(p + 1)) {
                return ERROR_CODE_INVALID_RANGE;
            }
         
            if (*(p - 1) == *(p + 1)) {
                my_strcat(p, p + 2);
                ++p;
                continue;
            } 
  
            if (*(p + 2) == '-') {
                duplicate = TRUE;
            }

            diff = *(p + 1) - *(p - 1);
            
            if (set_len + diff - 2 > 511) {
                return ERROR_CODE_ARGUMENT_TOO_LONG;
            }
            
            if (diff > 1) {
                my_strncat_rev(set + set_len + diff - 2, set + set_len, diff - 2);
                
                for (i = 1; i < diff; ++i) {
                    *p = *(p - 1) + 1;
                    p++;
                }     
            } else {
                my_strcat(p, p + 1);
            }                            
        }
      
        ++p;
    }

    return 0;
}

void my_strncpy(char* dest, const char* src, size_t count)
{
    while (*src != '\0' || count-- != 0) {
        *dest++ = *src++;
    }
    
    if (count == 0 && *src != '\0') {
    } else {
        *dest = '\0';
    }        
}

void my_strcat(char* str1, const char* str2)
{
    char* str1_p = str1;
    const char* str2_p = str2;

    while (*str2_p != '\0') {
        *str1_p++ = *str2_p++;
    }
    
    *str1_p = '\0';
}

void my_strncat_rev(char* str1, const char* str2, size_t count)
{
    char* str1_p = str1;
    const char* str2_p = str2;

    while (count--) {
        *str1_p-- = *str2_p--;
    }
}


size_t my_strlen(const char* str)
{
    const char* str_ptr = str;
    
    while (*str_ptr++ != '\0') {
    }
  
    return str_ptr - str - 1;
}

int my_strcmp(const char* str1, const char* str2)
{
    const char* str1_p = str1;
    const char* str2_p = str2;

    while (*str1_p == *str2_p && *str1_p != '\0') {
        ++str1_p;
        ++str2_p;
    }
    
    return *str1_p - *str2_p;    
}

int my_is_alpha(char c) 
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return TRUE;
    } else {
        return FALSE;
    }
}
