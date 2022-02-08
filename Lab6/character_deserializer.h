#ifndef CHARACTER_DESERIALIZER_H
#define CHARACTER_DESERIALIZER_H

#define NAME_LEN (50)
#define MINION_MAX (3)

typedef struct {
    char name[NAME_LEN];
    unsigned int health;
    unsigned int strength;
    unsigned int defence;
} minion_t;

typedef struct {
    unsigned int fire;
    unsigned int cold;
    unsigned int lightning;
} elemental_resistance_t;

typedef struct {
    char name[NAME_LEN];
    unsigned int level;
    unsigned int health;
    unsigned int mana;
    unsigned int strength;
    unsigned int dexterity;
    unsigned int intelligence;
    unsigned int armour;
    unsigned int evasion;
    unsigned int leadership;
    size_t minion_count;
    elemental_resistance_t elemental_resistance;
    minion_t minions[MINION_MAX];
} character_v3_t;

int get_character(const char* filename, character_v3_t* out_character);

void deserialize_version_one(char* char_stat, character_v3_t* out_character);

void deserialize_version_two(char* char_stat, character_v3_t* out_character);

void deserialize_version_three(char* char_stat, character_v3_t* out_character);

void my_strncpy(char* dest, const char* src, size_t count);

size_t my_strtok(char** dest, char* str, char delim);

size_t my_str_uint_tok(unsigned int* key, char* name, char* str, char delim);

#endif /* CHARACTER_DESERIALIZER_H */

