#ifndef TRADUCTOR_H
#define TRADUCTOR_H


#include <stack>
#include <stdlib.h>     /* malloc, free, rand */
#include <stdio.h>
#include <iostream>
#include <string.h>

#include "shared.h"

using namespace std;

//pila de operadores
static std::stack<cops> POperadores;

//pila de operandos
static std::stack<int> POperandos;

//pila de tipos
static std::stack<tipos> PTipos;

//pila de tmp avail
static std::stack<int> PAvail;

//pila de saltos
static std::stack<int> PSaltos;

//variable auxilar para la declaracion
static tipos DecAux;

//contador de la estructura de la tabla de simbolos
static int SimbolosCount = 3500;
static int ConstantesCount = 7000;
static int AvailCount = 5550;

//contador de cuadruplos
static int CuadruplosCount = 1;

//traductor de codigos a palabras
static bool traducir;

//Implementacion de funciones
void switchCode(){
    traducir = !traducir;
}

//identifica si la direccion pertenece al avail
bool esAvail(int n){
    return n>=5550&&n<=6000;
}

//rellenar un cuadruplo (res con cod)
void rellenar(int a, int cod){

     CuadruploPtr nodo;
     nodo = cuadruplos;
     do{
         if(nodo->key==a){
             nodo->resl = cod;
             break;
         }else{
             nodo = nodo->next2;
         }
     }while(nodo!=nullptr);

 }


//funciones tabla de constantes
ConstantesRowPtr buscarAddrEnTDC(int a){
    ConstantesRowPtr nodo;
    nodo = TDC;
    do{
        if(nodo->count==a){
            break;
        }else{
            nodo = nodo->next2;
        }
    }while(nodo!=nullptr);
    return nodo;
}

ConstantesRowPtr buscarODeclararEnTDC(int codeid, cadena_tipo lex){


    ConstantesRowPtr nodo,encontrado;
    nodo = TDC;


    if(nodo!=nullptr){
      //recorrer la lista en busca del elemento
      do{
        if(nodo->desc==lex){
            return nodo;
        }
        nodo = nodo->next2;
      }while(nodo!=nullptr);
    }


    ConstantesRowPtr newPtr = new ConstantesRow();

    if(newPtr!=nullptr){
        //si hay espacio en memoria

        newPtr->next2 = nullptr;
        newPtr->count = ConstantesCount;

        newPtr->desc = lex;
//        cout<<"declarando"<<lex<<endl;

        switch(codeid){
            case 1001:
                newPtr->type = ENTERO;
                break;
            case 1002:
                newPtr->type = FLOTANTE;
                break;
            case 1003:
                newPtr->type = FLOTANTE;
                break;
            case 1030:
                newPtr->type = CARACTER;
                break;
            case 1031:
                newPtr->type = CADENA;
                break;
            case 1050:
                newPtr->type = BOOLEANO;
                break;
        }

        if(TDC==nullptr){
          //lista vacia, crear primer elemento
          TDC = newPtr;
          FinalElemTC = newPtr;
        }else{
            FinalElemTC->next2 = newPtr;
            FinalElemTC = newPtr;
        }

        //incrementar contador de la tabla de simbolos
        ConstantesCount++;

        //retornar la direccion declarada
        return newPtr;
        }
}

//funciones tabla de variables
SimbolosRowPtr buscarAddrEnTDS(int a){
    SimbolosRowPtr nodo;
    nodo = TDS;
    do{
        if(nodo->count==a){
            break;
        }else{
            nodo = nodo->next2;
        }
    }while(nodo!=nullptr);
    return nodo;
}

