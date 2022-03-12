#include <stdlib.h>
#include <stdio.h>

#include "document_analyzer.h"

enum {
    BUFFER_SIZE = 2048,
    WORDS_SIZE = 512,
    SENTENCES_SIZE = 64,
    PARAGRAPH_SIZE = 8
};

const char* pa_document;

const char** pa_words;
const char*** pa_sentences;
const char**** pa_paragraphs;

size_t words_count = 0;
size_t sentences_count = 0;
size_t paragraphs_count = 0;

int load_document(const char* document)
{
    size_t length;

    FILE* stream = fopen(document, "rb");

    pa_document = malloc(BUFFER_SIZE);
 
    if (stream == NULL) {
        fprintf(stderr, "%s: %s", "error while opening the file", document);
        return FALSE;
    }

    length = fread((void*)pa_document, sizeof(char), BUFFER_SIZE, stream);

    while (length == BUFFER_SIZE) {
        size_t multiplier = 2;
        char* temp;
        const char* p;

        temp = realloc((void*)pa_document, BUFFER_SIZE * multiplier);      
        pa_document = temp;
        p = pa_document + BUFFER_SIZE * (multiplier++ - 1);

        length = fread((void*)p, sizeof(char), BUFFER_SIZE, stream);     
    }    

    if (fclose(stream) != 0) {
        fprintf(stderr, "%s: %s", "error while closing the file", document);
        return FALSE;
    }

    return TRUE;
}

void dispose(void)
{
    free((void*)pa_document);
    free((void*)pa_words);
    free((void*)pa_sentences);
    free((void*)pa_paragraphs);
    free((void*)pa_document);
}

void analyze_document(void)
{
    const char** words_p = pa_words;
    const char*** sentences_p = pa_sentences;
    const char**** paragraphs_p = pa_paragraphs;

    const char* p = pa_document;
    const char* word_start_p = p;
    const char** sentence_start_p = words_p;
    const char*** paragraph_start_p = sentences_p;

    pa_words = malloc(sizeof(char*) * WORDS_SIZE);
  
    pa_sentences = malloc(sizeof(char*) * SENTENCES_SIZE);

    pa_paragraphs = malloc(sizeof(char*) * PARAGRAPH_SIZE);
    
    pa_document = malloc(sizeof(char*));

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
            
            ++word_start_p;
            *paragraphs_p++ = paragraph_start_p;
            paragraph_start_p = sentences_p + 1;
        }

    next:       
        ++p;
    }

    printf("%d, %d, %d", words_count, sentences_count, paragraphs_count);
}


