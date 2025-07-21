
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

/**
 * @brief Counts the number of lines in a file
 * 
 * @param file Pointer to a FILE object that identifies the stream to be read
 * 
 * @return int The total number of lines in the file
 * 
 * @warning The file pointer must be valid and open for reading. This function
 *          will read through the entire file, moving the file position indicator
 *          to the end of the file. The cursor of the file i is rewinded after in the end.
 * 
 * @note This function counts lines by detecting newline characters ('\\n').
 *       The last line will be counted even if it doesn't end with a newline.
 *       Empty files will return 0.
 * 
 */
int count_lines(FILE *file) {
    /* Função retorna a quantidade de linhas de um arquivo de texto */

    /* Declaração de variáveis */
    char aux_char = '\0';
    int lines = 0;

    /* Enquanto a variável auxiliar de caractere percorrer a linha e não encontrar o fim do arquivo */
    while((aux_char = fgetc(file)) != EOF) {
        /* Se a variável auxiliar de caractere encontre uma quebra de linha */
        if(aux_char == '\n') {
            /* O contador de linhas é incrementado */
            ++lines;
        }
    }

    /* Volta o cursor do arquivo ao início */
    rewind(file);

    /* Retorna o contador de linhas */
    return lines;
}

#endif
