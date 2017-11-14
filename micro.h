#ifndef MICRO_H_INCLUDED
#define MICRO_H_INCLUDED



typedef enum {
 INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
 PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT, ERRORLEXICO
} TOKEN;


typedef TOKEN REG_OPERACION;

struct REG_EXPRESION {
    int valor;
    char nombre[32+1];
    TOKEN clase;
};

typedef char String[32];


struct tabla{
    String id;
    TOKEN t;

} ;

TOKEN esReservada(void);
TOKEN scanner(void);
void Objetivo(void);
void Programa(void);
void Generar(String string1,String string2,String string3,String string4);
void match(TOKEN t);
void sentencia(void);
void listaIdentificadores(void);
void expresion(REG_EXPRESION&);
void OperadorAditivo(REG_OPERACION&);
void ErrorSintactico();
void Primaria(REG_EXPRESION&);
void listaExpresiones(void);
void ErrorLexico(void);
void Terminar(void);
void chequear(String);
REG_EXPRESION ProcesarID(void);
REG_EXPRESION ProcesarCte(void);
REG_EXPRESION Identificador(void);
void Leer(REG_EXPRESION);
void Asignar(REG_EXPRESION,REG_EXPRESION);
char* Extraer(REG_EXPRESION);
REG_EXPRESION GenInfijo(REG_EXPRESION,REG_OPERACION,REG_EXPRESION);
void Escribir(REG_EXPRESION);



#endif // MICRO_H_INCLUDED
