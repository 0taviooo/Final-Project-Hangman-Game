
#define NOOSE_H

#ifdef NOOSE_H

typedef struct {
    char word[32]; /* Palavra */
    char showed_word[32]; /* Palavra a ser mostrada */
    char tip[64]; /* Dica */
    char used_letters[26]; /* Letras já utilizadas */
    int attempts; /* Número de tentativas */
    int is_complete; /* Valor indica se o jogo está finalizado */
} Noose;

/**
 * @brief Draws the hangman's noose with body parts based on remaining attempts
 * 
 * @param noose Pointer to Noose structure containing the game state
 * 
 * @note The drawing is progressive - more body parts appear as attempts decrease:
 * 
 *       - Head appears at <= 5 attempts;
 * 
 *       - Torso and arms appear at <= 4, 3, 2 attempts respectively;
 * 
 *       - Legs appear at <= 1 and 0 attempts
 */
void draw_noose(Noose *noose) {
    /* Função imprime na tela a forca */
    puts("______");
    puts("|    |");
    printf("|    %c\n", noose->attempts <= 5 ? 'O' : ' '); /* Cabeça é desenhada a partir de 5 tentativas*/
    printf("|   %c%c%c\n",noose->attempts <= 4 ? '/' : ' ', noose->attempts <= 3 ? '|' : ' ', noose->attempts <= 2 ? '\\' : ' '); /* Braço direito, torso, braço esquerdo são desenhados, respectivamente, a partir de 4, 3, 2 tentativas */
    printf("|   %c %c\n", noose->attempts <= 1 ? '/' : ' ', noose->attempts <= 0 ? '\\' : ' '); /* Perna direita, perna esquerda são desenhadas, respectivamente, a partir de 1, 0 tentativas*/
    puts("|");
}

/**
 * @brief Resets the noose structure to initial state
 * 
 * @param noose Pointer to Noose structure to be reset
 * 
 * @param new_attempts New attempt count to initialize
 * 
 * Performs the following resets:
 * 
 *          - Sets attempts to specified value;
 * 
 *          - Marks game as incomplete (is_complete = 0);
 * 
 *          - Clears used_letters array with null bytes;
 * 
 *          - Clears showed_word display buffer
 */
void reset_noose(Noose *noose, int new_attempts) {
    /* Função reseta as variáveis da forca para valores vazios */

    noose->attempts = new_attempts; /* Atribui um novo número de tentativas à variável */
    noose->is_complete = 0;
    memset(noose->used_letters, '\0', sizeof(noose->used_letters)); /* Preenche as letras já utilizadas com '\0' */
    memset(noose->showed_word, '\0', sizeof(noose->showed_word)); /* Preenche a palavra a ser mostrada com '\0' */
}

/**
 * @brief Updates the displayed word by revealing correctly guessed letters
 * 
 * @param noose Pointer to Noose structure containing game state
 * 
 * Compares each letter in the secret word against used_letters:
 * 
 *          - When a match is found, reveals that position in showed_word;
 * 
 *          - Non-matched letters remain hidden (preserves previous null bytes)
 */
void update_showed_word(Noose *noose) {
    /* Função atualiza a palavra a ser mostrada */

    /* Contadore genéricos para serem usados em laços for */
    int i = 0;
    int j = 0;
    for (i = 0; i < strlen(noose->word); ++i) {
        for (j = 0; j < strlen(noose->used_letters); ++j) {
            /* Compara cada letra da palavra secreta com cada letra já utilizada */
            if (noose->word[i] == noose->used_letters[j]) {
                /* Caso a verificação seja verdadeira, a palavra a ser mostrada exibe a letra já descoberta */
                noose->showed_word[i] = noose->word[i];
                /* Sai do laço */
                break;
            }
        }
    }
}

/**
 * @brief Displays the current state of the word being guessed
 * 
 * @param noose Pointer to Noose structure containing showed_word
 * 
 * Prints each character of showed_word separated by spaces,
 * followed by a newline. Hidden letters appear as '_'
 */
void show_word(Noose *noose) {
    /* Função imrpime na tela a palavra a ser mostrada */

    /* Contador genérico para ser usado em laços for */
    int i = 0;
    for (i = 0; i < strlen(noose->word); ++i) {
        /* Imprime na tela caractere por caractere entre espaços */
        printf("%c ", noose->showed_word[i]);
    }

    /* Adiciona uma quebra de linha para melhor visibilidade */
    putchar('\n');
}

/**
 * @brief Displays all previously guessed letters
 * 
 * @param noose Pointer to Noose structure containing used_letters
 * 
 * Prints letters in comma-separated format (e.g., "A, B, C").
 * Omits trailing comma for the last letter.
 */
void show_used_letters(Noose *noose) {
    /* Função imprime na tela as letras já utilizadas */

    /* Contador genérico para ser usado em laços for */
    int i = 0;
    for (i = 0; i < strlen(noose->used_letters); ++i) {
        /* Imprime caractere por caractere das letras já utilizadas */
        printf("%c", noose->used_letters[i]);
        /* Se a letra não for a letra final das letras já utilizadas, adiciona uma vírgula entre os caracteres */
        if (!(i == strlen(noose->used_letters) - 1)) printf(", "); 
    }
}

/**
 * @brief Checks if the player has guessed the complete word
 * 
 * @param noose Pointer to Noose structure containing word and showed_word
 * @return int 1 if words match (win condition), 0 otherwise
 * 
 */
int check_word(Noose *noose) {
    /* Função verifica se a palavra a ser mostrada e a palavra secreta são iguais */
    return strcmp(noose->showed_word, noose->word) == 0;
}

#endif
