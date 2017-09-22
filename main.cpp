#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>



using namespace std;

typedef enum {
    INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO, PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT
} TOKEN;



FILE *archivo;

char buffer[100];

void agregarCaracter(int c, int i){
    buffer[i] = c;
}

void vaciarBuffer(){
    memset(buffer, '\0', sizeof(buffer));
}

int columna(char a)
{
  if(isalpha(a)) return 0;
  if(isdigit(a)) return 1;
  if(a=='+') return 2;
  if(a=='-') return 3;
  if(a=='(') return 4;
  if(a==')') return 5;
  if(a==',') return 6;
  if(a==';') return 7;
  if(a==':') return 8;
  if(a=='=') return 9;
  if(a==EOF) return 10;
  if(isspace(a)) return 11;
  return 12;
}

bool esEstadoFinal(int estado){
    return  estado == 2 ||
            estado == 4 ||
            estado == 5 ||
            estado == 6 ||
            estado == 7 ||
            estado == 8 ||
            estado == 9 ||
            estado == 10 ||
            estado == 12 ||
            estado == 13;
}

bool esReservada(char palabra[]){
    return !strcmp(palabra, "inicio") || !strcmp(palabra, "fin") || !strcmp(palabra, "leer") || !strcmp(palabra, "escribir") ;
}

TOKEN scanner()
{
    static int mat[15][13] = {{1,3,5,6,7,8,9,10,11,14,13,0,14}, //Estado 0
                            {1,1,2,2,2,2,2,2,2,2,2,2,2}, //Estado 1
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 2
                            {4,3,4,4,4,4,4,4,4,4,4,4,4}, //Estado 3
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 4
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 5
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 6
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 7
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 8
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 9
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 10
                            {14,14,14,14,14,14,14,14,14,12,14,14,14}, //Estado 11
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 12
                            {99,99,99,99,99,99,99,99,99,99,99,99,99}, //Estado 13
                            {99,99,99,99,99,99,99,99,99,99,99,99,99} //Estado 14
                            };

    int i=0;
    int estado=0;
    char caracter;


    while (!feof(archivo) && estado!=14 && estado!=99 && !esEstadoFinal(estado))
    {
        caracter = getc(archivo);
        int col;
        col = columna(caracter);
        estado = mat[estado][col];
        if (estado != 2 && estado != 4 && caracter!='\n'){
            agregarCaracter(caracter, i);
            i++;
        }

    }

    switch(estado){

        case 2: ungetc(caracter, archivo);
                if(esReservada(buffer)){
                    printf("Palabra reservada \t   %s \n", buffer);

                    if(strcmp(buffer, "inicio")) return INICIO;
                    if(strcmp(buffer, "fin")) return FIN;
                    if(strcmp(buffer, "leer")) return LEER;
                    if(strcmp(buffer, "escribir")) return ESCRIBIR;


                } else {
                    printf("Identificador \t   %s \n", buffer);
                    return ID;
                }
        case 4: ungetc(caracter, archivo);
                printf("Constante \t   %s \n", buffer); return CONSTANTE;
        case 5: printf("operador \t   %s \n", buffer); return SUMA;
        case 6: printf("Operador \t   %s \n", buffer); return RESTA;
        case 7: printf("Parentesis \t   %s \n", buffer); return PARENIZQUIERDO;
        case 8: printf("Parentesis \t   %s \n", buffer); return PARENDERECHO;
        case 9: printf("Caracter de puntuacion \t   %s \n", buffer); return COMA;
        case 10: printf("Caracter de puntuacion \t   %s \n", buffer); return PUNTOYCOMA;
        case 12: printf("Operador \t   %s \n", buffer); return ASIGNACION;
        case 13: return FDT;
        case 14: printf("Error Lexico\n");

    }


}




int main()
{
    archivo = fopen("texto.txt","r");

    while (!feof(archivo)){
    vaciarBuffer();
    scanner();
    }

    fclose(archivo);

    return 0;
}
