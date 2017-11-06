#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "micro.h"

//error de tipos
//errores sintaxics


using namespace std;



tabla TS[500];
int indice = 0;
int cantTemp = 1;
TOKEN tokenActual;
char buffer[100];


FILE *archivoInicial;

FILE *archivoMV;

/* ---------------------------


int StringANumero(String numero){
    int numero;
    char str[20];

    sscanf(str, numero);
    numero = atoi(str);

    return numero;
}


REG_EXPRESION sumar(REG_EXPRESION izquierda, REG_OPERACION operando,REG_EXPRESION derecha){
    //pagina 51 -> explicacion
    REG_EXPRESION resultado;

    int numeroIzq;
    int numeroDer;

    numeroIzq = StringANumero(extraer(izquierda));
    numeroDer = StringANumero(extraer(derecha));

    if (operando.valor == '+'){
        resultado = numeroIzq + numeroDer
    }else resultado = numeroIzq - numeroDer

    return resultado;
}

*/
void generar(String String1, String String2, String String3, String String4){

    fwrite (String1, sizeof(String), 1, archivoMV);
    fwrite (String2, sizeof(String), 1, archivoMV);
    fwrite (String3, sizeof(String), 1, archivoMV);
    fwrite (String4, sizeof(String), 1, archivoMV);
    fwrite ("\n", sizeof(char), 1, archivoMV);
}


void comenzar(){

    archivoMV = fopen("textoFinal.txt","w");

    fwrite ("INICIO",1, sizeof("INICIO"), archivoMV);
}


void terminar(){

    fwrite ("FIN",1, sizeof("FIN"), archivoMV);

    fclose(archivoMV);
}


void objetivo(void){

    programa();
    match(FDT);
    terminar();
}

void programa(void){
    match(INICIO);
    comenzar();
    void listaSentencias();
    match(FIN);
}

void sentencia(TOKEN tok){
    switch(tok){
        case ID:
            match(ID);
            break;
        case LEER:
            match(LEER);
            match(PARENIZQUIERDO);
            listaIndentificadores();
            match(PARENDERECHO);
            match(PUNTOYCOMA);
            break;
        case ESCRIBIR:
            match(ESCRIBIR);
            match(PARENIZQUIERDO);
            listaIdentificadores();
            match(PARENDERECHO);
            match(PUNTOYCOMA);
            break;

        default:
            errorSintactico(tok);
            break;
    }
}

void listaSentencias(void){
    proximoToken();
    while(1){
        switch(tokenActual){
            case ID: case LEER: case ESCRIBIR  :
            sentencia(tokenActual);
            break;
            default:
                return;
        }
    }
}

void identificador(tabla *id){
    match(ID);
    *id = procesarId();
}

void listaIdentificadores(void){
    REG_EXPRESION registro;
    identificador(&registro);
    Leer(registro);
    proximoToken();
    while(tokenActual==COMA /*&& tokenActual!= PARENDERECHO*/){
        identificador(&registro);
        Leer(registro);
        proximoToken();
    }
}

void primaria(REG_EXPRESION *operando){
    proximoToken();
    switch(tokenActual){
    case ID:
        match(ID);
        operando->clase = tokenActual;
        break;
    case CONSTANTE:
        match(CONSTANTE);
        operando->clase = tokenActual;
        break;
    case PARENIZQUIERDO:
        match(PARENIZQUIERDO);
        expresion(operando);
        match(PARENDERECHO);
        break;
    }
}

void expresion(REG_EXPRESION *resultado){
    REG_EXPRESION operandoIzq, operandoDer;
    REG_OPERACION op;
    proximoToken();
    primaria(&operandoIzq); // <- aparece de la nada?
    while (tokenActual == SUMA || tokenActual == RESTA){
        operadorAditivo(&op);
        primaria(&operandoDer);
        operandoIzq = genInfijo(operandoIzq, op, operandoDer);
        proximoToken();

    }

    *resultado = operandoIzq;
}

