#include <stdio.h>
#include <assert.h>

#include "character_deserializer.h"

#define BUFFER_LEN (1024)
#define NAME_LEN (50)

int get_character(const char* filename, character_v3_t* out_character)
{
    char char_info[BUFFER_LEN];
    size_t read_count;
    size_t file_v = 0;

    FILE* stream = fopen(filename, "rb");

    if (!stream) {
        fprintf(stdout, "error while opening %s", filename);
        return -1;
    }

    read_count = fread(char_info, 1, BUFFER_LEN, stream);
    char_info[read_count] = '\0';

    if (char_info[3] == ':') {
        deserialize_version_one(char_info, out_character);
        file_v = 1;

    } else if (char_info[4] == ',') {
        deserialize_version_two(char_info, out_character);
        file_v = 2;

    } else {
        deserialize_version_three(char_info, out_character);
        file_v = 3;
    }

    if (fclose(stream) != 0) {
        fprintf(stdout, "error while closing %s", filename);
        return -1;
    }
 
    return file_v;
}

void deserialize_version_one(char* char_info, character_v3_t* out_character)
{
    const size_t STAT_COUNT = 8;
    const size_t count;
    char* p = char_info;
    char* start_p = char_info;

    char** stat_token[STAT_COUNT];
    char** stat_token_p = stat_token;
    unsigned int stat_uint_token[STAT_COUNT];
    unsigned int* stat_uint_token_p = stat_uint_token;

    while (*p != '\0') {
        if (*p == ':') {
            *p = ' ';
        } else if (*p == ',') {
            *p = '\0';

            sscanf(start_p, "%s%u", stat_token_p, stat_uint_token_p++);                

            start_p = p + 1;
        }
        
        ++p;
    }

    sscanf(start_p, "%s%u", stat_token_p, stat_uint_token_p++);
   
    stat_uint_token_p = stat_uint_token;
    stat_token_p = stat_token;
    count = STAT_COUNT;

    while (count-- == 0) {
        switch (**stat_token_p) {
        case 'm':
            out_character->mana = *stat_uint_token_p;
            break;
        case 'h':
            out_character->health = *stat_uint_token_p;
            break;
        case 's':
            out_character->strength = *stat_uint_token_p;
            break;
        case 'i':
            if (*(*stat_token_p + 1) == 'n') {
                out_character->intelligence = *stat_uint_token_p;
            } else {
                sprintf(temp, "%s%u", "player_", *stat_uint_token_p++);
                my_strcpy(out_character->name, temp);
            }
            break;
        case 'd':
            if (*(*stat_token_p + 2) == 'x') {
                out_character->dexterity = *stat_uint_token_p;
            } else {
                out_character->armour = *stat_uint_token_p;
            }
            break;
        case 'l':
            out_character->level = *stat_uint_token_p;
            break;
        default:
            break;
        }
        
        stat_uint_token_p++;
        stat_token++;
    }
    
    out_character->evasion = out_character->dexterity / 2;
    out_character->leadership = out_character->level / 10;
    out_character->minion_count = 0;
    out_character->elemental_resistance.fire = out_character->armour / 12;
    out_character->elemental_resistance.cold = out_character->armour / 12;
    out_character->elemental_resistance.lightning = out_character->armour / 12;
}


void deserialize_version_two(char* char_info, character_v3_t* out_character)
{
    char* char_info_p = char_info;
    char* stat_token[2];
    unsigned int stat_uint_token[10];

    char** stat_token_p = stat_token;
    unsigned int* stat_uint_token_p = stat_uint_token;

    char name[50];
    int token_count;

    token_count = my_strtok(stat_token_p, char_info_p, '\n');
    assert(token_count == 2);

    token_count = my_stat_token(stat_uint_token, name, stat_token_p[1], ',');
    assert(token_count == 10);

    my_strcpy(out_character->name, name);

    out_character->level = *stat_uint_token_p++;
    out_character->strength = *stat_uint_token_p++;
    out_character->dexterity = *stat_uint_token_p++;
    out_character->intelligence = *stat_uint_token_p++;
    out_character->armour = *stat_uint_token_p++;
    out_character->evasion = *stat_uint_token_p++;
    out_character->elemental_resistance.fire = *stat_uint_token_p++ / 3;
    out_character->elemental_resistance.cold = out_character->elemental_resistance.fire; 
    out_character->elemental_resistance.lightning = out_character->elemental_resistance.fire;
    out_character->health = *stat_uint_token_p++;
    out_character->mana = *stat_uint_token_p;
    out_character->leadership = out_character->level / 10;
    out_character->minion_count = 0;
}

