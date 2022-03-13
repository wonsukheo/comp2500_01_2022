#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "document_analyzer.h"

#pragma warning(disable : 4996)

static size_t s_words_count = 0;
static size_t s_sentences_count = 0;
static size_t s_paragraphs_count = 0;

const char* s_pa_document;
static char**** s_pa_document_p = NULL;

int load_document(const char* document)
{
    FILE* stream;
    size_t doc_length = 0;
    size_t words_count = 0;
    size_t sentences_count = 0;
    size_t paragraphs_count = 0;

    char** temp_words;   
    char** temp_sentences;
    char** temp_paragraphs;

    size_t i;
    size_t j;
    size_t k;

    size_t word_length;

    stream = fopen(document, "r");

    if (stream == NULL) {
        fprintf(stderr, "%s", "error while closing the file");
        return FALSE;
    }

    dispose();
 
    fseek(stream, 0, SEEK_END);
    doc_length = ftell(stream);

    s_pa_document = malloc(doc_length + 1);
    memset((void*)s_pa_document, 0, doc_length + 1);

    fseek(stream, 0, SEEK_SET);
    fread((void*)s_pa_document, doc_length, sizeof(char), stream);

    if (doc_length ==0) {
        goto exit;
    }

    temp_paragraphs = tokenize_malloc(s_pa_document, "\n", &paragraphs_count);
    s_paragraphs_count += paragraphs_count;
    s_pa_document_p = malloc((paragraphs_count + 1) * sizeof(char***));
    s_pa_document_p[paragraphs_count] = NULL;

    for (i = 0; i < paragraphs_count; ++i) {
        temp_sentences = tokenize_malloc(temp_paragraphs[i], ".!?", &sentences_count);
        s_sentences_count += sentences_count;
        s_pa_document_p[i] = malloc(sizeof(char**) * (sentences_count + 1));
        s_pa_document_p[i][sentences_count] = NULL;

        for (j = 0; j < sentences_count; ++j) {
            temp_words = tokenize_malloc(temp_sentences[j], " ,", &words_count);
            s_words_count += words_count;
            s_pa_document_p[i][j] = malloc(sizeof(char*) * (words_count + 1));
            s_pa_document_p[i][j][words_count] = NULL;
           
            for (k = 0; k < words_count; ++k) {
                word_length = strlen(temp_words[k]);
                s_pa_document_p[i][j][k] = malloc(sizeof(char) * (word_length + 1));
                strcpy(s_pa_document_p[i][j][k], temp_words[k]);
            }
          
            for (k = 0; k < words_count; ++k) {
                free(temp_words[k]);
                temp_words[k] = NULL;
            }
          
            free(temp_words); 
            temp_words = NULL;
        }
        
        for (j = 0; j < sentences_count; ++j) {
            free(temp_sentences[j]);
            temp_sentences[j] = NULL;  
        }
        
        free(temp_sentences);
        temp_sentences = NULL;
    }    

    for (i = 0; i < paragraphs_count; ++i) {
        free(temp_paragraphs[i]);
        temp_paragraphs[i] = NULL;
    }

    free(temp_paragraphs[i]);
    temp_paragraphs = NULL;

exit:
    if (fclose(stream) != 0) {
        fprintf(stderr, "%s", "error while closing the file");
        return FALSE;    
    }
    
    free((void*)s_pa_document);
    s_pa_document = NULL;
    
    return TRUE;  
}

char** tokenize_malloc(const char* str, const char* delim, size_t* out_count)
{
    char* pa_str;
    char* token;
    size_t str_length = strlen(str);
    size_t word_count = 0;
    size_t container_length = 5;
    size_t word_length = 0;
    char** pa_container;
    void* temp = NULL;

    pa_str = malloc(sizeof(char) * (str_length) + 1);
    strcpy(pa_str, str);
   
    pa_container = malloc(sizeof(char*) * container_length);

    token = strtok(pa_str, delim);

    if (token == NULL) {
        temp = realloc(pa_container, sizeof(char) * 1);
        pa_container[0] = NULL;
        goto free_and_exit;
    }

    while (token != NULL) {
        ++word_count;
        word_length = strlen(token);

        if (word_count == container_length - 1) {
            container_length += 5;
            temp = realloc(pa_container, sizeof(char*) * container_length);
  
            if (temp != NULL) {
                pa_container = temp;
            }
        }
      
        pa_container[word_count - 1] = malloc(sizeof(char) * (word_length + 1));
        strcpy(pa_container[word_count - 1], token);

        token = strtok(NULL, delim);
    }

    pa_container[word_count] = NULL;
    *out_count = word_count;

free_and_exit:
    free(pa_str);
    pa_str = NULL;
    return pa_container;
}

