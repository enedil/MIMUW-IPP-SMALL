#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "history_manager.h"
#include "utils.h"


void history_init(struct history* hist)
{
    hist->cls = NULL;
    for (size_t i = 0; i < QUANTUM_STATE_COUNT; ++i) {
        hist->next[i] = 0;
    }
}

void history_declare(struct history* hist, const char* prefix)
{
    if (*prefix == '\0') {
        return;
    }
    int first_digit = char_to_int(prefix[0]);
    if (hist->next[first_digit] == NULL) {
        hist->next[first_digit] = calloc(sizeof (struct history), 1);
        if (hist->next[first_digit] == NULL) {
            panic();
        }
    }
    history_declare(hist->next[first_digit], prefix + 1);
}

void history_remove(struct history* hist, const char* prefix)
{
    if (*prefix == '\0') {
        for (size_t i = 0; i < QUANTUM_STATE_COUNT; ++i) {
            if (hist->next[i] != NULL) {
                history_remove(hist->next[i], prefix);
                free(hist->next[i]);
                hist->next[i] = NULL;
            }
        }
        return;
    }
    int first_digit = char_to_int(prefix[0]);
    if (hist->next[first_digit] != NULL) {
        history_remove(hist->next[first_digit], prefix + 1);
    }
     
    if (prefix[1] == '\0') {
        if (hist->next[first_digit]) {
            free(hist->next[first_digit]);
            hist->next[first_digit] = NULL;
        }
    }

}

bool history_valid(const struct history* hist, const char* hist_string)
{
    if (hist_string[0] == '\0') {
        return true;
    }
    
    int first_digit = char_to_int(hist_string[0]);
    if (hist->next[first_digit] == NULL) {
        return false;
    }
    return history_valid(hist->next[first_digit], hist_string + 1);
}

uint64_t energy1(const struct history* hist, const char* hist_string)
{
    // 0 means error
    return (uint64_t)hist + (uint64_t)hist_string;

}
