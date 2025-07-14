
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
 * @example
 * @code
 * char str[10] = "hello";
 * append(str, '!');  // str becomes "hello!"
 * @endcode
 */
void append(char *s, char c) {
    while (*s++);
    *(s - 1) = c;
    *s = '\0';
}
