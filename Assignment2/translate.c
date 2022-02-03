#include "translate.h"

#define BUFFER_LEN (512)

int translate(int argc, const char** argv)
{
    const char* set1;
    const char* set1_p;
    const char* set2;
    size_t set1_len;
    size_t set2_len;

    char set1_cpy[512];
    char set2_cpy[512];
    
    int flag_i = FALSE;
    int range_set_return;

    char c;

    if (argc != 3 && argc != 4) {
        return ERROR_CODE_WRONG_ARGUMENTS_NUMBER;
    }
    
    if (argc == 4 && my_strcmp(argv[1], "-i") != 0) {
        return ERROR_CODE_INVALID_FLAG;
    }
 
    if (argc == 4) { 
        set1 = argv[2];
        set2 = argv[3];

        flag_i = TRUE;
    } else {
        set1 = argv[1];
        set2 = argv[2];
    }

    set1_len = my_strlen(set1);
    set2_len = my_strlen(set2);    

    if (set1_len > 511 || set2_len > 511) {
        return ERROR_CODE_ARGUMENT_TOO_LONG;
    }    

    my_strcpy(set1_cpy, set1);
    my_strcpy(set2_cpy, set2);

    if (replace_escape_char(set1_cpy) == -1 || replace_escape_char(set2_cpy) == -1) {
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
  
    while (c != -1) {
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
    char escape_char[] = {'\\', 'a', 'b', 'f', 'n', 'r', 't', 'v', '\'', '\"', '\0'};
    char* p = set;
    char* char_p = escape_char;

    while (*p != '\0') {
        if (*p == '\\') {
            while (*char_p != '\0') {         
                if (*(p + 1) == *char_p) {    
	            char c;            
                      
                    switch (*char_p) {
                    case '\\':
                        c = '\\';
                        break;
                    case 'a':
                        c = '\a';
                        break;                        
                    case 'b':
                        c = '\b';
                        break;
                    case 'f':
                        c = '\f';
                        break;
                    case 'n':
                        c = '\n';
                        break;
                    case 'r':
                        c = '\r';
                        break;
                    case 't':
                        c = '\t';
                        break;
                    case 'v':
                        c = '\v';
                        break;
                    case '\'':
                        c = '\'';
                        break;
                    case '\"':
                        c = '\"';
                        break;
                    default:
                        break;
                    }
                    
                    *p = c;
                    my_strcat(p + 1, p + 2);
                    goto next_iteration;
                }

                ++char_p;
            }

            /* ERROR_CODE_INVALID_FORMAT */
            return -1; 
        }

        next_iteration:
        ++p;   
    }
    
    return 0;        
}

int replace_range_set(char* set)
{
    char* p = set;
    int set_len = (int)my_strlen(set);
    int duplicate = FALSE;

    while (*p != '\0') {
        if (*p == '-') {     
            int diff;
            size_t i;
       
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

            diff = *(p + 1) - *(p - 1);
            
            if (set_len + diff - 2 >= 511) {
                return ERROR_CODE_ARGUMENT_TOO_LONG;
            }
            
            if (diff > 1) {
                set_len = (int)my_strlen(set);

                my_strcat_opp(set + set_len + diff - 2, set + set_len, diff - 2);
                
                for (i = 1; i < diff; ++i) {
                    *p = *(p - 1) + 1;
                    p++;
                }     
            } else {
                my_strcat(p, p + 1);
            }                            
       
            duplicate = TRUE;
        }
      
        ++p;
    }

    return 0;
}

void my_strcpy(char* dest, const char* src)
{
    while (*src != '\0') {
        *dest++ = *src++;
    }
  
    *dest = '\0';    
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

void my_strcat_opp(char* str1, const char* str2, size_t count)
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