SimbolosRowPtr DeclararEnTDS(cadena_tipo lexema){

        SimbolosRowPtr newPtr = new SimbolosRow();

        if(newPtr!=nullptr){
            //si hay espacio en memoria

            newPtr->next2 = nullptr;
            newPtr->count = SimbolosCount;

            newPtr->desc = lexema;

            if(TDS==nullptr){
              //lista vacia, crear primer elemento
              TDS = newPtr;
              FinalElemTS = newPtr;
            }else{
                FinalElemTS->next2 = newPtr;
                FinalElemTS = newPtr;
            }

            //incrementar contador de la tabla de simbolos
            SimbolosCount++;

            //retornar la direccion declarada
            return newPtr;
        }else{
            throw "no hay espacio disponible en memoria";
        }
}

//devuelve la direccion del id
//o devuelve -1 si no lo encontro
int buscarIdEnTDS(cadena_tipo lexema){

    SimbolosRowPtr node;
    node = TDS;

    if(node!=nullptr){
      //recorrer la lista en busca del elemento
      do{
        if(node->desc==lexema){
            return node->count;
        }
        node = node->next2;
      }while(node!=nullptr);
    }

    return -1;
}

//funciones de temporales
TempRowPtr buscarAddrEnTmp(int a){
    TempRowPtr nodo;
    nodo = temporales;

    if(nodo==nullptr){
        return nullptr;
    }

    do{
        if(nodo->addr==a){
            break;
        }else{
            nodo = nodo->next2;
        }
    }while(nodo!=nullptr);
    return nodo;
}

//funciones auxiliares que forman parte de los
//procesos de las acciones

int obtenerAvail(){

    if(PAvail.empty()){
        int navail = AvailCount;
        AvailCount++;
        return navail;
    }else{
        int tmp = PAvail.top();
        PAvail.pop();
        return tmp;
    }
}

void liberarAvail(int addr){
    PAvail.push(addr);
}


// DESTRADUCCION (metodos de ayuda para entender los numeros que aparecen)
cadena_tipo obtenerTipo(tipos t){
    switch(t){
    case ENTERO:
        return "entero";
    case FLOTANTE:
        return "flotante";
    case CARACTER:
        return "caracter";
    case CADENA:
        return "cadena";
    case BOOLEANO:
        return "booleano";
    }
}

cadena_tipo obtenerCOP(cops c){
    switch (c) {
    case FONDO_FALSO:
        return "-1";
    case INPUT:
        return "INPUT";
    case OUTPUT:
        return "OUTPUT";
    case ASIG:
        return ":=";
    case OR:
        return "OR";
    case AND:
        return "AND";
    case DIF:
        return "!";
    case PLUS:
        return "+";
    case MINUS:
        return "-";
    case MULT:
        return "*";
    case DIV:
        return "/";
    case MODULUS:
        return "%";
    case EQUALS:
        return "==";
    case NOTEQUALS:
        return "!=";
    case LESSTHAN:
        return "<";
    case LESSOREQUALSTHAN:
        return "<=";
    case GREATERTHAN:
        return ">";
    case GREATEROREQUALSTHAN:
        return ">=";
    case GOTO:
        return "GOTO";
    case GOTOFALSO:
        return "GOTOF";
    case GOTOVERDADERO:
        return "GOTOV";
    }
}

cadena_tipo obtenerValor(int dir){
    if(dir==0){
        return "--";
    }

    if(esAvail(dir)){
        int r = dir%5550 +1;
        cadena_tipo t = "TEMP";
        t.append(std::to_string(r));
        return t;
    }else{

        if(dir<1000){
            return std::to_string(dir);
        }else{
            if(dir>=7000){
                ConstantesRowPtr n = buscarAddrEnTDC(dir);
                return n->desc;
            }else{
                SimbolosRowPtr n = buscarAddrEnTDS(dir);
                return n->desc;
            }
        }
    }
}


//funciones de cuadruplos

void generarCuadruplo(cops cop, int op1, int op2, int res)
{
    CuadruploPtr nodo = new Cuadruplo();

    nodo->key=CuadruplosCount;
    nodo->cop = cop;
    nodo->op1 = op1;
    nodo->op2 = op2;
    nodo->resl = res;
    nodo->next2 = nullptr;

    CuadruplosCount++;

    if (cuadruplos==nullptr)
    {
        cuadruplos = nodo;
        FinalCuadruplo= nodo;
    }
    else
    {
        FinalCuadruplo->next2=nodo;
        FinalCuadruplo = nodo;
    }


}

