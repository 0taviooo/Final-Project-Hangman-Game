
/* Inclui as bibliotecas do C */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

/* Inclui as bibliotecas personalizadas */
#include "include/noose.h"
#include "include/utils.h"

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
        "", /* Dica */ 
        "", /* Letras já utilizadas */
        6, /* Número de tentativas */
        0 /* Valor indica se o jogo está finalizado */
    };

    char keep_playing = '\0'; /* Flag para parar o jogo */
    int word_line_number = 0; /* Número da linha dos arquivos a ser utilizado */
    char input_letter = '\0'; /* Letra inserida pelo usuário */
    
    char fgets_buffer[128] = ""; /* Buffer para leitura do fgets() */

    /* Flag de erro referente ao número da linha inserido */
    int not_valid_word_line_number = 0;

    /* Flag de erro referente ao valor da variável que armazena se o usuário deseja permanecer no jogo */
    int not_valid_keep_playing_value = 0;

    /* Flags referentes à letra inserida */
    int more_than_one_char_error = 0; /* Caso haja mais de 1 caractere */
    int is_not_letter_error = 0; /* Caso não seja uma letra */
    int already_used_letter_error = 0; /* Caso seja uma letra já utilizada */

    int i = 0; /* Contador genérico para ser usado em laços for */

    /* Abre os arquivos necessários */
    FILE *words_file = fopen("include/palavras.txt", "r");
    FILE *tips_file = fopen("include/dicas.txt", "r");
    
    /* Imprime na tela uma mensagem de erro se os arquivos não puderem ser abertos e fecha o programa */
    if (words_file == NULL || tips_file == NULL) {
        puts(ANSI_COLOR_YELLOW "Erro no acesso dos arquivos necessários!" ANSI_COLOR_RESET);
        exit(1);
    }

    /* Laço principal do jogo */
    while (keep_playing != 'N') {
        /* Pede para o usuário inserir um número de 1 a 100 para selecionar a palavra secreta */
        while (1) {
            /* Limpa a tela */
            system("clear");

            /* Imprime o título do jogo */
            puts("        _                         _         ______                  \n       | |                       | |       |  ____|                 \n       | | ___   __ _  ___     __| | __ _  | |__ ___  _ __ ___ __ _ \n   _   | |/ _ \\ / _` |/ _ \\   / _` |/ _` | |  __/ _ \\| '__/ __/ _` |\n  | |__| | (_) | (_| | (_) | | (_| | (_| | | | | (_) | | | (_| (_| |\n   \\____/ \\___/ \\__, |\\___/   \\__,_|\\__,_| |_|  \\___/|_|  \\___\\__,_|\n                 __/ |                                          \n                |___/                                               \n");

            /* Imprime na tela uma mensagem de erro caso o usuário tenha inserido um valor inválido */
            if (not_valid_word_line_number) puts(ANSI_COLOR_YELLOW "Valor inserido inválido. Tente novamente!" ANSI_COLOR_RESET);

            printf("Selecione um número de 1 a 100 para iniciar o jogo: ");
            
            /* Lê o número da linha */
            setbuf(stdin, NULL);
            fgets(fgets_buffer, sizeof(fgets_buffer), stdin);

            /* Verifica se o valor inserido é válido */
            if (
                strchr(fgets_buffer, ' ') != NULL || /* O valor contém espaços */
                strchr(fgets_buffer, ',') != NULL || /* O valor contém vírgula */
                strchr(fgets_buffer, '.') != NULL || /* O valor contém ponto */
                sscanf(fgets_buffer, "%d", &word_line_number) != 1|| /* Se ocorre erro na atribuição à variável */
                (word_line_number < 1 || word_line_number > 100) /* Se a variável está no intervalo correto */
            ) {
                /* Registra o erro e repete o laço */
                not_valid_word_line_number = 1;
                continue;
            }
            /* Sai do laço */
            not_valid_word_line_number = 0;
            break;
        }

        /* Lê dos arquivos a palavra e a dica correspondente ao número da linha inserido */
        for (i = 1; i <= word_line_number; ++i) {
            /* Imrpime na tela uma mensagem de erro se os arquivos não puderem ser lidos e fecha o programa */
            if (fgets(noose.word, sizeof(noose.word), words_file) == NULL || fgets(noose.tip, sizeof(noose.tip), tips_file) == NULL) {
                puts(ANSI_COLOR_YELLOW "Erro na leitura dos arquivos necessários!" ANSI_COLOR_RESET);
                exit(1);
            }
        }
        
        /* Tira o \n do fim da palavra */
        noose.word[strlen(noose.word)-1] = '\0';

        /* Faz a palavra a ser mostrada preencher-se com '_' para cada letra da palavra */
        for (i = 0; i < strlen(noose.word); ++i) {
            if (noose.word[i] != '-') noose.showed_word[i] = '_';
            else noose.showed_word[i] = '-'; /* Desconsidera hífens */
        }
        
        while (1) {
            /* Limpa a tela a cada tentativa */
            system("clear");

            update_showed_word(&noose); /* Atualiza a palavra a ser mostrada */
            noose.is_complete = check_word(&noose); /* Checa se a palavra foi totalmente descoberta */

            /* Imprime na tela a dica da palavra e o desenho da forca */
            printf("A dica de sua palavra é: %s", noose.tip);
            draw_noose(&noose);

            /* Imrpime na tela as tentativas restantes e as letras já inseridas pelo usuário */
            printf("Tentativas restantes: %s%d%s\n",
                /* Atualiza as cores das tentativas de acordo com a quantidade de tentativas restantes (6-5 verde, 3-4 amarelo, 2-0 vermelho) */
                noose.attempts >= 5 ? ANSI_COLOR_GREEN : noose.attempts >= 3 ? ANSI_COLOR_YELLOW : ANSI_COLOR_RED,
                noose.attempts,
                ANSI_COLOR_RESET
            );
            printf("Letras usadas: ", noose.used_letters);
            show_used_letters(&noose);
            putchar('\n');

            /* Imrpime na tela a palavra a ser mostrada de acordo com os acertos do usuário */
            show_word(&noose);

            /* Imprime na tela mensagens de erro caso o usuário tenha inserido um valor inválido */
            if (is_not_letter_error) puts(ANSI_COLOR_YELLOW "O caractere inserido não é uma letra! Tente novamente." ANSI_COLOR_RESET);
            if (already_used_letter_error) puts(ANSI_COLOR_YELLOW "Essa letra já foi inserida! Tente novamente." ANSI_COLOR_RESET);
            if (more_than_one_char_error) puts(ANSI_COLOR_YELLOW "Insira apenas um caractere! Tente novamente." ANSI_COLOR_RESET);
            
            /* Se a palavra está completa ou as tentativas foram zeradas, sai do laço */
            if (noose.is_complete || noose.attempts < 1) break;
            
            /* Pede ao usuário que insira a próxima letra */
            printf("Insira a letra que desejar: ");

            /* Lê a letra inserida */
            setbuf(stdin, NULL);
            fgets(fgets_buffer, sizeof(fgets_buffer), stdin);

            /* Verifica se o valor inserido é válido */
            if (
                strlen(fgets_buffer) > 2 /* O valor possui mais de 2 caracteres (Obs.: o valor 2 é por conta do caractere em si e o caractere '\n')*/
                || sscanf(fgets_buffer, "%c", &input_letter) != 1 /* Se ocorre erro na atribuição da variável */
            ) {
                /* Registra o erro e repete o laço */
                more_than_one_char_error = 1;
                /* Corrige os outros erros para não imprimir na tela múltiplos erros */
                is_not_letter_error = 0;
                already_used_letter_error = 0;
                continue;
            }
            else {
                /* Corrige o erro */
                more_than_one_char_error = 0;
            }

            /* Atualiza a letra inserida para ela mesma em caixa alta */
            input_letter = toupper(input_letter);

            /* Verifica se o caractere inserido é, de fato, uma letra */
            if (!isalpha(input_letter)) {
                /* Registra o erro e repete o laço */
                is_not_letter_error = 1;
                /* Corrige os outros erros para não imprimir na tela múltiplos erros */
                more_than_one_char_error = 0;
                already_used_letter_error = 0;
                continue;
            }
            else {
                /* Corrige o erro */
                is_not_letter_error = 0;
            }

            /* Verifica se a letra inserida já foi atualizada */
            if (strchr(noose.used_letters, input_letter) != NULL) {
                /* Registra o erro e repete o laço */
                already_used_letter_error = 1;
                /* Corrige os outros erros para não imprimir na tela múltiplos erros */
                more_than_one_char_error = 0;
                is_not_letter_error = 0;
                continue;
            }
            else {
                /* Corrige o erro */
                already_used_letter_error = 0;
            }

            /* Anexa a letra inserida no final da string de letras já inseridas */
            append(noose.used_letters, input_letter);

            /* Se a palavra secreta não contém a letra inserida, o usuário é informado e perde uma tentativa */
            if (strchr(noose.word, input_letter) == NULL) {
                printf("A letra %c não está na palavra!\n", input_letter);
                --noose.attempts;
            }
        }

        /* Imprime na tela mensagens de comemoração para vitória e consolação para derrota */
        if (noose.is_complete) {
            puts(ANSI_COLOR_GREEN "Meus parabéns! Você acertou a palavra!" ANSI_COLOR_RESET);
        }
        else {
            puts(ANSI_COLOR_RED "Não foi dessa vez!" ANSI_COLOR_RESET);
            printf("A palavra secreta era %s\n", noose.word);
        }

        /* Pede para o usuário inserir se deseja continuar no jogo, a partir de um caractere */
        while (1) {
            /* Imprime na tela uma mensagem de erro caso o usuário tenha inserido um valor inválido */
            if (not_valid_keep_playing_value) {
                /* Apaga a linha superior para melhorar a visibilidade */
                printf("\x1b[1F"); /* Move o cursor para o começo da linha superior */
                printf("\x1b[2K"); /* Limpa essa linha */
                puts(ANSI_COLOR_YELLOW "Valor inserido inválido. Tente novamente!" ANSI_COLOR_RESET);
            }
            printf("Deseja continuar jogando (S/N)? ");

            /* Lê a escolha do usuário */
            setbuf(stdin, NULL);
            fgets(fgets_buffer, sizeof(fgets_buffer), stdin);

            /* Verifica se o valor inserido é válido */
            if (
                strlen(fgets_buffer) > 2 /* O valor possui mais de 2 caracteres (Obs.: o valor 2 é por conta do caractere em si e o caractere '\n')*/
                || sscanf(fgets_buffer, "%c", &keep_playing) != 1 /* Se ocorre erro na atribuição da variável */
            ) {
                /* Registra o erro e repete o laço */
                not_valid_keep_playing_value = 1;
                continue;
            }

            /* Atualiza a letra inserida para ela mesma em caixa alta */
            keep_playing = toupper(keep_playing);

            /* Imrpime na tela uma mensagem de erro se o valor inserido não for 'S' (sim) ou 'N' (não) */
            if (keep_playing == 'S') {
                /* Move o cursor do arquivo palavras.txt para o início */
                fseek(words_file, 0, SEEK_SET);
                /* Move o cursor do arquivo dicas.txt para o início */
                fseek(tips_file, 0, SEEK_SET);
                /* Reseta as variáveis da forca para valores vazios */
                reset_noose(&noose, 6); /* Atribui 6 às tentativas */

                /* Limpa a tela */
                system("clear");
                /* Corrige o erro */
                not_valid_keep_playing_value = 0;
                break;
            }   
            else if (keep_playing == 'N') {
                /* Sai desse laço e do laço superior, pois o usuário selecionou o valor que sai do laço enquanto */
                break;
            }
            else {
                /* Registra o erro e repete o laço */
                not_valid_keep_playing_value = 1;
                continue;
            }
        }
        /* Adiciona uma quebra de linha para melhor visibilidade */
        putchar('\n');
    }

    /* Fecha os arquivos lidos */
    fclose(words_file);
    fclose(tips_file);

    return 0;
}
