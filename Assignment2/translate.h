#ifndef TRANSLATE_H
#define TRANSLATE_H

#define TRUE (1)
#define FALSE (0)

typedef enum error_code {
    ERROR_CODE_WRONG_ARGUMENTS_NUMBER = 1,
    ERROR_CODE_INVALID_FLAG,
    ERROR_CODE_INVALID_FORMAT,
    ERROR_CODE_ARGUMENT_TOO_LONG,
    ERROR_CODE_INVALID_RANGE
} error_code_t;

int translate(int argc, const char** argv);

int replace_escape_char(char* set);

int replace_range_set(char* set);

void my_strcpy(char* dest, const char* src);

void my_strcat(char* str1, const char* str2);

void my_strcat_opp(char* str1, const char* str2, size_t count);

size_t my_strlen(const char* str);

int my_strcmp(const char* str1, const char* str2);

int my_is_alpha(char c);

#endif /* TRANSLATE_H */
