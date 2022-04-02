#include <string.h>
#include <stdio.h>

#include "data_store.h"

#define LOG_FILE "log.txt"

user_t* get_user_by_id_or_null(user_t** users_or_null, size_t id)
{
    user_t** user = users_or_null;

    while (*user != NULL) {
        if ((*user)->id == id) {
            return *user;
        }

        ++user;
    }    
 
    return NULL;
}

user_t* get_user_by_username_or_null(user_t** users_or_null, const char* username)
{
    user_t** user = users_or_null;

    while (*user != NULL) {
        if (strncmp((*user)->username, username, 50) == 0) {
            return *user;
        }

        ++user;
    }    
 
    return NULL;
}

bool update_email(user_t** users_or_null, size_t id, const char* email)
{
    user_t** user = users_or_null;

    while ((*user) != NULL) {
        if ((*user)->id == id) {
            char old_email[LENGTH];
            char new_email[LENGTH];
  
            strcpy(old_email, (*user)->email);
            strcpy(new_email, email);

            memcpy((*user)->email, email, LENGTH);
       
            #if defined (RELEASE)
            
            mask_info(old_email, '@');
            mask_info(new_email, '@');

            #endif /* RELEASE */
           
            {
                FILE* stream = fopen(LOG_FILE, "a+");
    
                if (stream == NULL) {
                    fprintf(stderr, "%s: %s", "error while opening file:", LOG_FILE);
                    return false;
                }

                char log[150];
                char* p = log;
            
                p += sprintf(log, "%s %d %s \"%s\" %s \"%s\"\n", "TRACE: User", id, "updated email from", old_email, "to", new_email);
                *p = '\0';              
    
                fwrite(log, sizeof(char), strlen(log), stream);

                if (fclose(stream) != 0) {
                    fprintf(stderr, "%s: %s", "error while closing file:", LOG_FILE);
                    return false;
                }
            }

            return true;
        }

        ++user;
    }    
 
    return false;
}

bool update_password(user_t** users_or_null, size_t id, const char* password)
{
    user_t** user = users_or_null;

    while ((*user) != NULL) {
        if ((*user)->id == id) {
            char old_password[LENGTH];
            char new_password[LENGTH];
  
            strcpy(old_password, (*user)->password);
            strcpy(new_password, password);

            memcpy((*user)->password, password, LENGTH);
       
            #if defined (RELEASE)
            
            mask_info(old_password, '\0');
            mask_info(new_password, '\0');       

            #endif /* RELEASE */       
 
            {
                FILE* stream = fopen(LOG_FILE, "a+");
    
                if (stream == NULL) {
                    fprintf(stderr, "%s: %s", "error while opening file:", LOG_FILE);
                    return false;
                }

                char log[150];
                char* p = log;
            
                p += sprintf(log, "%s %d %s \"%s\" %s \"%s\"\n", "TRACE: User", id, "updated password from", old_password, "to", new_password);
                *p = '\0';              
    
                fwrite(log, sizeof(char), strlen(log), stream);

                if (fclose(stream) != 0) {
                    fprintf(stderr, "%s: %s", "error while closing file:", LOG_FILE);
                    return false;
                }
            }

            return true;
        }

        ++user;
    }    
 
    return false;
}

static void mask_info(char* info, char c) 
{
    char* p = info;

    while (*p++ != c) {
    }
    
    size_t len = p - info - 1;
    p = info;

    if (len == 1) {
        *p = '*';

        return;
    } else if (len == 2) {
        *++p = '*';
        
        return;
    }

    p = info + 1;
    size_t i = 0;

    while (i++ < len - 2) {
        *p++ = '*';        
    }          
}
