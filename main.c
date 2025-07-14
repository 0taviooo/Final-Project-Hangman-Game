
/* Inclui as bibliotecas do C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

/* Inclui as bibliotecas personalizadas */
#include "include/noose.h"
#include "include/utils.h"
#include <stdio.h>

/* Define cores personalizadas para os textos do terminal */
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main(void) {
    /* Estabelece o locale para português do Brasil */
    setlocale(LC_ALL, "ptb");

    /* Limpa a tela de início */
    system("clear");

    /* Declaração e inicialização da forca */
    Noose noose = {
        "", /* Palavra*/
        "", /* Palavra a ser mostrada*/
        "", /* Frase da dica */ 
        "", /* Letras já utilizadas */
        6, /* Número de tentativas */
        0 /* Valor indica de o jogo está finalizado */
    };

    char keep_playing = '\0';
    int word_line_number = 0;
    char word_line_number_buffer[128] = "";
    char tentativa = '\0';

    int i = 0; /* Contador genérico para ser usado em laços for */

    FILE *words_file = fopen("include/palavras.txt", "r");
    FILE *tips_file = fopen("include/dicas.txt", "r");
    
    if (words_file == NULL || tips_file == NULL) {
        puts("Erro no acesso dos arquivos necessários!");
        exit(1);
    }

    while (keep_playing != 'N') {
        
        puts("Seja bem-vindo ao Jogo da Forca!\n");

        /* Pede para o usuário inserir um número de 1 a 100 para selecionar a palavra secreta */
        while (1) {
            printf("Selecione um número de 1 a 100 par iniciar o jogo: ");
            
            setbuf(stdin, NULL);
            fgets(word_line_number_buffer, sizeof(word_line_number_buffer), stdin);

            /* Imprime na tela uma mensagem de erro se o valor inserido está fora do intervalo */
            if (sscanf(word_line_number_buffer, "%d", &word_line_number) != 1 || (word_line_number < 1 || word_line_number > 100)) {
                puts("Valor inserido inválido. Tente novamente!");
                continue;
            }
            break;
        }

        /* Lê dos arquivos a palavra e a frase correspondente ao valor inserido */
        for (i = 1; i <= word_line_number; ++i) {
            if (fgets(noose.word, sizeof(noose.word), words_file) == NULL || fgets(noose.tip, sizeof(noose.tip), tips_file) == NULL) {
                puts("Erro na leitura dos arquivos necessários!");
                exit(1);
            }
        }

        noose.word[strlen(noose.word)-1] = '\0'; /* Tira o \n do fim da palavra */
        /* noose.frase[strlen(buffer_frase)-1] = '\0'; */

        /* Faz used_words preencher-se com '_' para cada letra da palavra */
        for (i = 0; i < strlen(noose.word); ++i) noose.showed_word[i] = '_';
        
        /* Enquanto as tentativas restantes forem maior que zero ou a flag da completude do jogo for falsa */
        while (1) {
            /* Limpa a tela a cada tentativa */
            system("clear");

            /* Checa se a palavra já foi completamente descoberta */
            update_showed_word(&noose);
            check_word(&noose);

            /* Imprime na tela a dica da palavra e o desenho da forca */
            printf("A dica de sua palavra é: %s", noose.tip);
            draw_noose(noose);

            /* Imrpime na tela as tentativas restantes e as letras já inseridas pelo usuário */
            printf("Tentativas restantes: %d\n", noose.attempts);
            printf("Letras usadas: \n");

            /* Imrpime na tela a palavra secreta de acordo com os acertos do usuário */
            show_word(&noose);

            if (noose.is_complete || noose.attempts < 1) break;

            /* Pede ao usuário que insira a próxima letra */
            printf("Insira a letra que desejar: ");
            setbuf(stdin, NULL);
            scanf(" %c", &tentativa);

            /* Atualiza a letra inserida para ela mesma em caixa alta */
            tentativa = toupper(tentativa);
            
            /* Anexa a letra inserida na string de letras já inseridas */
            append(noose.used_words, tentativa);

            /* Se a palavra secreta não contém a letra inserida, o usuário é informado e perde uma tentativa */
            if (strchr(noose.word, tentativa) == NULL) {
                printf("A letra %c não está na palavra!\n", tentativa);
                --noose.attempts;
            }
        }

        /* Imprime na tela mensagens de comemoração para vitória e consolação para derrota */
        if (noose.is_complete) {
            puts(ANSI_COLOR_GREEN "Meus parabéns! Você acertou a palavra!" ANSI_COLOR_RESET);
        }
        else {
            puts(ANSI_COLOR_RED "Não foi dessa vez!" ANSI_COLOR_RESET);
        }

        /* Pede para o usuário inserir se deseja continuar no jogo, a partir de um caractere */
        while (1) {
            printf("Deseja continuar jogando (S/N)? ");
            setbuf(stdin, NULL);
            scanf(" %c", &keep_playing);
            /* Imrpime na tela uma mensagem de erro se o valor inserido (atualizado para ele mesmo em caixa alta) não for 'S' (sim) ou 'N' (não) */
            if (toupper(keep_playing) == 'S') {
                fseek(words_file, 0, SEEK_SET);
                fseek(tips_file, 0, SEEK_SET);
                reset_noose(&noose, 6);
                getchar();
                break;
            }   
            else if (toupper(keep_playing) == 'N') {
                break;
            }
            else {
                printf("Valor inserido inválido. Tente novamente!\n");
                continue;
            }
        }
        puts("");
    }

    /* Fecha os arquivos lidos */
    fclose(words_file);
    fclose(tips_file);

    return 0;
}