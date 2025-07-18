
#define UTILS_H

#ifdef UTILS_H

/**
 * @brief Appends a character to the end of a string
 * 
 * @param s Null-terminated string to which the character will be appended
 * @param c Character to append to the string
 * 
 * @warning The string buffer must have enough space for the additional character
 *          and the null terminator. This function does not perform bounds checking.
 * 
 * @note This function modifies the original string in place.
 * 
 */
void append(char *s, char c) {
    /* Função anexa um caractere no final de uma string */
    while (*s++);
    *(s - 1) = c;
    *s = '\0';
}

#endif
