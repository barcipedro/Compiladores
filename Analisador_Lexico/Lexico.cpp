#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Defina as palavras-chave
#define AND "and"
#define BREAK "break"
#define DO "do"
#define ELSE "else"
#define ELSEIF "elseif"
#define END "end"
#define FALSE "false"
#define FOR "for"
#define FUNCTION "function"
#define IF "if"
#define IN "in"
#define LOCAL "local"
#define NIL "nil"
#define NOT "not"
#define OR "or"
#define REPEAT "repeat"
#define RETURN "return"
#define THEN "then"
#define TRUE "true"
#define UNTIL "until"
#define WHILE "while"

// Definições de tokens
#define RELOP 259
#define ID 260
#define NUM 261
#define STRING 262
#define ERRO 263
#define EOF_TOKEN 264

// Tamanho máximo para lexemas
#define MAX_LEXEMA 100

// Tamanho máximo para strings literais
#define MAX_STRING_LENGTH 1000

// Tabela de palavras-chave
char *keywords[] = {
    "and", "break", "do", "else", "elseif", "end", "false", "for", "function",
    "if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "true", "until", "while", NULL
};

// Estrutura para representar um token
struct Token {
    int nome_token;
    int atributo;
    char lexema[MAX_LEXEMA];
};

int estado = 0;
int partida = 0;
int cont_sim_lido = 0;
char *code;

int falhar() {
    switch (estado) {
        case 0: partida = 9; break;
        case 9: partida = 12; break;
        case 12: partida = 20; break;
        case 20: partida = 25; break;
        case 25: return -1; // Erro encontrado no código
        default:
            printf("Erro do compilador");
    }
    return partida;
}

struct Token proximo_token() {
    struct Token token;
    char c;
    char lexema[MAX_LEXEMA]; // Para armazenar temporariamente o lexema