void dispose(void) 
{
    size_t i;
    size_t j;
    size_t k;

    if (s_pa_document_p == NULL) {
        return;
    }

    for (i = 0; s_pa_document_p[i] != NULL; ++i) {
        for (j = 0; s_pa_document_p[i][j] != NULL; ++j) {
            for (k = 0; s_pa_document_p[i][j][k] != NULL; ++k) {
                free(s_pa_document_p[i][j][k]);
                s_pa_document_p[i][j][k] = NULL;
            }

            free(s_pa_document_p[i][j]);
            s_pa_document_p[i][j] = NULL;
        }

        free(s_pa_document_p[i]);
        s_pa_document_p[i] = NULL;
    }
    
    free(s_pa_document_p);
    s_pa_document_p = NULL;

    s_words_count = 0;
    s_sentences_count = 0;
    s_paragraphs_count = 0;
}

size_t get_total_word_count(void)
{
    return s_words_count;
}

size_t get_total_sentence_count(void)
{
    return s_sentences_count;
}

size_t get_total_paragraph_count(void)
{
    return s_paragraphs_count;
}

const char*** get_paragraph_or_null(const size_t paragraph_index)
{
    if (paragraph_index > s_paragraphs_count - 1 || s_pa_document_p == NULL) {
        return NULL;
    }

    return (const char***)s_pa_document_p[paragraph_index];
}

size_t get_paragraph_word_count(const char*** paragraph)
{
    size_t word_count = 0;
    size_t sentence_index;
    size_t word_index;

    if (paragraph == NULL) {
        goto exit;
    }

    for (sentence_index = 0; paragraph[sentence_index] != NULL; ++sentence_index) {
        for (word_index = 0; paragraph[sentence_index][word_index] != NULL; ++word_index) {
            word_count++;
        }
    }
exit:
    return word_count;
}

size_t get_paragraph_sentence_count(const char*** paragraph)
{
    size_t sentence_count = 0;
    size_t sentence_index;
    
    if (paragraph == NULL) {
        goto exit;
    }
    
    for (sentence_index = 0; paragraph[sentence_index] != NULL; ++sentence_index) {
        sentence_count++;
    }

exit:
    return sentence_count;
}

const char** get_sentence_or_null(const size_t paragraph_index, const size_t sentence_index)
{
    const char*** paragraph = NULL;
    size_t sentence_count = 0;    
    paragraph = get_paragraph_or_null(paragraph_index);

    if (paragraph == NULL) {
        return NULL;
    }

    sentence_count = get_paragraph_sentence_count(paragraph);

    if (sentence_index > sentence_count - 1) {
        return NULL;
    }
    
    return (const char**)s_pa_document_p[paragraph_index][sentence_index];
}

size_t get_sentence_word_count(const char** sentence)
{
    size_t word_count = 0;
    size_t word_index;

    if (s_pa_document_p == NULL) {
        goto exit;
    }    
    
    for (word_index = 0; sentence[word_index] != NULL; ++word_index) {
        word_count++;
    }

exit:
    return word_count; 
}

int print_as_tree(const char* filename)
{
    FILE* stream;
    size_t paragraph_index = 0;
    size_t sentence_index = 0;
    size_t word_index = 0;

    if (s_pa_document_p == NULL || s_words_count == 0) {
        return FALSE;
    }

    stream = fopen(filename, "wb");
    
    if (stream == NULL) {
        fprintf(stderr, "%s", "error while opening the file");
        return FALSE;
    }

    for (paragraph_index = 0; paragraph_index < s_paragraphs_count; ++paragraph_index) {
        if (paragraph_index != 0) {
            fprintf(stream, "\n\n");   
        }

        fprintf(stream, "Paragraph %u:", paragraph_index);
    
        for (sentence_index = 0; s_pa_document_p[paragraph_index][sentence_index] != NULL; ++sentence_index) {
            fprintf(stream, "\n    Sentence %u:", sentence_index);
        
            for (word_index = 0; s_pa_document_p[paragraph_index][sentence_index][word_index] != NULL; ++word_index) {
                fprintf(stream, "\n        %s", s_pa_document_p[paragraph_index][sentence_index][word_index]);
            }
        }
    }

    if (fclose(stream) != 0) {
        fprintf(stderr, "%s", "error while closing the file");
        return FALSE;
    }
    
    return TRUE;
}
