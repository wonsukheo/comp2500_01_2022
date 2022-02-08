#include <assert.h>
#include <stdio.h>

#include "character_deserializer.h"

#define BUFFER_LEN (1024)

#define V_ONE_STAT (8)
#define V_TWO_STAT (10)
#define V_THREE_STAT (14)

int get_character(const char* filename, character_v3_t* out_character)
{
    char buffer[BUFFER_LEN];
    size_t num_read;

    FILE* stream;
    size_t file_version;

    stream = fopen(filename, "rb");
 
    if (!stream) {
        fprintf(stderr, "error while opening file %s", filename);
        return -1;
    }
  
    num_read = fread(buffer, 1, BUFFER_LEN, stream);
    buffer[num_read] = '\0';

    if (buffer[4] == ',') {
        file_version = 2;
        deserialize_version_two(buffer, out_character);
    } else if (buffer[4] == ' ') {
        file_version = 3;
        deserialize_version_three(buffer, out_character);
    } else {
        file_version = 1;
        deserialize_version_one(buffer, out_character);
    }

    if (fclose(stream) == -1) {
        fprintf(stderr, "error while closing file %s", filename);
        return -1;
    }
   
    return file_version;
}

void deserialize_version_one(char* char_stat, character_v3_t* out_character)
{
    char* key_arr[V_ONE_STAT];
    char** key_p = key_arr;

    unsigned int value_arr[V_ONE_STAT];
    unsigned int* value_p = value_arr;

    char* p = char_stat;
    char* start_p = char_stat;

    int key_count = 8;
  
    while (*p != '\0') {
        if (*p == ':') {
            *p = '\0';
            *key_p++ = start_p;
         
            start_p = p + 1;
        } else if (*p == ',') {
            *p = '\0';
            sscanf(start_p, "%u", value_p++);
            
            start_p = p + 1;
        }

        ++p;
    }
    
    sscanf(start_p, "%u", value_p);
    
    key_p = key_arr;
    value_p = value_arr;
  
    while (key_count-- > 0) {
        switch (**key_p) {
        case 'm':
            out_character->mana = *value_p;
            break;
        case 'h':
            out_character->health = *value_p;
            break;
        case 's':
            out_character->strength = *value_p;
            break;
        case 'i':
            if (*(*key_p + 1) == 'n') {
                out_character->intelligence = *value_p;
            } else {
                char name[NAME_LEN];
                sprintf(name, "%s%u", "player_", *value_p);
                my_strncpy(out_character->name, name, NAME_LEN);
            }
            break;
        case 'd':
            if (*(*key_p + 2) == 'x') {
                out_character->dexterity = *value_p;
            } else {
                out_character->armour = *value_p;
            }
            break;
        case 'l':
            out_character->level = *value_p;
            break;
        default:
            break;
        }

        ++value_p;
        ++key_p;
    }
    
    out_character->evasion = out_character->dexterity / 2;
    out_character->leadership = out_character->level / 10;
    out_character->minion_count = 0;
    out_character->elemental_resistance.fire = out_character->armour / 12;
    out_character->elemental_resistance.cold = out_character->armour / 12;
    out_character->elemental_resistance.lightning = out_character->armour / 12;
}

void deserialize_version_two(char* char_stat, character_v3_t* out_character)
{
    char* key_arr[2];
    char name[NAME_LEN];

    unsigned int value_arr[V_TWO_STAT - 1];
    unsigned int* value_p = value_arr;
    
    size_t token_count;

    token_count = my_strtok(key_arr, char_stat, '\n');
    assert(token_count == 2);

    token_count = my_str_uint_tok(value_p, name, key_arr[1], ',');
    assert(token_count == V_TWO_STAT);

    my_strncpy(out_character->name, name, NAME_LEN);

    out_character->level = *value_p++;
    out_character->strength = *value_p++;
    out_character->dexterity = *value_p++;
    out_character->intelligence = *value_p++;
    out_character->armour = *value_p++;
    out_character->evasion = *value_p++;
    out_character->elemental_resistance.fire = *value_p++ / 3;
    out_character->elemental_resistance.cold = out_character->elemental_resistance.fire; 
    out_character->elemental_resistance.lightning = out_character->elemental_resistance.fire;
    out_character->health = *value_p++;
    out_character->mana = *value_p;
    out_character->leadership = out_character->level / 10;
    out_character->minion_count = 0;        
}

void deserialize_version_three(char* char_stat, character_v3_t* out_character)
{
    char* key_arr[6];
    char name[NAME_LEN];

    unsigned int value_arr[V_THREE_STAT - 1];
    unsigned int* value_p = value_arr;
    
    size_t token_count;
    size_t minion_count;

    token_count = my_strtok(key_arr, char_stat, '\n');
    assert(token_count > 1 && token_count < 7);

    token_count = my_str_uint_tok(value_p, name, key_arr[1], '|');
    assert(token_count == V_THREE_STAT);

    my_strncpy(out_character->name, name, NAME_LEN);

    out_character->level = *value_p++;
    out_character->health = *value_p++;
    out_character->mana = *value_p++;
    out_character->strength = *value_p++;
    out_character->dexterity = *value_p++;
    out_character->intelligence = *value_p++;
    out_character->armour = *value_p++;
    out_character->evasion = *value_p++;
    out_character->elemental_resistance.fire = *value_p++;
    out_character->elemental_resistance.cold = *value_p++;
    out_character->elemental_resistance.lightning = *value_p++;
    out_character->leadership = *value_p++;
    out_character->minion_count = *value_p++;

    minion_count = out_character->minion_count;    
    value_p = value_arr;

    while (minion_count > 0) {
        token_count = my_str_uint_tok(value_p, name, key_arr[minion_count + 2], '|');
        assert(token_count == 4);

        my_strncpy(out_character->minions[minion_count - 1].name, name, NAME_LEN);

        out_character->minions[minion_count - 1].health = *value_p++;
        out_character->minions[minion_count - 1].strength = *value_p++;
        out_character->minions[minion_count - 1].defence = *value_p++;

        --minion_count;
        value_p = value_arr;
    }
}

void my_strncpy(char* dest, const char* src, size_t count)
{
    char* dest_p = dest;
    const char* src_p = src;
    
    while (*src_p != '\0' && *src_p != ' ' && count > 0) {
        *dest_p++ = *src_p++;
        --count;
    }
    
    *dest_p = '\0';
}

size_t my_strtok(char** dest, char* str, char delim) 
{
    char* p = str;
    char* start_p = str;
    char** dest_p = dest;

    size_t token_count = 0;

    while (*p != '\0') {
        if (*p == delim) {
            *p = '\0';                   
            *dest_p++ = start_p;
            
            start_p = p + 1;
          
            ++token_count;
        }     

        ++p;
    }
    
    if (*start_p != '\0') {
        *dest_p = start_p;

        ++token_count;
    }
  
    return token_count;
} 

size_t my_str_uint_tok(unsigned int* key, char* name, char* str, char delim)
{
    char* p = str;
    char* start_p = str;
    unsigned int* key_p = key;

    size_t name_flag = 1;
    size_t token_count = 0;

    while (*p != '\0') {
        if (*p == delim) {
            *p = '\0';

            if (name_flag) {
                my_strncpy(name, start_p, NAME_LEN);
                name_flag = 0;     
            } else {
                sscanf(start_p, "%u", key_p++);
            }
            
            ++token_count;
            start_p = p + 1;
        }
        ++p;
    }
    
    sscanf(start_p, "%u", key_p);
    
    return token_count + 1;
}

