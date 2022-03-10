#include <stdlib.h>
#include <limits.h>

#include "parentheses.h"

enum {
    PARENTHESES = -4,
    SQUARES,
    CURLIES,
    ANGLES
};

size_t get_matching_parentheses(parenthesis_t* out_parentheses, size_t max_size, const char* str)
{
    parenthesis_t** pa_parentheses = malloc(sizeof(parenthesis_t*) * max_size);
    parenthesis_t** pa_ptr = pa_parentheses;
    parenthesis_t** pa_end;
    size_t pa_cnt = 0;

    parenthesis_t* out_p;
    size_t out_cnt;

    const char* p = str;
    parenthesis_t* temp = NULL;

    while (*p != '\0') {
        switch (*p) {
        case '(':
            temp = malloc(sizeof(parenthesis_t));
            temp->opening_index = p - str;
            temp->closing_index = PARENTHESES;

            *pa_ptr++ = temp;

            ++pa_cnt;

            break;
        case ')':
            if (temp == NULL) {
                break;
            }

            pa_end = pa_parentheses + pa_cnt - 1;

            while (pa_end >= pa_parentheses) {
                if ((*pa_end)->closing_index == (size_t)PARENTHESES) {
                    (*pa_end)->closing_index = p - str;
                    break;
                }
                --pa_end;
            }

            break;
        case '[':
            temp = malloc(sizeof(parenthesis_t));
            temp->opening_index = p - str;
            temp->closing_index = SQUARES;

            *pa_ptr++ = temp;

            ++pa_cnt;

            break;
        case ']':
            if (temp == NULL) {
                break;
            }

            pa_end = pa_parentheses + pa_cnt - 1;

            while (pa_end >= pa_parentheses) {
                if ((*pa_end)->closing_index == (size_t)SQUARES) {
                    (*pa_end)->closing_index = p - str;
                    break;
                }
                --pa_end;
            }

            break;
        case '{':
            temp = malloc(sizeof(parenthesis_t));
            temp->opening_index = p - str;
            temp->closing_index = CURLIES;

            *pa_ptr++ = temp;

            ++pa_cnt;

            break;
        case '}':
            if (temp == NULL) {
                break;
            }

            pa_end = pa_parentheses + pa_cnt - 1;

            while (pa_end >= pa_parentheses) {
                if ((*pa_end)->closing_index == (size_t)CURLIES) {
                    (*pa_end)->closing_index = p - str;
                    break;
                }
                --pa_end;
            }

            break;
        case '<':
            temp = malloc(sizeof(parenthesis_t));
            temp->opening_index = p - str;
            temp->closing_index = ANGLES;

            *pa_ptr++ = temp;

            ++pa_cnt;

            break;
        case '>':
            if (temp == NULL) {
                break;
            }

            pa_end = pa_parentheses + pa_cnt - 1;

            while (pa_end >= pa_parentheses) {
                if ((*pa_end)->closing_index == (size_t)ANGLES) {
                    (*pa_end)->closing_index = p - str;
                    break;
                }
                --pa_end;
            }

            break;
        default:
            break;
        }

        if (pa_cnt == max_size) {
            parenthesis_t** temp_pp = realloc(pa_parentheses, sizeof(parenthesis_t*) * max_size * 2);
            pa_parentheses = temp_pp;
            pa_ptr = pa_parentheses + pa_cnt;
        }

        ++p;
    }

    pa_ptr = pa_parentheses;
    out_cnt = 0;
    out_p = out_parentheses;

    while (pa_ptr - pa_parentheses < (int)pa_cnt) {
        size_t val;
        temp = *pa_ptr;
        val = temp->closing_index;

        if (val != (size_t)ANGLES && val != (size_t)SQUARES && val != (size_t)PARENTHESES && val != (size_t)CURLIES && out_cnt < max_size) {
            out_p->opening_index = temp->opening_index;
            out_p->closing_index = val;

            ++out_p;
            ++out_cnt;
        }

        free(*pa_ptr++);
    }

    free(pa_parentheses);

    return out_cnt;
}