void deserialize_version_three(char* char_info, character_v3_t* out_character)
{   
    char* char_info_p = char_info;

    char* stat_token[6];
    unsigned int stat_uint_token[13];
  
    char** stat_token_p = stat_token;
    unsigned int* stat_uint_token_p = stat_uint_token;

    char name[50];
    int token_count;
    int minion_count;

    token_count = my_strtok(stat_token_p, char_info_p, '\n');
    assert(token_count > 1 && token_count < 7);

    token_count = my_stat_token(stat_uint_token, name, stat_token_p[1], '|');
    assert(token_count == 14);

    my_strcpy(out_character->name, name);

    out_character->level = *stat_uint_token_p++;
    out_character->health = *stat_uint_token_p++;
    out_character->mana = *stat_uint_token_p++;
    out_character->strength = *stat_uint_token_p++;
    out_character->dexterity = *stat_uint_token_p++;
    out_character->intelligence = *stat_uint_token_p++;
    out_character->armour = *stat_uint_token_p++;
    out_character->evasion = *stat_uint_token_p++;
    out_character->elemental_resistance.fire = *stat_uint_token_p++;
    out_character->elemental_resistance.cold = *stat_uint_token_p++;
    out_character->elemental_resistance.lightning = *stat_uint_token_p++;
    out_character->leadership = *stat_uint_token_p++;
    out_character->minion_count = *stat_uint_token_p;

    minion_count = out_character->minion_count;

    while (minion_count > 0) {
        token_count = my_stat_token(stat_uint_token, name, stat_token_p[minion_count + 2], '|');
        assert(token_count == 4);

        stat_uint_token_p = stat_uint_token;

        my_strcpy(out_character->minions[minion_count - 1].name, name);
        out_character->minions[minion_count - 1].health = *stat_uint_token_p++;
        out_character->minions[minion_count - 1].strength = *stat_uint_token_p++;
        out_character->minions[minion_count - 1].defence = *stat_uint_token_p;

        --minion_count;
    }
}

int my_strcmp(char* str1, char* str2) 
{
    char* str1_p = str1;
    char* str2_p = str2;

    while (*str1_p != '\0' && *str1_p == *str2_p) {
        ++str1_p;
        ++str2_p;
    }

    return *str1_p - *str2_p;
}

void my_strcpy(char* dest, const char* src)
{
    char* dest_p = dest;
    const char* src_p = src;

    while (*src_p != '\0') {
        *dest_p++ = *src_p++;
    }
    
    *dest_p = '\0';

    return;
}

int my_strtok(char** dest, char* str, char delim) 
{
    char* str_p = str;
    char* tok_start = str;
    char** dest_p = dest;

    int token_count = 0;

    while (*str_p != '\0') {
        if (*str_p == delim) {
            *str_p = '\0';                   
            *dest_p++ = tok_start;
            tok_start = str_p + 1;
          
            ++token_count;
        }     

        ++str_p;
    }
    
    if (*tok_start != '\0') {
        *dest_p = tok_start;

        ++token_count;
    }
  
    return token_count;
} 

int my_stat_token(unsigned int* uint_dest, char* name_dest, char* str, char delim)
{
    char* p = str;
    unsigned int* dest_p = uint_dest;
    char* start_p = str;
    int token_count = 0;
    int name_flag = 1;
    
    while (*p != '\0') {
        if (*p == delim) {
            *p = '\0';
            
            if (name_flag) {
                size_t num_read;
                num_read = sscanf(start_p, "%.49s", name_dest);
                name_dest[num_read] = '\0';

                ++token_count;
                name_flag = 0;
            } else {
                sscanf(start_p, "%u", dest_p++);
                ++token_count;
            }
            
            start_p = p + 1;
        }

        ++p;
    }
    
    sscanf(start_p, "%u", dest_p);

    ++token_count;

    return token_count;
}
