#include <assert.h>
#include "macros.h"

int main(void)
{
    int i = -6;
    int arr[20];

    if (i == 0) 
        SET(arr, 0, 20, 0);
     else 
        SET(arr, 0, 1, 10000004);
    
    assert(arr[0] == 10000004);



    return 0;
}


