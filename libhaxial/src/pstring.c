/**
 * @file pstring.c
 * @brief Pascal string utilities implementation
 */

#include "haxial/pstring.h"
#include <string.h>
#include <stdint.h>

/**
 * Internal structure definition
 */
struct TPascalString {
    uint8_t length;
    char data[HX_PSTRING_MAX_LENGTH];
};

uint8_t hx_pstring_from_cstr(TPascalString *pstr, const char *cstr) {
    if (pstr == NULL) {
        return 0;
    }
    
    if (cstr == NULL) {
        pstr->length = 0;
        return 0;
    }
    
    size_t len = strlen(cstr);
    if (len > HX_PSTRING_MAX_LENGTH) {
        len = HX_PSTRING_MAX_LENGTH;
    }
    
    pstr->length = (uint8_t)len;
    memcpy(pstr->data, cstr, len);
    
    return pstr->length;
}

size_t hx_pstring_to_cstr(char *cstr, const TPascalString *pstr, size_t max_length) {
    if (cstr == NULL || max_length == 0) {
        return 0;
    }
    
    if (pstr == NULL || pstr->length == 0) {
        cstr[0] = '\0';
        return 0;
    }
    
    size_t copy_len = pstr->length;
    if (copy_len >= max_length) {
        copy_len = max_length - 1;  // Leave room for null terminator
    }
    
    memcpy(cstr, pstr->data, copy_len);
    cstr[copy_len] = '\0';
    
    return copy_len;
}

uint8_t hx_pstring_copy(TPascalString *dest, const TPascalString *src) {
    if (dest == NULL) {
        return 0;
    }
    
    if (src == NULL) {
        dest->length = 0;
        return 0;
    }
    
    dest->length = src->length;
    memcpy(dest->data, src->data, src->length);
    
    return dest->length;
}

uint8_t hx_pstring_copy_truncate(TPascalString *dest, const TPascalString *src, uint8_t max_length) {
    if (dest == NULL) {
        return 0;
    }
    
    if (src == NULL) {
        dest->length = 0;
        return 0;
    }
    
    uint8_t copy_len = src->length;
    if (copy_len > max_length) {
        copy_len = max_length;
    }
    
    dest->length = copy_len;
    memcpy(dest->data, src->data, copy_len);
    
    return dest->length;
}

uint8_t hx_pstring_to_buffer(void *buffer, const TPascalString *pstr, uint8_t max_length) {
    if (buffer == NULL) {
        return 0;
    }
    
    if (pstr == NULL || pstr->length == 0) {
        return 0;
    }
    
    uint8_t copy_len = pstr->length;
    if (copy_len > max_length) {
        copy_len = max_length;
    }
    
    memcpy(buffer, pstr->data, copy_len);
    
    return copy_len;
}

int hx_pstring_compare(const TPascalString *pstr1, const TPascalString *pstr2) {
    if (pstr1 == NULL && pstr2 == NULL) {
        return 0;
    }
    if (pstr1 == NULL) {
        return -1;
    }
    if (pstr2 == NULL) {
        return 1;
    }
    
    // Compare lengths first
    if (pstr1->length != pstr2->length) {
        return (int)pstr1->length - (int)pstr2->length;
    }
    
    // Same length, compare data
    return memcmp(pstr1->data, pstr2->data, pstr1->length);
}

uint8_t hx_pstring_length(const TPascalString *pstr) {
    return (pstr != NULL) ? pstr->length : 0;
}

const char* hx_pstring_data(const TPascalString *pstr) {
    return (pstr != NULL) ? pstr->data : NULL;
}

bool hx_pstring_is_empty(const TPascalString *pstr) {
    return (pstr == NULL) || (pstr->length == 0);
}

int hx_pstring_compare_cstr(const TPascalString *pstr, const char *cstr) {
    if (pstr == NULL && cstr == NULL) {
        return 0;
    }
    if (pstr == NULL) {
        return -1;
    }
    if (cstr == NULL) {
        return 1;
    }
    
    size_t cstr_len = strlen(cstr);
    size_t min_len = (pstr->length < cstr_len) ? pstr->length : cstr_len;
    
    int result = memcmp(pstr->data, cstr, min_len);
    if (result != 0) {
        return result;
    }
    
    // Prefixes match, compare lengths
    return (int)pstr->length - (int)cstr_len;
}