CuadruploPtr BuscarCuadruplo(int count)
{
    CuadruploPtr nodo;
    nodo=cuadruplos;
    do
    {
        if(nodo->key== count)
            break;
        else
            nodo= nodo->next2;
    }
    while(nodo!=nullptr);
    return nodo;
}


// ACCIONES

//=====> EXPRESIONES

//ID
void ACTION_2001(cadena_tipo lexema){

    int addr = buscarIdEnTDS(lexema);

    if(addr!=-1){
        SimbolosRowPtr ptr = buscarAddrEnTDS(addr);

        POperandos.push(addr);
        PTipos.push(ptr->type);

    }else{
        cout<<"ERROR NO SE DECLARO EL IDENTIFICADOR: "<<lexema<<endl;
    }
}

//push ||
void ACTION_2002() {
    POperadores.push(OR);
}

//push &&
void ACTION_2003 () {
    POperadores.push(AND);
}

//mete operador !
void ACTION_2004 () {
    POperadores.push(DIF);
}

//push + o -
void ACTION_2005 (int type) {

    if(type == 1004) {
         //1004 +
        POperadores.push(PLUS);
    } else {
        if(type==1005){
        //1005 -
        POperadores.push(MINUS);
        }else{
            cout<<"operador no valido se esperaba + o -"<<endl;
        }
    }
}

//push operando * / o %
void ACTION_2006 (int type) {
    //*
    if(type == 1006) {
        POperadores.push(MULT);
    } else {
        // /
        if(type == 1007) {
            POperadores.push(DIV);
        } else {
            // %
            if(type == 1008){
                POperadores.push(MODULUS);
            }else{
                cout<<"operador no valido, se esperaba * / %"<<endl;
            }
        }
    }
}

//busca o declara cte
void ACTION_2007 (int type, cadena_tipo lex){

    //declara o busca la constante con su tipo
    //en este metodo se comprueba y se dan de alta con su tipo
    ConstantesRowPtr nodo = buscarODeclararEnTDC(type,lex);

    POperandos.push(nodo->count);
    PTipos.push(nodo->type);

}

//mete marca de fondo falso
void ACTION_2008 (int codeid) {
    if(codeid == 1009) {
        POperadores.push(FONDO_FALSO);
    }else{
        cout<<"no es un )"<<endl;
    }
}

//saca marca de fondo falso
void ACTION_2009 () {
    POperadores.pop();
}

//cuadruplo de multiplicacion, division y modulo
void ACTION_2010 () {

    if(POperadores.empty())
        return;

    if(POperadores.top() == MULT || POperadores.top() == DIV || POperadores.top() == MODULUS) {

        tipos t1 = PTipos.top();
        PTipos.pop();
        tipos t2 = PTipos.top();
        PTipos.pop();

        if(t1==t2) {

            int operando2 = POperandos.top();
            POperandos.pop();

            int operando1 = POperandos.top();
            POperandos.pop();

            int resultado = obtenerAvail();

            generarCuadruplo(POperadores.top(),operando1,operando2,resultado);


            if(esAvail(operando2)) {
               liberarAvail(operando2);
            }
            if(esAvail(operando1)) {
               liberarAvail(operando1);
            }

            PTipos.push(t1);
            POperandos.push(resultado);
            POperadores.pop();

        } else {
            cout<<"tipos no compatibles para la operacion de ( * | / | % )"<<endl;
            cout<<"encontrado "<<obtenerTipo(t1)<<" y "<<obtenerTipo(t2)<<endl;

        }
    }
}

