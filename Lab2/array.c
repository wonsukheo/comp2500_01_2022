#include <limits.h>

#include "array.h"


int get_index_of(const int numbers[], const size_t element_count, const int num) 
{
    size_t i;

    for (i = 0; i < element_count; ++i) {
        if (numbers[i] == num) {
            return i;
        }
    }
   
    return -1;
}

int get_last_index_of(const int numbers[], const size_t element_count, const int num) 
{
    size_t i;

    for (i = element_count - 1; i >= 0; --i) {
        if (numbers[i] == num) {
            return i;
        }
    }
   
    return -1;    
}

int get_max_index(const int numbers[], const size_t element_count) 
{
    int max;
    size_t max_index;
    size_t i;

    if (element_count == 0) {
        return -1;
    }

    max = numbers[0];
    max_index = 0;

    for (i = 0; i < element_count; ++i) {
        if (numbers[i] > max) {
            max = numbers[i];
            max_index = i;
        }
    }
   
    return max_index;    
}

int get_min_index(const int numbers[], const size_t element_count) 
{
    int min;
    size_t min_index;
    size_t i;

    if (element_count == 0) {
        return -1;
    }

    min = numbers[0];
    min_index = 0;

    for (i = 0; i < element_count; ++i) {
        if (numbers[i] < min) {
            min = numbers[i];
            min_index = i;
        }
    }
   
    return min_index;    
}

int is_all_positive(const int numbers[], const size_t element_count)
{
    size_t i;
    /*if "empty array" == element_count = 0, i shouldnt have build bot problem*/
    if (element_count == 0) {
        return -1;
    }

    for (i = 0; i < element_count; ++i) {
        if (numbers[i] <= 0) {
            return FALSE;
        }
    }

    return TRUE;
}

int has_even(const int numbers[], const size_t element_count)
{
    size_t i;
   
    for (i = 0; i < element_count; ++i) {
        if (numbers[i] % 2 == 0) {
            return TRUE;
        }
    }

    return FALSE;
}    

int insert(int numbers[], const size_t element_count, const int num, const size_t pos)
{
    size_t i;

    if (pos > element_count) {
        return FALSE;
    }

    for (i = element_count; i > pos; --i) {
        numbers[i] = numbers[i - 1];
    }
 
    numbers[pos] = num;

    return TRUE;
}

int remove_at(int numbers[], const size_t element_count, const size_t index)
{
    size_t i;

    if (index >= element_count) {
        return FALSE;
    }

    for (i = index; i < element_count - 1; ++i) {
        numbers[i] = numbers[i + 1];
    }

    numbers[element_count - 1] = INT_MIN;

    return TRUE;
}
