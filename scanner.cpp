#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "micro.h"


tabla TS[500];
int indice = 0;
char buffer[1024];

TOKEN tokenActual;
FILE * fuente;


int tmp=0;


int main(int argc, char* argv[]) {
    fuente = fopen("test.txt","r");
    Objetivo();
    fclose(fuente);
    getchar();

}

void match(TOKEN t) {
    if (t!= tokenActual)
        ErrorSintactico();
    else
        tokenActual = scanner();
}


int columna(int a) {
    if (isalpha(a)) return 0;
    if (isdigit(a)) return 1;
    if (a=='+') return 2;
    if (a=='-') return 3;
    if (a=='(') return 4;
    if (a==')') return 5;
    if (a==',') return 6;
    if (a==';') return 7;
    if (a==':') return 8;
    if (a=='=') return 9;
    if (a==EOF) return 10;
    if (isspace(a)) return 11;
    return 12;
}
void agregarCaracter(int c, int i){
    buffer[i] = c;
}

void vaciarBuffer(){
    memset(buffer, '\0', sizeof(buffer));
}


bool esReservada(char palabra[]){
    return !strcmp(palabra, "inicio") || !strcmp(palabra, "fin") || !strcmp(palabra, "leer") || !strcmp(palabra, "escribir") ;
}


TOKEN scanner(void) {
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


    vaciarBuffer();
    int estado = 0, i=0;
    char caracter;
    while (!feof(fuente)) {
        caracter = fgetc(fuente);
        int col = columna(caracter);
        estado = mat[estado][col];
        switch (estado) {
            case 1: agregarCaracter(caracter,i);
                    i++;
                    break;
            case 2: ungetc(caracter,fuente);
                    if(esReservada(buffer)){
                        printf("Reservada \t   %s \n", buffer);
                        if(!strcmp(buffer, "inicio")) return INICIO;
                        if(!strcmp(buffer, "fin")) return FIN;
                        if(!strcmp(buffer, "leer")) return LEER;
                        if(!strcmp(buffer, "escribir")) return ESCRIBIR;
                        printf("Re \t   %s \n", buffer);
                    } else {
                            printf("Identificador \t   %s \n", buffer);
                            chequear(buffer);
                            return ID;
                            }
            case 3: agregarCaracter(caracter,i);
                    i++;
                    break;
            case 4: ungetc(caracter,fuente);
                    printf("Constante \t   %s \n", buffer);
                    return CONSTANTE;
            case 5: return SUMA;
            case 6: return RESTA;
            case 7: return PARENIZQUIERDO;
            case 8: return PARENDERECHO;
            case 9: return COMA;
            case 10: return PUNTOYCOMA;
            case 12: return ASIGNACION;
            case 14: ErrorLexico();return ERRORLEXICO;

        }

    }
    if (feof(fuente)) return FDT;
}

void Objetivo(void) {
    tokenActual = scanner();
    Programa();
    match(FDT);
    Terminar();
}



void listaSentencias(void){
    sentencia();
    while (1){
        switch (tokenActual) {
            case ID: sentencia();
                break;
            case LEER: sentencia();
                break;
            case ESCRIBIR: sentencia();
                break;
            default:
                return;
        }
    }
}

void Programa(void) {
    match(INICIO);
    listaSentencias();
    match(FIN);
}

void sentencia(void) {
    REG_EXPRESION exp;
    REG_EXPRESION asg;
    switch (tokenActual) {
        case ID: asg=Identificador();
            match(ASIGNACION);
            expresion(exp);
            match(PUNTOYCOMA);
            Asignar(asg,exp);
            break;
        case LEER:
            match(LEER);
            match(PARENIZQUIERDO);
            listaIdentificadores();
            match(PARENDERECHO);
            match(PUNTOYCOMA);
            break;
        case ESCRIBIR:
            match(ESCRIBIR);
            match(PARENIZQUIERDO);
            listaExpresiones();
            match(PARENDERECHO);
            match(PUNTOYCOMA);
            break;
        default: ErrorSintactico();
            break;
    }
}

void listaIdentificadores(void) {
    Leer(Identificador());
    while (1) {
        switch (tokenActual) {
            case COMA:
                match(COMA);
                Leer(Identificador());
                break;
            default:
                return;
        }
    }
}

