
typedef struct {
    char word[32];
    char showed_word[32];
    char tip[64];
    char used_words[26];
    int attempts;
    int is_complete;
} Noose;

void draw_noose(Noose noose) {
    puts("______");
    puts("|    |");
    printf("|    %c\n", noose.attempts <= 5 ? 'O' : ' '); // Cabeça
    printf("|   %c%c%c\n",noose.attempts <= 4 ? '/' : ' ', noose.attempts <= 3 ? '|' : ' ', noose.attempts <= 2 ? '\\' : ' '); // Braço direito, tordo, braço esquerdo
    printf("|   %c %c\n", noose.attempts <= 1 ? '/' : ' ', noose.attempts <= 0 ? '\\' : ' '); // Perna direita, perna esquerda
    puts("|");
}

void reset_noose(Noose *noose, int new_attempts) {
    noose->attempts = new_attempts;
    noose->is_complete = 0;
    memset(noose->used_words, 0, sizeof(noose->used_words));
    memset(noose->showed_word, 0, sizeof(noose->showed_word));
}

void update_showed_word(Noose *noose) {
    for (int i = 0; i < strlen(noose->word); ++i) {
        for (int j = 0; j < sizeof(noose->used_words); ++j) {
            if (noose->word[i] == noose->used_words[j]) {
                noose->showed_word[i] = noose->word[i];
                break;
            }
        }
    }
}

void show_word(Noose *noose) {
    for (int i = 0; i < strlen(noose->word); ++i) {
        printf("%c ", noose->showed_word[i]);
    }
    puts("");
}

void check_word(Noose *noose) {
    if (strcmp(noose->showed_word, noose->word) == 0)
        noose->is_complete = 1;
}