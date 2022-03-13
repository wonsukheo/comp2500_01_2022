#include <stdlib.h>
#include <stdio.h>

#include "document_analyzer.h"

#pragma warning(disable : 4996)

enum {
    BUFFER_SIZE = 2048,
    WORDS_SIZE = 256,
    SENTENCES_SIZE = 32,
    PARAGRAPH_SIZE = 8,
    COUNT_SIZE = 32
};

const char* s_pa_document = NULL;

const char** s_pa_words;
const char*** s_pa_sentences;
const char**** s_pa_paragraphs;

const char** words_p;
const char*** sentences_p;
const char**** paragraphs_p;

size_t* pa_char_in_word;
size_t* pa_words_in_sentence;
size_t* pa_sentences_in_paragraph;

size_t* char_in_word_p;
size_t* words_in_sentence_p;
size_t* sentences_in_paragraph_p;

size_t doc_length = 0;
size_t g_words_count = 0;
size_t g_sentences_count = 0;
size_t g_paragraphs_count = 0;

int load_document(const char* document)
{
    size_t multiplier = 2;
    FILE* stream;

    stream = fopen(document, "r");

    if (stream == NULL) {
        fprintf(stderr, "%s: %s", "error while opening the file", document);
        return FALSE;
    }
  
    dispose();

    s_pa_document = malloc(BUFFER_SIZE);

    doc_length = fread((void*)s_pa_document, sizeof(char), BUFFER_SIZE, stream);
    
    s_pa_document = realloc((void*)s_pa_document, sizeof(char) * doc_length);

    if (doc_length == 0) {
        free(s_pa_document);
        s_pa_document = NULL;        
    }

    while (doc_length == BUFFER_SIZE) {
        const char* p;

        s_pa_document = realloc((void*)s_pa_document, BUFFER_SIZE * multiplier);

        p = s_pa_document + BUFFER_SIZE * (multiplier++ - 1);

        doc_length = fread((void*)p, sizeof(char), BUFFER_SIZE, stream);
    }  

    if (fclose(stream) != 0) {
        fprintf(stderr, "%s: %s", "error while closing the file", document);
        return FALSE;
    }

    if (s_pa_document != NULL) {
        analyze_document();
    }

    return TRUE;
}

void dispose(void)
{
    size_t paragraph_index = 0;
    size_t sentence_index = 0;
    size_t word_index = 0;

    paragraphs_p = s_pa_paragraphs;
    sentences_in_paragraph_p = pa_sentences_in_paragraph;
    words_in_sentence_p = pa_words_in_sentence;

    while (paragraph_index < g_paragraphs_count) {
        while (sentence_index < *sentences_in_paragraph_p) {
            while (word_index < *words_in_sentence_p) {
                free((void*)paragraphs_p[paragraph_index][sentence_index][word_index]);
                paragraphs_p[paragraph_index][sentence_index][word_index++] = NULL;
            }

            word_index = 0;
            ++words_in_sentence_p;

            free(paragraphs_p[paragraph_index][sentence_index]);
            paragraphs_p[paragraph_index][sentence_index++] = NULL;
        }

        free(paragraphs_p[paragraph_index]);
        paragraphs_p[paragraph_index++] = NULL;

        sentence_index = 0;

        ++sentences_in_paragraph_p;
    }

    free(s_pa_words);
    s_pa_words = NULL;

    free(s_pa_sentences);
    s_pa_sentences = NULL;

    free(s_pa_paragraphs);
    s_pa_paragraphs = NULL;

    free((void*)s_pa_document);
    s_pa_document = NULL;

    free(pa_char_in_word);
    pa_char_in_word = NULL;

    free(pa_words_in_sentence);
    pa_words_in_sentence = NULL;

    free(pa_sentences_in_paragraph);
    pa_words_in_sentence = NULL;

    g_words_count = 0;
    g_sentences_count = 0;
    g_paragraphs_count = 0;
}

void analyze_document(void)
{
    const char* p = s_pa_document;
    const char* word_start_p = s_pa_document;
    char* pa_word;
    size_t words_count = 0;
    size_t sentences_count = 0;

    s_pa_words = (const char**)malloc(sizeof(char*) * WORDS_SIZE);
    words_p = s_pa_words;

    s_pa_sentences = (const char***)malloc(sizeof(char**) * SENTENCES_SIZE);
    sentences_p = s_pa_sentences;

    s_pa_paragraphs = (const char****)malloc(sizeof(char***) * PARAGRAPH_SIZE);
    paragraphs_p = s_pa_paragraphs;

    pa_words_in_sentence = (size_t*)malloc(sizeof(size_t) * COUNT_SIZE);
    words_in_sentence_p = pa_words_in_sentence;

    pa_sentences_in_paragraph = (size_t*)malloc(sizeof(size_t) * COUNT_SIZE);
    sentences_in_paragraph_p = pa_sentences_in_paragraph;

    pa_char_in_word = (size_t*)malloc(sizeof(size_t) * WORDS_SIZE);
    char_in_word_p = pa_char_in_word;

    while (*p != '\0') {
        if (*p == ' ' || *p == ',' || *p == '.' || *p == '!' || *p == '?') {
            if (p == word_start_p) {
                ++word_start_p;
                goto next;
            }

            *char_in_word_p++ = p - word_start_p;
            pa_word = malloc(p - word_start_p + 1);
            memcpy(pa_word, word_start_p, p - word_start_p);
            pa_word[p - word_start_p] = '\0';

            *words_p++ = pa_word;
            word_start_p = p + 1;
            ++words_count;

            if (*p == '.' || *p == '!' || *p == '?') {
                const char** pa_temp = malloc(sizeof(char*) * words_count);
                memcpy(pa_temp, s_pa_words, sizeof(char*) * words_count);

                *sentences_p++ = pa_temp;

                ++sentences_count;

                *words_in_sentence_p++ = words_count;

                g_words_count += words_count;
                words_count = 0;

                words_p = s_pa_words;
            }
        } 

        if (*p == '\n' || (p + 1) - s_pa_document == (int)doc_length) {
            const char*** pa_temp;

            ++word_start_p;

            if (*(p - 1) == '\n') {
                goto next;
            }

            pa_temp = malloc(sizeof(char**) * sentences_count);
            memcpy(pa_temp, s_pa_sentences, sizeof(char**) * sentences_count);

            *paragraphs_p++ = pa_temp;

            ++g_paragraphs_count;

            *sentences_in_paragraph_p++ = sentences_count;

            g_sentences_count += sentences_count;
            sentences_count = 0;

            sentences_p = s_pa_sentences;
        }

    next:
        ++p;
    }

    pa_char_in_word = realloc(pa_char_in_word, sizeof(size_t) * g_words_count);
    pa_words_in_sentence = realloc(pa_words_in_sentence, sizeof(size_t) * g_sentences_count);
    pa_sentences_in_paragraph = realloc(pa_sentences_in_paragraph, sizeof(size_t) * g_paragraphs_count);
}

