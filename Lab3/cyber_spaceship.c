#include "cyber_spaceship.h"

#define TRUE (1)
#define FALSE (0)
#define NULL ((void*)0)

const char* get_longest_safe_zone_or_null(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count, size_t* out_longest_safe_area_length)
{
    const char* travel_ptr = cab_start_location;
    const char* longest_safe_zone_start_ptr = NULL;

    size_t bSafe = FALSE;
    size_t safe_area_length = 0;
    size_t longest_safe_area_length = 0;
    size_t overlapped_cluster_cnt = 0;

    /* pre-condition check */
    if (cab_length == 0) {
        *out_longest_safe_area_length = 0;
    
        return NULL;
    } 
  
    if (cluster_count == 0) {
        *out_longest_safe_area_length = cab_length;
   
        return travel_ptr;
    }

    while (travel_ptr < cab_start_location + cab_length) {
        size_t i;
        
        for (i = 0; i < cluster_count; ++i) {
            if (travel_ptr == cluster_start_locations[i]) {
                overlapped_cluster_cnt++;
            } else if (travel_ptr == cluster_start_locations[i] + cluster_lengths[i] - 1) {
                overlapped_cluster_cnt--;
            }
        }

        if (overlapped_cluster_cnt % 2 == 0) {     
            if (bSafe) { 
                safe_area_length++;     
            }

            bSafe = TRUE;       
        } else if (bSafe) {          

            if (safe_area_length >= longest_safe_area_length) {
                longest_safe_area_length = safe_area_length;
                longest_safe_zone_start_ptr = travel_ptr - longest_safe_area_length;
            }

            bSafe = FALSE;

            safe_area_length = 0;     
        }

        ++travel_ptr;
    }
  
    *out_longest_safe_area_length = longest_safe_area_length;

    return longest_safe_zone_start_ptr;
}

int get_travel_time(const char* const cab_start_location, const size_t cab_length, const char* const cluster_start_locations[], const size_t cluster_lengths[], const size_t cluster_count)
{
    const double SAFE_AREA_TRAVEL_SPEED = 1 / 6.0;
    const double UNSAFE_AREA_TRAVEL_SPEED = 1 / 12.0;

    const char* travel_ptr = cab_start_location;    
    size_t safe_area_length = 0;
    size_t unsafe_area_length = 0;
    size_t overlapped_cluster_cnt = 0;

    /* cab_length > 0 expected */
  
    if (cluster_count == 0) {
        return (int)(cab_length * SAFE_AREA_TRAVEL_SPEED + 0.5);
    }

    while (travel_ptr < cab_start_location + cab_length) {
        size_t i;
        
        for (i = 0; i < cluster_count; ++i) {
            if (travel_ptr == cluster_start_locations[i]) {
                overlapped_cluster_cnt++;
            } else if (travel_ptr == cluster_start_locations[i] + cluster_lengths[i] - 1) {
                overlapped_cluster_cnt--;
            }
        }

        if (overlapped_cluster_cnt % 2 == 0) {     
            safe_area_length++;
        } else {          
            unsafe_area_length++; 
        }

        ++travel_ptr;
    }
  
    return (int)(safe_area_length * SAFE_AREA_TRAVEL_SPEED + unsafe_area_length * UNSAFE_AREA_TRAVEL_SPEED + 0.5);
}

