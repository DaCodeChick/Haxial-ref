/**
 * @file random.c
 * @brief Random number generation implementation
 * 
 * Implements KDXServer.lexe random ID generation algorithm.
 */

#include "haxial/random.h"
#include "haxial/fnv1a.h"
#include <sys/time.h>
#include <pthread.h>

/* Random pool state (matches KDXServer.lexe globals) */
static uint8_t g_random_pool[256] = {0};  /* 256-byte random pool */
static uint32_t g_random_seed = 0;        /* LCG seed */
static uint8_t g_random_index = 0;        /* Current index in pool */
static pthread_mutex_t g_random_mutex = PTHREAD_MUTEX_INITIALIZER;

/* LCG constants from KDXServer.lexe */
#define LCG_MULTIPLIER 0x41c64e6d
#define LCG_INCREMENT  0x3039

uint32_t hx_get_microseconds(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint32_t)tv.tv_usec;
}

uint32_t hx_random(void) {
    uint32_t microseconds;
    uint32_t hash;
    uint32_t random_value;
    
    pthread_mutex_lock(&g_random_mutex);
    
    /* Get microseconds for entropy */
    microseconds = hx_get_microseconds();
    
    /* Hash the 256-byte pool with microseconds as seed */
    hash = hx_fnv1a_32(g_random_pool, 256, microseconds);
    
    /* Initialize LCG seed on first use */
    if (g_random_seed == 0) {
        g_random_seed = microseconds;
    }
    
    /* Update LCG seed */
    g_random_seed = g_random_seed * LCG_MULTIPLIER + LCG_INCREMENT;
    
    /* Mix hash with LCG */
    random_value = hash ^ g_random_seed;
    
    /* XOR random value back into pool at current index */
    g_random_pool[g_random_index] ^= random_value;
    
    /* Update index (keep it 0-63) */
    g_random_index = (uint8_t)random_value & 0x3f;
    
    pthread_mutex_unlock(&g_random_mutex);
    
    return random_value;
}

void hx_random_seed(uint32_t seed) {
    pthread_mutex_lock(&g_random_mutex);
    g_random_seed = seed;
    pthread_mutex_unlock(&g_random_mutex);
}
