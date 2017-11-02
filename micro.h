#ifndef MICRO_H_INCLUDED
#define MICRO_H_INCLUDED

typedef enum {
    INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO, PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT
} TOKEN;

void match(TOKEN a);
void programa(void);
void listaSentencias(void);
void sentencia(TOKEN a);
void listaIdentificadores();
void proximoToken();

#endif // MICRO_H_INCLUDED
