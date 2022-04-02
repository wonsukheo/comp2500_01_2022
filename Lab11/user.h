#ifndef USER_H
#define USER_H

#define LENGTH (51)

typedef struct {
    size_t id;
    char username[LENGTH];
    char email[LENGTH];
    char password[LENGTH];
} user_t;

#endif /* USER_H */