//cuadruplo de suma y resta
void ACTION_2011 () {

    if(POperadores.empty())
        return;

    if(POperadores.top() == PLUS || POperadores.top() == MINUS) {

        tipos t1 = PTipos.top();
        PTipos.pop();
        tipos t2 = PTipos.top();
        PTipos.pop();

        if(t1==t2) {

            int operando2 = POperandos.top();
            POperandos.pop();

            int operando1 = POperandos.top();
            POperandos.pop();

            int resultado = obtenerAvail();

            generarCuadruplo(POperadores.top(),operando1,operando2,resultado);


            if(esAvail(operando2)) {
               liberarAvail(operando2);
            }
            if(esAvail(operando1)) {
               liberarAvail(operando1);
            }

            PTipos.push(t1);
            POperandos.push(resultado);
            POperadores.pop();

        } else {
            cout<<"tipos no compatibles para la operacion de + y -"<<endl;
            cout<<"encontrado "<<obtenerTipo(t1)<<" y "<<obtenerTipo(t2)<<endl;
        }
    }
}

//push == != < <= > >=
void ACTION_2012(int codeid){
    switch (codeid) {
        //==
        case 1020:
            POperadores.push(EQUALS);
        break;
        //!=
        case 1018:
            POperadores.push(NOTEQUALS);
        break;
        //<
        case 1023:
            POperadores.push(LESSTHAN);
        break;
        //<=
        case 1024:
            POperadores.push(LESSOREQUALSTHAN);
        break;
        //>
        case 1021:
            POperadores.push(GREATERTHAN);
        break;
        //>=
        case 1022:
            POperadores.push(GREATEROREQUALSTHAN);
        break;
    default:
        cout<<"OPREL no valido: "<<codeid<<endl;
    }
}

//genera cuadruplos para OPREL
void ACTION_2013(){

    tipos t1,t2;

    t1 = PTipos.top();
    PTipos.pop();

    t2 = PTipos.top();
    PTipos.pop();

    //si top y top-1 son compatibles entonces
    if(t1==t2){

        //obtener operando 2
        int operando2 = POperandos.top();
        POperandos.pop();

        //obtener operando 1
        int operando1 = POperandos.top();
        POperandos.pop();

        //obtiene una direccion correspondiente al avail
        int resultado = obtenerAvail();

        //se obtiene el operador
        cops operador = POperadores.top();

        //genera cuadruplo
        generarCuadruplo(operador,operando1,operando2,resultado);

        if(esAvail(operando1))
            liberarAvail(operando1);

        if(esAvail(operando2))
            liberarAvail(operando2);

        POperandos.push(resultado);
        PTipos.push(BOOLEANO);

        POperadores.pop();
    }else{
        cout<<"tipos no compatibles para la operacion de oprel"<<endl;
        cout<<"encontrado "<<obtenerTipo(t1)<<" y "<<obtenerTipo(t2)<<endl;
    }
}

//genera cuadruplo del not (unario)
void ACTION_2014(){

    //TODO si top de la pila es booleano
    if(PTipos.top()==BOOLEANO){

        int resultado = POperandos.top();
        POperandos.pop();

        cops operador = POperadores.top();
        POperadores.pop();

        //generar cuadruplo del not
        generarCuadruplo(operador,0,0,resultado);

        //linea extra no viene
        //en los diagramas pero debe de ir (?)
        POperandos.push(resultado);
    }

}

//genera cuadruplo de comparacion &&
void ACTION_2015(){

    tipos t1,t2;

    t1 = PTipos.top();
    PTipos.pop();

    t2 = PTipos.top();
    PTipos.pop();

    //si top y top-1 son compatibles entonces
    if(t1==BOOLEANO&&t2==BOOLEANO){

        //obtener operando 2
        int operando2 = POperandos.top();
        POperandos.pop();

        //obtener operando 1
        int operando1 = POperandos.top();
        POperandos.pop();

        //obtiene una direccion correspondiente al avail
        int resultado = obtenerAvail();

        //se obtiene el operador
        cops operador = POperadores.top();

        //genera cuadruplo
        generarCuadruplo(operador,operando1,operando2,resultado);

        if(esAvail(operando1))
            liberarAvail(operando1);

        if(esAvail(operando2))
            liberarAvail(operando2);

        POperandos.push(resultado);
        PTipos.push(t1);

        POperadores.pop();
    }else{
        cout<<"tipos no compatibles para &&"<<endl;
        cout<<"encontrado "<<obtenerTipo(t1)<<" y "<<obtenerTipo(t2)<<endl;
    }
}