void listaExpresiones(){
    REG_EXPRESION registro;
    expresion(&registro);
    escribir(registro);
    while(tokenActual==COMA /*&& tokenActual!= PARENDERECHO*/){
        expresion(&registro);
        escribir(registro);
        proximoToken();
    }
}

REG_EXPRESION genInfijo(REG_EXPRESION e1, REG_OPERACION op, REG_EXPRESION e2){

    REG_EXPRESION registro;
    String opStr;
    //registro debe tener token ID
    String temp = "temp";
    if(op.valor== '+')strcpy(opStr, "Sumar");
    if(op.valor== '-')strcpy(opStr, "Restar");
    strcat(temp, cantTemp);
    generar(opStr, Extraer(&e1), Extraer(&e2),temp);

    strcpy(registro.nombre,temp);
    cantTemp++;
    return registro;
}

void operadorAditivo(){
    proximoToken();
    if(tokenActual == SUMA || tokenActual == RESTA)
        match(tokenActual);
    else
        errorSintactico(tokenActual);
}

void Leer(REG_EXPRESION in){
    generar("Read", in.nombre, "Entera", "");
}

void Escribir(REG_EXPRESION out){
    generar("Write", extraer(out), "Entera", "");
}

REG_EXPRESION procesarCte (void){
    REG_EXPRESION t;
    t.clase = CONSTANTE;
    scanf(buffer, "%d", &t.nombre);
    return t;
}

REG_EXPRESION procesarId(void){
    REG_EXPRESION t;
    t.clase = ID;
    strcpy(t.nombre, buffer);
    return t;
}

char *extraer(REG_EXPRESION *registro){
    return registro->nombre;
}

/*
void terminar(void){
    generar("Detiene", "", "", "");
}
*/

void asignar(REG_EXPRESION izquierda, REG_EXPRESION derecha){
    generar("Almacena", extraer(derecha), izquierda.nombre, "");
}


void match(TOKEN tokenEsperado){
    proximoToken();

    if (tokenEsperado != tokenActual){

        errorLexico();
    }

}

void proximoToken(){
        tokenActual = scanner();
        switch(tokenActual){
            case ID: colocar(buffer);
            break;
        }
    return;
}


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


    while (!feof(archivoInicial) && estado!=14 && estado!=99 && !esEstadoFinal(estado))
    {
        caracter = getc(archivoInicial);
        int col;
        col = columna(caracter);
        estado = mat[estado][col];
        if (estado != 2 && estado != 4 && caracter!='\n'){
            agregarCaracter(caracter, i);
            i++;
        }

    }

    switch(estado){

        case 2: ungetc(caracter, archivoInicial);
                if(esReservada(buffer)){
                    printf("Palabra reservada \t   %s \n", buffer);

                    if(strcmp(buffer, "inicio")) return INICIO;
                    if(strcmp(buffer, "fin")) return FIN;
                    if(strcmp(buffer, "leer")) return LEER;
                    if(strcmp(buffer, "escribir")) return ESCRIBIR;


                } else {
                    printf("Identificador \t   %s \n", buffer);
                    colocar(buffer);
                    chequear(buffer;
                    return ID;
                }
        case 4: ungetc(caracter, archivoInicial);
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

void inicializarTabla(){
    memset(TS, 0 ,500);
 }

bool buscar(String nuevoId){
    for(int j=0;j<=indice; j++){
        if(TS[indice].id==nuevoId){
            return true;
        }
    }
    return false;
}

void colocar(String nuevoId){

    TS[indice].t = ID;
    strcpy(TS[indice].id, nuevoId);
    indice++;
    return;
}

void chequear(String s){
    if(!buscar(s)){
        colocar(s);
        generar("Declara", s, "Entera", "");
    }
}

void errorSintactico(){
    printf("Error sintactico\n");

    fwrite ("Error sintactico\n", sizeof(String), 1, archivoMV);
}

void errorLexico(){
    printf("Error Lexico\n");

    fwrite ("Error Lexico\n", sizeof(String), 1, archivoMV);
}

int main()
{
    inicializarTabla();

    archivoInicial = fopen("texto.txt","r");

    objetivo();

    fclose(archivoInicial);

    return 0;
}