void Leer(REG_EXPRESION in) {
    Generar("Read",in.nombre,"Entera","");
}

void expresion(REG_EXPRESION &resultado) {
    REG_EXPRESION operandoIzq,operandoDer;
    REG_OPERACION op;
    Primaria(operandoIzq);
    while (tokenActual == SUMA || tokenActual == RESTA) {
        OperadorAditivo(op);Primaria(operandoDer);
        operandoIzq = GenInfijo(operandoIzq,op,operandoDer);
    }
    resultado = operandoIzq;
}

void OperadorAditivo(REG_OPERACION &op) {
    if (tokenActual==SUMA ||tokenActual == RESTA){
        op = tokenActual;
        match(tokenActual);
    }
    else{
        ErrorSintactico();
    }
}

void ErrorSintactico() {
    printf("Error sintactico\n");

}

void ErrorLexico(void) {
    printf("Error lexico\n");
}

REG_EXPRESION Identificador(void) {
    match(ID);
    return ProcesarID();
}

void Primaria(REG_EXPRESION &operando) {
    switch(tokenActual) {
        case ID: operando=Identificador();
            break;
        case CONSTANTE: operando=ProcesarCte();
            match(CONSTANTE);
            break;
        case PARENIZQUIERDO:
            match(PARENIZQUIERDO);
            expresion(operando);
            match(PARENDERECHO);
            break;
        default:
            ErrorSintactico();

    }
}

void listaExpresiones(void) {
    REG_EXPRESION exp;
    expresion(exp);
    Escribir(exp);
    while (1) {
        switch (tokenActual) {
            case COMA:
                match(COMA);
                expresion(exp);
                Escribir(exp); break;

            default:
                return;
        }
    }
}

void Terminar(void) {
    Generar("Detiene","","","");
}

void Generar(String string1,String string2,String string3,String string4) {
    printf("%s %s %s %s\n",string1,string2,string3,string4);
}

REG_EXPRESION ProcesarID(void) {
    REG_EXPRESION t;
    t.clase = ID;
    strcpy(t.nombre,buffer);
    chequear(buffer);
    return t;
}

REG_EXPRESION ProcesarCte(void) {
    REG_EXPRESION t;
    t.clase = CONSTANTE;
    t.valor = atoi(buffer);
    return t;
}

bool buscar(String nuevoId){
    for(int j=0;j<=indice; j++){
        if(!strcmp(TS[j].id,nuevoId)){
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
        Generar("Declara", s, "Entera", "");
    }
}
void Asignar(REG_EXPRESION izquierda,REG_EXPRESION derecha) {
    String auxstr;
    if (derecha.clase == CONSTANTE) {
        sprintf(auxstr, "%d", derecha.valor);
    } else{
        strcpy(auxstr,derecha.nombre);
    }
    Generar("Almacena",auxstr,izquierda.nombre,"");
}

char* Extraer(REG_EXPRESION t){
    String auxstr;
    if (t.clase == CONSTANTE) {
        sprintf(auxstr, "%d", t.valor);
        return auxstr;
    }
    strcpy(auxstr,t.nombre);
    return auxstr;
}

REG_EXPRESION GenInfijo(REG_EXPRESION e1,REG_OPERACION op,REG_EXPRESION e2){
    tmp++;
    String tmpstring;
    sprintf(tmpstring,"Temp&%d",tmp);
    chequear(tmpstring);
    String auxstr;
    if (e1.clase == CONSTANTE) {
        sprintf(auxstr, "%d", e1.valor);
    } else{
        strcpy(auxstr,e1.nombre);
    }
    String auxstr2;
    if (e2.clase == CONSTANTE) {
        sprintf(auxstr2, "%d", e2.valor);
    } else{
        strcpy(auxstr2,e2.nombre);
    }
    if (op==SUMA){
        Generar("Suma",auxstr,auxstr2,tmpstring);
    }
    else{
        Generar("Resta",auxstr,auxstr2,tmpstring);
    }
    REG_EXPRESION t;
    t.clase = ID;
    strcpy(t.nombre,tmpstring);
    return t;
}

void Escribir(REG_EXPRESION out){
    String auxstr;
    if (out.clase == CONSTANTE) {
        sprintf(auxstr, "%d", out.valor);
    } else{
        strcpy(auxstr,out.nombre);
    }
    Generar("Write",auxstr,"Entera","");
}
