#include <stdlib.h>

#include "parentheses.h"


size_t get_matching_parentheses(parenthesis_t* out_parentheses, size_t max_size, const char* str)
{
    size_t PARENTHESES = SIZE_MAX;   
    size_t SQUARES = SIZE_MAX - 1;
    size_t CURLIES = SIZE_MAX - 2;  
    size_t ANGLES = SIZE_MAX - 3;   

    parenthesis_t** pa_parentheses = malloc(sizeof(parenthesis_t*) * max_size);
    parenthesis_t** pa_ptr = pa_parentheses;
    size_t pa_cnt = 0;
    parenthesis_t* temp = NULL;
    parenthesis_t** temp_p = pa_parentheses;
    parenthesis_t* out_p = out_parentheses;
    size_t cnt = 0;

    const char* p = str;

    while (*p != '\0') {
        switch (*p) {
        case '(':
            if (pa_cnt == max_size) {
                parenthesis_t** temp_pp = realloc(pa_parentheses, sizeof(parenthesis_t*) * max_size * 2);
                pa_parentheses = temp_pp;
                pa_ptr = pa_parentheses + pa_cnt;
            }

            temp = malloc(sizeof(parenthesis_t));
            temp->opening_index = p - str;
            temp->closing_index = PARENTHESES;

            *pa_ptr = temp;
            ++pa_ptr;
            ++pa_cnt;
            break;

        case ')':
            if (temp == NULL) {
                break;
            }

            temp_p = pa_parentheses + pa_cnt - 1;

            while (temp_p >= pa_parentheses) {
                if ((*temp_p)->closing_index == PARENTHESES) {
                    (*temp_p)->closing_index = p - str;
                    break;
                }
                --temp_p;
            }
            break;
        case '[':
            if (pa_cnt == max_size) {
                parenthesis_t** temp_pp = realloc(pa_parentheses, sizeof(parenthesis_t*) * max_size * 2);
                pa_parentheses = temp_pp;
                pa_ptr = pa_parentheses + pa_cnt;
            }

            temp = malloc(sizeof(parenthesis_t));
            temp->opening_index = p - str;
            temp->closing_index = SQUARES;

            *pa_ptr = temp;
            ++pa_ptr;
            ++pa_cnt;
            break;

        case ']':
            if (temp == NULL) {
                break;
            }

            temp_p = pa_parentheses + pa_cnt - 1;

            while (temp_p >= pa_parentheses) {
                if ((*temp_p)->closing_index == SQUARES) {
                    (*temp_p)->closing_index = p - str;
                    break;
                }
                --temp_p;
            }
            break;
        case '{':
            if (pa_cnt == max_size) {
                parenthesis_t** temp_pp = realloc(pa_parentheses, sizeof(parenthesis_t*) * max_size * 2);
                pa_parentheses = temp_pp;
                pa_ptr = pa_parentheses + pa_cnt;
            }

            temp = malloc(sizeof(parenthesis_t));
            temp->opening_index = p - str;
            temp->closing_index = CURLIES;

            *pa_ptr = temp;
            ++pa_ptr;
            ++pa_cnt;
            break;

        case '}':
            if (temp == NULL) {
                break;
            }

            temp_p = pa_parentheses + pa_cnt - 1;

            while (temp_p >= pa_parentheses) {
                if ((*temp_p)->closing_index == CURLIES) {
                    (*temp_p)->closing_index = p - str;
                    break;
                }
                --temp_p;
            }
            break;
        case '<':
            if (pa_cnt == max_size) {
                parenthesis_t** temp_pp = realloc(pa_parentheses, sizeof(parenthesis_t*) * max_size * 2);
                pa_parentheses = temp_pp;
                pa_ptr = pa_parentheses + pa_cnt;
            }

            temp = malloc(sizeof(parenthesis_t));
            temp->opening_index = p - str;
            temp->closing_index = ANGLES;

            *pa_ptr = temp;
            ++pa_ptr;
            ++pa_cnt;
            break;

        case '>':
            if (temp == NULL) {
                break;
            }

            temp_p = pa_parentheses + pa_cnt - 1;

            while (temp_p >= pa_parentheses) {
                if ((*temp_p)->closing_index == ANGLES) {
                    (*temp_p)->closing_index = p - str;
                    break;
                }
                --temp_p;
            }
            break;

        default:
            break;
        }

        ++p;
    }

    pa_ptr = pa_parentheses;

    while (pa_ptr - pa_parentheses < (int)pa_cnt) {
        size_t val;
        temp = *pa_ptr;
        val = temp->closing_index;

        if (val != ANGLES && val != SQUARES && val != PARENTHESES && val != CURLIES && cnt < max_size) {
            out_p->opening_index = temp->opening_index;
            out_p->closing_index = val;
            ++out_p;
            ++cnt;
        }

        free(*pa_ptr);

        ++pa_ptr;
    }

    free(pa_parentheses);

    return cnt + 1;
}
