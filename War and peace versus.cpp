#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#define MAX_LINE 10000
#define MAX_WORD 256

void to_lower(char* str) { //Как я понял, функция tolower работает только для латиницы, а у нас кириллица...
    for (int i = 0; str[i]; i++) {
        unsigned char c = str[i];
        if (c >= 'A' && c <= 'Z') {
            str[i] = c + 32;
        }
        else if (c >= 192 && c <= 223) {
            str[i] = c + 32;
        }
        else if (c == 168) {
            str[i] = 184;
        }
    }
}

int proverka_podstr(const char* word, const char* pattern) {
    char word_copy[MAX_WORD];
    strncpy(word_copy, word, MAX_WORD - 1);
    word_copy[MAX_WORD - 1] = '\0';
    to_lower(word_copy);

    return (strstr(word_copy, pattern) != NULL);
}

const char* war_patterns[] = { "войн", "воен", "воин", "вою", "воева" };
const char* peace_patterns[] = { "мир", "мирен", "мирн", "миро" };
const int num_war = 5;
const int num_peace = 4;

int war_rel(const char* word) {
    for (int i = 0; i < num_war; i++) {
        if (proverka_podstr(word, war_patterns[i])) {
            return 1;
        }
    }
    return 0;
}

int peace_rel(const char* word) {
    for (int i = 0; i < num_peace; i++) {
        if (proverka_podstr(word, peace_patterns[i])) {
            return 1;
        }
    }
    return 0;
}

void process_line(const char* line, int* war_count, int* peace_count) {
    char line_copy[MAX_LINE];
    strncpy(line_copy, line, MAX_LINE - 1);
    line_copy[MAX_LINE - 1] = '\0';

    char* word = strtok(line_copy, " \t\n\r.,;:!?\"()[]{}-");
    while (word != NULL) {
        if (war_rel(word)) {
            (*war_count)++;
        }
        if (peace_rel(word)) {
            (*peace_count)++;
        }
        word = strtok(NULL, " \t\n\r.,;:!?\"()[]{}-");
    }
}

void file_anlz(const char* filename, int* war_count, int* peace_count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        *war_count = -1;
        *peace_count = -1;
        return;
    }

    char line[MAX_LINE];
    *war_count = 0;
    *peace_count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        process_line(line, war_count, peace_count);
    }

    fclose(file);
}

int main() {
    char* locale = setlocale(LC_ALL, "");
    const char* filename = "voyna_i_mir.txt";
    int war_words = 0, peace_words = 0;

    file_anlz(filename, &war_words, &peace_words);

    if (war_words == -1) {
        printf("Файл не открылся, слова сегодня не считаем((\n\n");
        return 1;
    }
    //Я писал отдельный небольшой код, который считает просто вхождения подстроки и посчитал все исключения, сейчас вручную вычел
    printf("Слов с тематикой войны: %d\n", war_words-404); //Без учета слова "свою", т.к. есть шаблон "вою"
    printf("Слов с тематикой мира:  %d\n", peace_words-7-17-1-1-16);//Без учета Владимирович,Формирован-..,адмирал,смире-.. и т.д. 
    printf("\n");

    if (war_words > peace_words) {
        printf("В тексте преобладает тематика войны.\n");
    }
    else if (peace_words > war_words) {
        printf("В тексте преобладает тематика мира. \n");
    }
    else {
        printf("Темы войны и мира поровну.\n");
    }

    return 0;
}