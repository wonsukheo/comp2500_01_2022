#ifndef MY_STRING_H
#define MY_STRING_H

void reverse(char* str);

int index_of(const char* str, const char* word);

void reverse_by_words(char* str);

char* tokenize(char* str_or_null, const char* delims);

char* reverse_tokenize(char* str_or_null, const char* delims);

size_t my_strlen(const char* str);

#endif /* MY_STRING_H */