    while (code[cont_sim_lido] != '\0') {
        switch (estado) {
            case 0:
                c = code[cont_sim_lido];
                if ((c == ' ') || (c == '\n')) {
                    estado = 0;
                    cont_sim_lido++;
                }
                else if (c == '<') estado = 1;
                else if (c == '=') estado = 5;
                else if (c == '>') estado = 6;
                else if (isalpha(c) || c == '_') estado = 10; // Identificador
                else if (isdigit(c)) estado = 13; // Número
                else if (c == '"') estado = 30; // String Literal
                else if (c == '/') estado = 20; // Comentário
                else {
                    estado = falhar();
                }
                break;

            case 1:
                cont_sim_lido++;
                c = code[cont_sim_lido];
                if ((c == ' ') || (c == '\n')) {
                    cont_sim_lido++;
                    c = code[cont_sim_lido];
                }
                if (c == '=') estado = 2;
                else if (c == '>') estado = 3;
                else estado = 4;
                break;

            case 2:
                cont_sim_lido++;
                token.nome_token = RELOP;
                token.atributo = 0;
                estado = 0;
                return token;
                break;

            case 3:
                cont_sim_lido++;
                token.nome_token = RELOP;
                token.atributo = 1;
                estado = 0;
                return token;
                break;

            case 4:
                token.nome_token = RELOP;
                token.atributo = 2;
                estado = 0;
                return token;
                break;

            case 5:
                cont_sim_lido++;
                token.nome_token = RELOP;
                token.atributo = 3;
                estado = 0;
                return token;
                break;

            case 6:
                cont_sim_lido++;
                c = code[cont_sim_lido];
                if ((c == ' ') || (c == '\n')) {
                    cont_sim_lido++;
                    c = code[cont_sim_lido];
                }
                if (c == '=') estado = 7;
                else estado = 8;
                break;

            case 7:
                cont_sim_lido++;
                token.nome_token = RELOP;
                token.atributo = 4;
                estado = 0;
                return token;
                break;

            case 8:
                token.nome_token = RELOP;
                token.atributo = 5;
                estado = 0;
                return token;
                break;

            case 9:
                c = code[cont_sim_lido];
                if ((c == ' ') || (c == '\n')) {
                    estado = 0;
                    cont_sim_lido++;
                }
                else {
                    estado = falhar();
                }
                break;

            case 10: // Reconhecimento de Identificador
                token.nome_token = ID;
                token.atributo = 0; // Você pode definir um atributo para identificadores, se necessário
                while (isalnum(code[cont_sim_lido]) || code[cont_sim_lido] == '_') {
                    lexema[strlen(lexema)] = code[cont_sim_lido]; // Adicione o caractere ao lexema
                    cont_sim_lido++;
                }

                // Verificar se é uma palavra-chave
                lexema[strlen(lexema)] = '\0'; // Encerre o lexema
                if (isKeyword(lexema)) {
                    if (strcmp(lexema, AND) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 0;
                    } else if (strcmp(lexema, BREAK) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 1;
                    } else if (strcmp(lexema, DO) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 2;
                    } else if (strcmp(lexema, ELSE) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 3;
                    } else if (strcmp(lexema, ELSEIF) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 4;
                    } else if (strcmp(lexema, END) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 5;
                    } else if (strcmp(lexema, FALSE) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 6;
                    } else if (strcmp(lexema, FOR) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 7;
                    } else if (strcmp(lexema, FUNCTION) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 8;
                    } else if (strcmp(lexema, IF) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 9;
                    } else if (strcmp(lexema, IN) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 10;
                    } else if (strcmp(lexema, LOCAL) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 11;
                    } else if (strcmp(lexema, NIL) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 12;
                    } else if (strcmp(lexema, NOT) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 13;
                    } else if (strcmp(lexema, OR) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 14;
                    } else if (strcmp(lexema, REPEAT) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 15;
                    } else if (strcmp(lexema, RETURN) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 16;
                    } else if (strcmp(lexema, THEN) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 17;
                    } else if (strcmp(lexema, TRUE) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 18;
                    } else if (strcmp(lexema, UNTIL) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 19;
                    } else if (strcmp(lexema, WHILE) == 0) {
                        token.nome_token = RELOP;
                        token.atributo = 20;
                    }
                }
                estado = 0; // Retornar ao estado inicial
                strcpy(token.lexema, lexema); // Copiar o lexema para a estrutura do token
                return token;
                break;

            case 11: // Reconhecimento de Delimitadores
                c = code[cont_sim_lido];
                if (c == '(') {
                    token.nome_token = '(';
                } else if (c == ')') {
                    token.nome_token = ')';
                } else if (c == '{') {
                    token.nome_token = '{';
                } else if (c == '}') {
                    token.nome_token = '}';
                } else if (c == ';') {
                    token.nome_token = ';';
                }
                cont_sim_lido++; // Avançar para o próximo caractere
                estado = 0; // Retornar ao estado inicial
                strcpy(token.lexema, lexema); // Copiar o lexema para a estrutura do token
                return token;
                break;

            case 12:
                c = code[cont_sim_lido];
                if ((c == ' ') || (c == '\n')) {
                    estado = 0;
                    cont_sim_lido++;
                }
                else {
                    estado = falhar();
                }
                break;

            case 13: // Reconhecimento de Número
                token.nome_token = NUM;
                token.atributo = 0; // Você pode definir um atributo para números, se necessário
                while (isdigit(code[cont_sim_lido])) {
                    lexema[strlen(lexema)] = code[cont_sim_lido]; // Adicione o caractere ao lexema
                    cont_sim_lido++;
                }
                estado = 0; // Retornar ao estado inicial
                strcpy(token.lexema, lexema); // Copiar o lexema para a estrutura do token
                return token;
                break;

            case 20: // Comentário
                cont_sim_lido++; // Avançar para o próximo caractere após a barra '/'
                if (code[cont_sim_lido] == '/') {
                    // Comentário de uma linha, continue lendo até o final da linha
                    while (code[cont_sim_lido] != '\n' && code[cont_sim_lido] != '\0') {
                        cont_sim_lido++;
                    }
                }
                else if (code[cont_sim_lido] == '*') {
                    // Comentário de várias linhas, continue lendo até encontrar "*/"
                    cont_sim_lido++; // Avance para o próximo caractere após o asterisco '*'
                    while (1) {
                        if (code[cont_sim_lido] == '\0') {
                            // Erro: comentário de várias linhas não foi fechado
                            estado = falhar();
                            break;
                        }
                        if (code[cont_sim_lido] == '*' && code[cont_sim_lido + 1] == '/') {
                            // Fim do comentário de várias linhas
                            cont_sim_lido += 2; // Avance para além de "*/"
                            estado = 0; // Retornar ao estado inicial
                            break;
                        }
                        cont_sim_lido++;
                    }
                }
                break;

            case 25:
                c = code[cont_sim_lido];
                if ((c == ' ') || (c == '\n')) {
                    estado = 0;
                    cont_sim_lido++;
                }
                else {
                    estado = falhar();
                    token.nome_token = ERRO;
                    token.atributo = 0;
                    strcpy(token.lexema, lexema); // Copiar o lexema para a estrutura do token
                    return token;
                }
                break;

            case 30: // String Literal
                token.nome_token = STRING;
                token.atributo = 0; // Você pode definir um atributo para strings, se necessário
                cont_sim_lido++; // Avançar para o próximo caractere após a primeira aspa
                while (code[cont_sim_lido] != '"' && code[cont_sim_lido] != '\0') {
                    lexema[strlen(lexema)] = code[cont_sim_lido]; // Adicione o caractere ao lexema
                    cont_sim_lido++;
                }
                if (code[cont_sim_lido] == '"') {
                    // String foi fechada corretamente
                    cont_sim_lido++; // Avançar para além da última aspa
                    estado = 0; // Retornar ao estado inicial
                    strcpy(token.lexema, lexema); // Copiar o lexema para a estrutura do token
                    return token;
                } else {
                    // Erro: string não foi fechada corretamente
                    estado = falhar();
                }
                break;

            // Adicione mais casos conforme necessário para operadores, delimitadores, etc.

            default:
                // Se nenhum caso corresponder, retorne um token de erro
                token.nome_token = ERRO;
                token.atributo = 0;
                estado = 0; // Retornar ao estado inicial
                strcpy(token.lexema, lexema); // Copiar o lexema para a estrutura do token
                return token;
        }
    }
    token.nome_token = EOF_TOKEN;
    token.atributo = -1;
    strcpy(token.lexema, lexema); // Copiar o lexema para a estrutura do token
    return token;
}

