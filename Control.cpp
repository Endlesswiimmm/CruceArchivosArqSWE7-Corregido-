//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

const int MAX_ELEM = 50;
const string FIN_ARCHIVO = "9999";

//
void reportarMovInvalido(){

}

//
void insertRegistro(){

}

//
void leerRegistroMov(){
    
}

//
void leerRegistroPer(){
    
}

//
void leerArchivoPer(){

}

//
void generarReporte(){

}

//

void procActualizacion(){
    if (trab.M == trab.P){

        switch (opt){
        case "A":
            procCopia();
            reportarMovInvalido();
            break;

        case "B":
            procBaja();
            break;

        case "C":
            procCambio();
            break;

        default:
            break;
        }
            
    } else if (trab.M > trab.P){

        switch (opt){
        case "A":
            procAlta();
            reportarMovInvalido();
            break;

        case "B":
            reportarMovInvalido();
            leerRegistroMov();
            break;

        case "C":
            reportarMovInvalido();
            leerRegistroMov();
            break;

        default:
            break;
        }

    } else {
        reportarMovInvalido();
    }
    leerRegistroMov();
    leerRegistroPer();
}

//
void procBaja(){

}
//
void procAlta(){

}

//
void procCambio(){

}

//
void procCopia(){

}