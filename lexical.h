#include "shared.h"
#include "iostream"
#include <fstream>

#ifndef LEXICAL_H
#define LEXICAL_H

//DEFINICIONES, SIMBOLOS QUE CONCLUYEN CON UN 'DIFERENTE'
#define PALABRA_RESERVADA 100
#define IDENTIFICADOR 101
#define CONSTANTE_NUMERICA_ENTERA 102
#define CONSTANTE_NUMERICA_REAL 103
#define CONSTANTE_NUMERICA_NC 104
#define ASIGNACION 120
#define MAYOR_QUE 122
#define MENOR_QUE 124
#define NOT 119

//Vars
std::ifstream theFile;
static cadena_tipo lexema;
static char c;

//global
static int Ltoken,edo, col,elfounds;;

//Tabla de transicion de estados
static int TTABLE[19][33] = {
    //       l,  d,  _,  E,  e,  +,  -,  *,  /,  %,  (,  ),  [,  ],  {,  },  &,  |,  !,  =,  >,  <,  ,,  ;, :,   .,  #,  ',  ", /n, /t, /b, =/
    /*00*/{  1,  3,500,  1,  1,105,106,107,108,109,110,111,112,113,114,115,  9, 10, 11, 12, 13, 14,126,127,128,129, 15, 16, 18,  0,  0,  0,500},
    /*01*/{  1,  2,  2,  1,  1,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100,100},
    /*02*/{  2,  2,  2,  2,  2,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101,101},
    /*03*/{102,  3,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,102,  4,102,102,102,102,102,102,102},
    /*04*/{501,  5,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501,501},
    /*05*/{103,  5,103,  6,  6,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103,103},
    /*06*/{502,  8,502,502,502,  7,  7,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502,502},
    /*07*/{503,  8,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503,503},
    /*08*/{104,  8,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104,104},
    /*09*/{504,504,504,504,504,504,504,504,504,504,504,504,504,504,504,504,116,504,504,504,504,504,504,504,504,504,504,504,504,504,504,504,504},
    /*10*/{505,505,505,505,505,505,505,505,505,505,505,505,505,505,505,505,505,117,505,505,505,505,505,505,505,505,505,505,505,505,505,505,505},
    /*11*/{119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,119,118,119,119,119,119,119,119,119,119,119,119,119,119,119},
    /*12*/{120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,121,120,120,120,120,120,120,120,120,120,120,120,120,120},
    /*13*/{122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,122,123,122,122,122,122,122,122,122,122,122,122,122,122,122},
    /*14*/{124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,125,124,124,124,124,124,124,124,124,124,124,124,124,124},
    /*15*/{ 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15,130, 15, 15, 15},
    /*16*/{ 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17,506, 17, 17, 17, 17, 17},
    /*17*/{507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,507,131,507,507,507,507,507},
    /*18*/{ 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18,132, 18, 18, 18, 18}
};

//Tabla de tokens
cadena_tipo Token(int edo)
{
    cadena_tipo cad = "NO ENCONTRADO";
    switch(edo){
        case 100:{
        cad = "Palabra reservada";
            break;
        }
        case 101:{
        cad =  "Identificador";
             break;
        }
        case 102:{
        cad =  "Constante numerica entera";
             break;
        }
        case 103:{
        cad =  "Constante numerica real";
             break;
        }
        case 104:{
        cad =  "Constante numerica notación cientifica";
             break;
        }
        case 105:{
        cad =  "Operador aritmetico suma";
             break;
        }
        case 106:{
        cad =  "Operador aritmetico resta";
             break;
        }
        case 107:{
        cad =  "Operador aritmetico multiplicación";
             break;
        }
        case 108:{
        cad =  "Operador aritmetico divisíon";
             break;
        }
        case 109:{
        cad =  "Operador aritmetico modulo";
             break;
        }
        case 110:{
        cad =  "Parentesis de apertura";
             break;
        }
        case 111:{
        cad =  "Parentesis de cierre";
            break;
        }
        case 112:{
        cad =  "Corchete de apertura";
             break;
        }
        case 113:{
        cad =  "Corchete de cierre";
             break;
        }
        case 114:{
        cad =  "Llave de apertura";
             break;
        }
        case 115:{
        cad =  "Llave de cierre";
            break;
        }
        case 116:{
        cad =  "Operador lógico AND";
             break;
        }
        case 117:{
        cad =  "Operador lógico OR";
             break;
        }
        case 118:{
        cad =  "Operador asociativo diferente";
             break;
        }
        case 119:{
        cad =  "Operador lógico NOT";
             break;
        }
        case 120:{
        cad =  "Asignación";
             break;
        }
        case 121:{
        cad =  "Comparación (igualdad)";
             break;
        }
        case 122:{
        cad =  "Mayor que";
             break;
        }
        case 123:{
        cad =  "Mayor o igual que";
             break;
        }
        case 124:{
        cad = "Menor que";
             break;
        }
        case 125:{
        cad = "Menor o igual que";
            break;
        }
        case 126:{
        cad = "Signo puntuación coma";
            break;
        }
        case 127:{
        cad = "Signo de puntuación punto y coma";
             break;
        }
        case 128:{
        cad = "Signo de puntuación dos puntos";
            break;
        }
        case 129:{
        cad = "Signo de puntuación punto";
             break;
        }
        case 130:{
        cad = "Comentario";
            break;
        }
        case 131:{
        cad = "Caracter";
             break;
        }
        case 132:{
        cad = "String";
             break;
        }
    }
    return cad;
}