//genera cuadruplo de comparacion ||
void ACTION_2016(){

    tipos t1,t2;

    t1 = PTipos.top();
    PTipos.pop();

    t2 = PTipos.top();
    PTipos.pop();

    //si top y top-1 son compatibles entonces
    if(t1==BOOLEANO&&t2==BOOLEANO){

        //obtener operando 2
        int operando2 = POperandos.top();
        POperandos.pop();

        //obtener operando 1
        int operando1 = POperandos.top();
        POperandos.pop();

        //obtiene una direccion correspondiente al avail
        int resultado = obtenerAvail();

        //se obtiene el operador
        cops operador = POperadores.top();

        //genera cuadruplo
        generarCuadruplo(operador,operando1,operando2,resultado);

        if(esAvail(operando1))
            liberarAvail(operando1);

        if(esAvail(operando2))
            liberarAvail(operando2);

        POperandos.push(resultado);
        PTipos.push(t1);

        POperadores.pop();
    }else{
        cout<<"tipos no compatibles para ||"<<endl;
        throw "Error semantico, tipos no compatibles";
    }
}

//=====> ASIGNACON
void ACTION_2022(){

    tipos t1 = PTipos.top();
    PTipos.pop();

    tipos t2 = PTipos.top();
    PTipos.pop();

    if(t1==t2){
        int exp = POperandos.top();
        POperandos.pop();
        int res = POperandos.top();
        POperandos.pop();
        generarCuadruplo(ASIG,exp,0,res);

        if(esAvail(exp))
            liberarAvail(exp);
        if(esAvail(res))
            liberarAvail(res);
    }else{
        cout<<"ERROR DE ASIGNACION, no puede asignarse ";
        cout<<"una variable de tipo "<<obtenerTipo(t1)<<" a una de tipo "<<obtenerTipo(t2)<<endl;
        throw "ERROR SEMANTICO";
    }
}

//Declaracion
void ACTION_2017(int code_id){

    if(code_id == 1033){
        DecAux = ENTERO;
    }

    if(code_id == 1034)
    {
        DecAux = FLOTANTE;
    }

    if(code_id == 1049){
        DecAux = BOOLEANO;
    }

    if(code_id == 1035)
    {
        DecAux = CARACTER;
    }

    if(code_id == 1036)
    {
        DecAux = CADENA;
    }


}

void ACTION_2018(cadena_tipo lexema){

    int a = buscarIdEnTDS(lexema);

    if(a==-1){

        SimbolosRowPtr nodo = DeclararEnTDS(lexema);
        POperandos.push(nodo->count);

    }else{
        cout<<"la variable ya se encuentra declarada"<<endl;;
    }
}

void ACTION_2019(){

    while(!POperandos.empty()){

        int a = POperandos.top();
        POperandos.pop();

        SimbolosRowPtr n = buscarAddrEnTDS(a);
        n->type = DecAux;
    }

}

//READ
void ACTION_2020(cadena_tipo lexema){

    int addr = buscarIdEnTDS(lexema);

    if(addr!=-1){
        generarCuadruplo(INPUT,0,0,addr);
    }else{
        cout<<"NO SE DECLARO "<<lexema<<endl;
    }
}

//WRITE
void ACTION_2021(){
    PTipos.pop();
    int op = POperandos.top();
    POperandos.pop();
    generarCuadruplo(OUTPUT,0,0,op);

}

//WHILE

void ACTION_2023(){
    PSaltos.push(CuadruplosCount);
}

