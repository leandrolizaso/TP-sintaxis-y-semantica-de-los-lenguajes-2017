#ifndef MICRO_H_INCLUDED
#define MICRO_H_INCLUDED



typedef enum {
    INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO, PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT
} TOKEN;

struct REG_OPERACION{
    char valor;
} ;

struct REG_EXPRESION{
    char nombre[32];
    TOKEN clase;
};

typedef char String[32];


struct tabla{
    String id;
    TOKEN t;  //esto lo vi en el libro

};


//faltan agregar todos los prototipos xd

void comenzar();
void terminar();
void match(TOKEN a);
void programa(void);
void listaSentencias(void);
void sentencia(TOKEN a);
void listaIndentificadores();
void identificador(tabla *id);
void primaria(REG_EXPRESION *operando);
void expresion(REG_EXPRESION *resultado);
void listaExpresiones();
REG_EXPRESION genInfijo(REG_EXPRESION e1, REG_OPERACION op, REG_EXPRESION e2);
void operadorAditivo();
void Leer(REG_EXPRESION in);
void Escribir(REG_EXPRESION out);
REG_EXPRESION procesarCte (void);
REG_EXPRESION procesarId(void);
char *extraer(REG_EXPRESION *registro);
void asignar(REG_EXPRESION izquierda, REG_EXPRESION derecha);

void proximoToken();
int StringANumero(String numero);
REG_EXPRESION sumar(REG_EXPRESION izquierda, REG_OPERACION operando,REG_EXPRESION derecha);
void generar(String String1, String String2, String String3, String String4);

#endif // MICRO_H_INCLUDED
