#include <stdlib.h>
#include <stdio.h>

#include "document_analyzer.h"

enum {
    BUFFER_SIZE = 2048,
    WORDS_SIZE = 512,
    SENTENCES_SIZE = 64,
    PARAGRAPH_SIZE = 8
};

static char* s_pa_document;

static const char* pa_words;
static const char** pa_sentences;
static const char*** pa_paragraphs;
static const char**** pa_document;

size_t words_count = 0;
size_t sentences_count = 0;
size_t paragraphs_count = 0;

size_t words_multiplier = 2;

int load_document(const char* document)
{
    size_t length;

    s_pa_document = malloc(BUFFER_SIZE);
    
    FILE* stream = fopen(document, "r");
 
    if (stream == NULL) {
        fprintf(stderr, "%s: %s", "error while opening the file", document);
        return FALSE;
    }

    length = fread(s_pa_document, sizeof(char), BUFFER_SIZE, stream);

    while (length == BUFFER_SIZE) {
        size_t multiplier = 2;
        char* temp;
        char* p;

        temp = realloc(s_pa_document, BUFFER_SIZE * multiplier++);      
        s_pa_document = temp;
        p = s_pa_document + BUFFER_SIZE;

        length = fread(p, sizeof(char), BUFFER_SIZE, stream);     
    }    

    if (fclose(stream) != 0) {
        fprintf(stderr, "%s: %s", "error while closing the file", document);
        return FALSE;
    }

    return TRUE;
}

void dispose(void)
{
    free(s_pa_document);
}

void analyze_document(void)
{
    pa_words = malloc(sizeof(char*) * WORDS_SIZE);
    const char* words_p = pa_words;
    pa_sentences = malloc(sizeof(char*) * SENTENCES_SIZE);
    const char** sentences_p = pa_sentences;
    pa_paragraphs = malloc(sizeof(char*) * PARAGRAPH_SIZE);
    pa_document = malloc(sizeof(char*));

    char* pa_word;
    char** pa_sentence;
    char*** pa_paragraph;
    char**** pa_document;

    /* get paragraphs */
    const char* p = s_pa_document;
    const char* word_start = p;
    const char* sentence_start = p;
    const char* paragraph_start = p;

    while (*p != '\0') {
        if (*p == ' ' || *p == ',' || *p == '.' || *p == '!' || *p == '?') {
            if (p == word_start) {
                ++word_start; 
                continue;
            }

            pa_word = malloc(sizeof(char*));
            pa_word = word_start;
            words_p++ = pa_word;       
            ++words_count;     
            word_start = p + 1;     

            if (words_count == WORDS_SIZE) {
                const char* temp = realloc(pa_words, sizeof(char*) * words_count * 2);
                pa_words = temp;
                words_p = pa_words + words_count;
                sentence_start = pa_words;
            }       

            if (*p == '.' || *p == '!' || *p == '?') {
                pa_sentence = malloc(sizeof(char**));
                pa_sentence = &pa_words;
                sentences_p++ = pa_sentence;
                ++sentences_count;
 
                if (sentences_count == SENTENCES_SIZE) {
                    const char* temp = realloc(pa_sentences, sizeof(char**) * sentences_count * 2);
                    pa_sentences = temp;
                    sentences_p = pa_sentences + sentences_count;
                }     
            }
        } else if (*p == '\n') {
             if (p == paragraph_start) {
                 ++paragraph_start;
                 continue;
             }          
   
             pa_paragraph = malloc(sizeof(char***));
             pa_paragraph = &pa_words;
                sentences_p++ = pa_sentence;
                ++sentences_count;
          
        }
       
        ++p;
    }
}