size_t get_total_word_count(void)
{
    return g_words_count;
}

size_t get_total_sentence_count(void)
{
    return g_sentences_count;
}

size_t get_total_paragraph_count(void)
{
    return g_paragraphs_count;
}

const char*** get_paragraph_or_null(const size_t paragraph_index)
{
    if (paragraph_index >= g_paragraphs_count || g_paragraphs_count == 0 || s_pa_document == NULL) {
        return NULL;
    }

    return *(s_pa_paragraphs + paragraph_index);
}

size_t get_paragraph_word_count(const char*** paragraph)
{
    size_t i = 0;
    size_t index = 0;
    size_t sentences_count;
    size_t words_count = 0;

    while (i < g_paragraphs_count) {
        if (*(s_pa_paragraphs + i) == paragraph) {
            break;
        }
        else {
            index += *(pa_sentences_in_paragraph + i);
        }

        ++i;
    }

    sentences_count = *(pa_sentences_in_paragraph + i);

    for (i = index; i < index + sentences_count; ++i) {
        words_count += *(pa_words_in_sentence + i);
    }

    return words_count;
}

size_t get_paragraph_sentence_count(const char*** paragraph)
{
    size_t i = 0;
    size_t sentences_count;

    while (i < g_paragraphs_count) {
        if (*(s_pa_paragraphs + i) == paragraph) {
            break;
        }

        ++i;
    }

    sentences_count = *(pa_sentences_in_paragraph + i);

    return sentences_count;
}

const char** get_sentence_or_null(const size_t paragraph_index, const size_t sentence_index)
{
    const char*** paragraph = get_paragraph_or_null(paragraph_index);

    if (paragraph == NULL) {
        return NULL;
    }

    if (sentence_index >= *(pa_sentences_in_paragraph + paragraph_index)) {
        return NULL;
    }

    return *(paragraph + sentence_index);
}

size_t get_sentence_word_count(const char** sentence)
{
    size_t paragraph_index = 0;
    size_t sentence_index = 0;
    size_t index = 0;

    while (paragraph_index <= g_paragraphs_count) {
        if (s_pa_paragraphs[paragraph_index][sentence_index] == sentence) {
            break;
        }

        ++sentence_index;

        if (sentence_index == pa_sentences_in_paragraph[paragraph_index]) {
            ++paragraph_index;
            sentence_index = 0;
        }
    }

    while (paragraph_index-- != 0) {
        index += pa_sentences_in_paragraph[paragraph_index];
    }

    return *(pa_words_in_sentence + index + sentence_index);
}

int print_as_tree(const char* filename)
{
    FILE* stream;
    size_t paragraph_index = 0;
    size_t sentence_index = 0;
    size_t word_index = 0;

    char* p = malloc(sizeof(char) * 2048);
    char* pp = p;

    paragraphs_p = s_pa_paragraphs;
    char_in_word_p = pa_char_in_word;
    words_in_sentence_p = pa_words_in_sentence;
    sentences_in_paragraph_p = pa_sentences_in_paragraph;

    if (s_pa_document == NULL) {
        return FALSE;
    }

    while (paragraph_index < g_paragraphs_count) {
        pp += sprintf(pp, "%s %u:\n", "Paragraph", paragraph_index);

        while (sentence_index < *sentences_in_paragraph_p) {
            pp += sprintf(pp, "    %s %u:\n", "Sentence", sentence_index);

            while (word_index < *words_in_sentence_p) {
                pp += sprintf(pp, "        ");
                memcpy(pp, s_pa_paragraphs[paragraph_index][sentence_index][word_index++], sizeof(char) * *char_in_word_p);
                pp += sizeof(char) * *char_in_word_p++;
                pp += sprintf(pp, "\n");
            }

            word_index = 0;

            ++words_in_sentence_p;
            ++sentence_index;
        }

        if (paragraph_index + 1 == g_paragraphs_count) {
            break;
        }

        pp += sprintf(pp, "\n");

        sentence_index = 0;

        ++sentences_in_paragraph_p;
        ++paragraph_index;
    }

    stream = fopen(filename, "w");

    if (stream == NULL) {
        fprintf(stderr, "%s: %s", "error while opening the file", filename);
        return FALSE;
    }

    fwrite(p, pp - p, sizeof(char), stream);

    if (fclose(stream) != 0) {
        fprintf(stderr, "%s: %s", "error while closing the file", filename);
        return FALSE;
    }

    free(p);
    p = NULL;

    return TRUE;
}
