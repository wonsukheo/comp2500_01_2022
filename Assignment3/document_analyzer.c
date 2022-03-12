#include <stdlib.h>
#include <stdio.h>

#include "document_analyzer.h"

enum {
    BUFFER_SIZE = 2048,
    WORDS_SIZE = 512,
    SENTENCES_SIZE = 64,
    PARAGRAPH_SIZE = 8
};

const char* s_pa_document;

const char** pa_words;
const char*** pa_sentences;
const char**** pa_paragraphs;

size_t words_count = 0;
size_t sentences_count = 0;
size_t paragraphs_count = 0;

int load_document(const char* document)
{
    size_t length;

    s_pa_document = malloc(BUFFER_SIZE);
    
    FILE* stream = fopen(document, "rb");
 
    if (stream == NULL) {
        fprintf(stderr, "%s: %s", "error while opening the file", document);
        return FALSE;
    }

    length = fread(s_pa_document, sizeof(char), BUFFER_SIZE, stream);

    while (length == BUFFER_SIZE) {
        size_t multiplier = 2;
        char* temp;
        char* p;

        temp = realloc(s_pa_document, BUFFER_SIZE * multiplier);      
        s_pa_document = temp;
        p = s_pa_document + BUFFER_SIZE * (multiplier++ - 1);

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
    free(pa_wrods);
    free(pa_sentences);
    free(pa_paragraphs);
    free(pa_document);
}

void analyze_document(void)
{
    pa_words = malloc(sizeof(char*) * WORDS_SIZE);
    const char** words_p = pa_words;
   
    pa_sentences = malloc(sizeof(char*) * SENTENCES_SIZE);
    const char*** sentences_p = pa_sentences;

    pa_paragraphs = malloc(sizeof(char*) * PARAGRAPH_SIZE);
    const char**** paragraphs_p = pa_paragraphs;
    
    pa_document = malloc(sizeof(char*));

    char* pa_word;
    char** pa_sentence;
    char*** pa_paragraph;
    char**** pa_document;

    const char* p = s_pa_document;
    const char* word_start_p = p;
    const char** sentence_start_p = words_p;
    const char*** paragraph_start_p = sentences_p;

    while (*p != '\0') {
        if (*p == ' ' || *p == ',' || *p == '.' || *p == '!' || *p == '?') {
            if (p == word_start_p) {
                ++word_start_p;
                goto next;
            }

            *words_p++ = word_start_p;       
            word_start_p = p + 1;       

            if (*p == '.' || *p == '!' || *p == '?') {
                *sentences_p++ = sentence_start_p;
                sentence_start_p = words_p + 1;
            }
        } 
   
        if (*p == '\n') {
            if (*(p + 1) == '\n') {
                ++word_start_p;
                goto next;
            }
            
            ++words_start_p;
            *paragraphs_p++ = paragraph_start_p;
            paragraph_start_p = sentences_p + 1;
        }

    next:       
        ++p;
    }
}