// Função para verificar se uma string é uma palavra-chave
int isKeyword(char *lexema) {
    int i = 0;
    while (keywords[i] != NULL) {
        if (strcmp(lexema, keywords[i]) == 0) {
            return 1; // É uma palavra-chave
        }
        i++;
    }
    return 0; // Não é uma palavra-chave
}

int main() {
    struct Token token;
    code = "if (x == 5) { break; } else { do_something(); }";
    while (1) {
        token = proximo_token();
        if (token.nome_token == EOF_TOKEN) {
            break; // Fim do arquivo
        }
        // Lógica para lidar com tokens reconhecidos (imprimir ou processar)
        if (token.nome_token == RELOP) {
            printf("Token: RELOP, Atributo: %d, Lexema: %s\n", token.atributo, token.lexema);
        } else if (token.nome_token == ID) {
            printf("Token: ID, Atributo: %d, Lexema: %s\n", token.atributo, token.lexema);
        } else if (token.nome_token == NUM) {
            printf("Token: NUM, Atributo: %d, Lexema: %s\n", token.atributo, token.lexema);
        } else if (token.nome_token == STRING) {
            printf("Token: STRING, Atributo: %d, Lexema: %s\n", token.atributo, token.lexema);
        } else if (token.nome_token == ERRO) {
            printf("Token: ERRO, Atributo: %d, Lexema: %s\n", token.atributo, token.lexema);
        } else {
            printf("Token: %c, Lexema: %s\n", token.nome_token, token.lexema);
        }
    }
    return 0;
}
