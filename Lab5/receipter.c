#include <stdio.h>
#include <time.h>

#include "receipter.h"

static char s_restaurant_name[] = "Charles' Seafood";
static char s_header[51] = "--------------------------------------------------";
static char s_footer[51] = "==================================================";

static char s_itemlist[512];
static char* s_itemlist_p = s_itemlist;
static char s_message[78];
static char* s_message_p = s_message;

static size_t s_receipt_num = 0;
static size_t s_item_count = 0;
static double s_subtotal = 0.f;
static double s_tip = 0.f;

int add_item(const char* name, double price)
{
    if (s_item_count == 10) {
        return FALSE;
    } 

    s_itemlist_p += sprintf(s_itemlist_p, "%33.25s%17.2f\n", name, price);

    s_subtotal += (price > 999.99) ? 999.99 : price;
   
    ++s_item_count;

    return TRUE;
}

void set_tip(double tip)
{
    s_tip = (tip > 999.99) ? 999.99 : tip;    
}

void set_message(const char* message)
{
    int msg_len = (int)my_strlen(message);
    
    s_message_p += sprintf(s_message_p, "%.50s\n", message);

    if (msg_len > 50) {
        sprintf(s_message_p, "%.25s\n", message + 50);
    }    
}

char* print_total(char* receipt)
{
    char* p = receipt;
    double subtotal;
    double tax;
    double total;

    subtotal = (int)(s_subtotal * 100) / 100.f;

    p += sprintf(p, "%33s%17.2f\n", "Subtotal", subtotal);
    
    if (s_tip != 0.f) {
        p += sprintf(p, "%33s%17.2f\n", "Tip", s_tip);
    }
    
    tax = (int)(s_subtotal * 5) / 100.0f;

    p += sprintf(p, "%33s%17.2f\n", "Tax", tax);

    total = (int)((s_subtotal * 1.05 + s_tip) * 100) / 100.f;

    p += sprintf(p, "%33s%17.2f\n", "Total", total);

    return p;
}

char* print_header(char* receipt, time_t timestamp)
{
    char time[50]; 
    char* p = receipt;

    struct tm* local = gmtime(&timestamp);
    
    sprintf(time, "%d-%02d-%02d %02d:%02d:%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);

    p += sprintf(p, "%s\n%s\n%-45s%05d\n%s\n", s_restaurant_name, s_header, time, s_receipt_num, s_header);

    return p;     
}

char* print_footer(char* receipt)
{
    char* p = receipt;
    
    p += sprintf(p, "%s\n%50s", s_footer, "Tax#-51234");

    return p;
}

void reset_receipt(void)
{   
    s_item_count = 0;
    s_subtotal = 0.f;
    s_tip = 0.f;
    s_itemlist_p = s_itemlist;
    s_message_p = s_message;
    *s_message = '\0';
}
int print_receipt(const char* filename, time_t timestamp)
{
    char receipt[2048];
    char* p;
    FILE* stream;
  
    if (s_item_count == 0) {
        reset_receipt();
        return FALSE;
    }

    p = print_header(receipt, timestamp);

    p += sprintf(p, "%s\n", s_itemlist);

    p = print_total(p);
    
    p += sprintf(p, "\n%s", s_message);

    p = print_footer(p);

    *p = '\0';

    stream = fopen(filename, "wb");
    
    if (stream == NULL) {
        reset_receipt();
        return FALSE;
    }

    fprintf(stream, "%s", receipt);

    if (fclose(stream) == -1) {
        reset_receipt();
        return FALSE;
    }

    ++s_receipt_num;
    reset_receipt();

    return TRUE;
}

size_t my_strlen(const char* str) 
{
    const char* p = str;

    while (*p++ != '\0') {
    }

    return p - str - 1;
}