//Tabla de errores
cadena_tipo Error(int edo)
{
    cadena_tipo cad = "CODIGO DE ERROR NO ENCONTRADO";
    switch(edo){
        case 500:{
        cad = "Simbolo no reconocido por el lenguaje";
            break;
        }
        case 501:{
        cad = "Numero real malformado se esperaba un digito despues de '.' ";
            break;
        }
        case 502:{
        cad = "Notacion cientifica malformada se esperaba un digito despues del exponente (e/E)";
            break;
        }
        case 503:{
        cad = "Notacion cientifica malformada se esperaba un digito para el exponente";
            break;
        }
        case 504:{
        cad = "AND malformado se esperaba &";
            break;
        }
        case 505:{
        cad = "OR malformado se esperaba |";
            break;
        }
        case 506:{
        cad = "Se esperaba un carater, '' es invalido";
            break;
        }
        case 507:{
        cad = "se esperaba ' pero no se encontro";
            break;
        }
    }
    return cad;
}

//Funcion de relacion de caracter
int relaciona(char c)
{
    //Es letra?
    if((c>='A'&&c<='Z'&&c!='E')||(c>='a'&&c<='z'&&c!='e')){
        return 0;
    }

    //ES numero?
    if(c>=0x30&&c<=0x39){
        return 1;
    }

    switch(c)
    {
        case '_':
            return 2;
        case 'E':
            return 3;
        case 'e':
            return 4;
        case '+':
            return 5;
        case '-':
            return 6;
        case '*':
            return 7;
        case '/':
            return 8;
        case '%':
            return 9;
        case '(':
            return 10;
        case ')':
            return 11;
        case '[':
            return 12;
        case ']':
            return 13;
        case '{':
            return 14;
        case '}':
            return 15;
        case '&':
            return 16;
        case '|':
            return 17;
        case '!':
            return 18;
        case '=':
            return 19;
        case '>':
            return 20;
        case '<':
            return 21;
        case ',':
            return 22;
        case ';':
            return 23;
        case ':':
            return 24;
        case '.':
            return 25;
        case '#':
            return 26;
        case 39:
            return 27;
        //39->"'"
        case '"':
            return 28;
        //10 -> enter
        case 10:
            return 29;
        //9 -> tabulacion
        case 9:
            return 30;
        //32 -> espacio en blanco
        case 32:
            return 31;
        default:
            return 32;
    }

}

//Funcion para comprobar si  c es un numero
bool esNumero(char c){
    return (c>='0'&&c<='9');
}

//Funcion para comprobar si c es una letra
bool esLetra(char c){
    return (c>='A'&&c<='Z')||(c>='a'&&c<='z');
}

//Funcion para comparar si c es un guion bajo
bool esGuionBajo(char c){
    return c=='_';
}

//elimina el ultimo caracter de lexema
void popcharacter(char c){
    if(!(c==0x09||c==0x0A||c==0x20)){
        theFile.unget();
        lexema.pop_back();
    }
}

void loadFile(char *filepath){
    theFile.open (filepath, std::ios::in);
}

void loadStdFile(cadena_tipo filepath){
    theFile.open (filepath, std::ios::in);
}

void closeFile(){
    if(theFile.is_open()){
        if(theFile.eof()){
            theFile.get();
            theFile.clear();
            theFile.close();
        }
    }
}


//Metodo que trae el siguiente token
//Analizador Lexico, un token a la vez
lex_entry *dameToken(){

    //inicializacion de estado
    edo = 0;

    //inicializacion de cadenas
    lexema = "";

    //si es fin de archivo regresa -1
    if(theFile.eof()){
        Ltoken = -1;
        return nullptr;
    }

    //mientras haya caracteres por leer
    while(edo<100){

        //mientras no se encuentra em estado terminal
        //analizar
        while(edo<=18){

            //obtenemos siguiente caracter
            c = theFile.get();

            if(c==0x00||theFile.eof()){
                Ltoken = -1;
                return nullptr;
            }

            //guardar el caracter actual en la cadena lexema
            if(c!=-1){
                if(c!=0x09&&c!=0x0A&&c!=0x20){
                    lexema += c;
                }
                col = relaciona(c);
            }else{
                //fin de fichero, marcamos como diferente
                col = 32;
            }

            //Avanza al siguiente estado
            edo = TTABLE[edo][col];
        }

        //ESTOS SON LOS ELEMENTOS QUE NO CONCLUYEN DIRECTAMENTE
        switch(edo){
            case PALABRA_RESERVADA:
                if(!esLetra(c))popcharacter(c);
                break;
            case IDENTIFICADOR:
                if(!(esNumero(c)||esLetra(c)||esGuionBajo(c)))popcharacter(c);
                break;
            case CONSTANTE_NUMERICA_ENTERA:
            case CONSTANTE_NUMERICA_REAL:
            case CONSTANTE_NUMERICA_NC:
                if(!esNumero(c)&&c!=-1)popcharacter(c);
                break;
            case ASIGNACION:
            case MAYOR_QUE:
            case MENOR_QUE:
            case NOT:
                popcharacter(c);
                break;
        }

        //Analizamos el resultado del analisis
        if(edo>=100&&edo<=132){
            elfounds++;
            Ltoken = edo;

            lex_entry *a = new lex_entry();
            a->token = edo;
            a->lexema = lexema;
            a->granema = Token(edo);
            return a;

        }
        if(c==-1){
            break;
        }
    }

    return nullptr;

}

#endif // LEXICAL_H