void ACTION_2024(){
        tipos aux = PTipos.top();
        PTipos.pop();

        if(aux!=BOOLEANO){
            cout<<"WHILEDO -> no se esta evaluando una expresion booleana valida"<<endl;
        }
        else{
            int resultado = POperandos.top();
            POperandos.pop();

            generarCuadruplo(GOTOFALSO,resultado,0,CuadruplosCount);
            PSaltos.push(CuadruplosCount-1);

            if(esAvail(resultado)){
                liberarAvail(resultado);
            }

        }
    }

void ACTION_2025(){

    int f = PSaltos.top();
    PSaltos.pop();

    int retorno=PSaltos.top();
    PSaltos.pop();

    generarCuadruplo(GOTO,0,0, retorno);
    rellenar(f,CuadruplosCount);
}

//EST_DO
void ACTION_2026(){
    PSaltos.push(CuadruplosCount);
}

void ACTION_2027(){
    int expr = POperandos.top();
    POperandos.pop();

    if(expr!=BOOLEANO){
        cout<<"DO -> no se esta evaluando una expresion booleana valida"<<endl;
    }
    else{
        int salto = PSaltos.top();
        PSaltos.pop();

        generarCuadruplo(GOTOVERDADERO,expr,0,salto);

        if(esAvail(expr)){
            liberarAvail(expr);
        }
    }
}

//EST_IF
void ACTION_2028(){

    int aux = PTipos.top();
    PTipos.pop();

    if(aux!=BOOLEANO){
        cout<<"IF -> no se esta evaluando una expresion booleana valida"<<endl;
    }
    else{
       int resultado = POperandos.top();
        POperandos.pop();
        generarCuadruplo(GOTOFALSO,resultado,0,0);

        PSaltos.push((CuadruplosCount-1));

        if(esAvail(resultado)){
            liberarAvail(resultado);
        }

    }
}

void ACTION_2029(){

    generarCuadruplo(GOTO,0,0,0);
    int falso = PSaltos.top();
    PSaltos.pop();

    rellenar(falso,CuadruplosCount);
    PSaltos.push(CuadruplosCount - 1);

}

void ACTION_2030(){
    int  fin = PSaltos.top();
    PSaltos.pop();

    rellenar(fin,CuadruplosCount);
}


void ejecutarAccion(int accion,int stkn, lex_entry *l){
    switch (accion) {
    case 2001:
        ACTION_2001(l->lexema);
        break;
    case 2002:
        ACTION_2002();
        break;
    case 2003:
        ACTION_2003();
        break;
    case 2004:
        ACTION_2004();
        break;
    case 2005:
        ACTION_2005(stkn);
        break;
    case 2006:
        ACTION_2006(stkn);
        break;
    case 2007:
        ACTION_2007(stkn,l->lexema);
        break;
    case 2008:
        ACTION_2008(stkn);
        break;
    case 2009:
        ACTION_2009();
        break;
    case 2010:
        ACTION_2010();
        break;
    case 2011:
        ACTION_2011();
        break;
    case 2012:
        ACTION_2012(stkn);
        break;
    case 2013:
        ACTION_2013();
        break;
    case 2014:
        ACTION_2014();
        break;
    case 2015:
        ACTION_2015();
        break;
    case 2016:
        ACTION_2016();
        break;
    case 2017:
        ACTION_2017(stkn);
        break;
    case 2018:
        ACTION_2018(l->lexema);
        break;
    case 2019:
        ACTION_2019();
        break;
    case 2020:
        ACTION_2020(l->lexema);
        break;
    case 2021:
        ACTION_2021();
        break;
    case 2022:
        ACTION_2022();
        break;
    case 2023:
        ACTION_2023();
        break;
    case 2024:
        ACTION_2024();
        break;
    case 2025:
        ACTION_2025();
        break;
    case 2026:
        ACTION_2026();
        break;
    case 2027:
        ACTION_2027();
        break;
    case 2028:
        ACTION_2028();
        break;
    case 2029:
        ACTION_2029();
        break;
    case 2030:
        ACTION_2030();
        break;
    }
}

#endif // TRADUCTOR_H
