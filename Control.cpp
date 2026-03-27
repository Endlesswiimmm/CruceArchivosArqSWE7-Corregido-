/*
 * Programa : Cruce de archivos 
 * Objetivo : Actualizar el archivo de Personal con Movimientos
 * (altas, bajas y cambios) usando cruce de archivos.
 * Autor    : Andrea Escobar - Jesús Pérez - Adrián Vázquez - Gabriel Ortiz
 * Fecha    : 26 de Marzo de 2026
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

/* --- CONSTANTES --- */
const string FIN_ARCHIVO = "9999";
const string SIN_CAMBIO  = "0";
const double SIN_SALARIO = 0.0;
const int    SIN_FECHA   = 0;

// Constantes y variables para el control del reporte
const string NOMBRE_PROGRAMA = "PROG0001"; 
const int MAX_LINEAS_PAGINA = 40;          
int LineaActual = 99;                      
int HojaActual = 1;                        

/* --- ESTRUCTURAS --- */
struct RegistroPersonal {
    string Trabajador   = "";
    string Grupo        = "";
    string Empresa      = "";
    string Planta       = "";
    string Departamento = "";
    char   CveOE        = ' ';
    string Nombre       = "";
    double SalarioBase  = 0.0;
    int    FechaIngreso = 0;
};

struct RegistroMovimiento {
    char   CveMovimiento = ' ';
    string Trabajador    = "";
    string Grupo         = "";
    string Empresa       = "";
    string Planta        = "";
    string Departamento  = "";
    char   CveOE         = ' ';
    string Nombre        = "";
    double SalarioBase   = 0.0;
    int    FechaIngreso  = 0;
};

/* --- VARIABLES GLOBALES --- */
RegistroPersonal  RegPersonal;
RegistroMovimiento RegMovimiento;
RegistroPersonal  RegNuevoPersonal;

int FinPersonal    = 0;
int FinMovimientos = 0;

ifstream InPersonalFile;
ifstream InMovimientosFile;
ofstream OutNuevoPersonalFile;
ofstream OutReporteFile;

/* --- DECLARACIONES ANTICIPADAS --- */
void LeerRegistroPersonal();
void LeerRegistroMovimiento();
void InsertarRegistro();
void ImprimirEncabezado();
void GenerarReporte(string TipoMovimiento, bool Valido);
void ProcCopia();
void ProcAlta();
void ProcBaja();
void ProcCambio();
void ProcActualizacion();
void Inicio();
void Termina();

/* --- IMPLEMENTACIÓN DE MÓDULOS --- */

void LeerRegistroPersonal() {
    InPersonalFile >> RegPersonal.Trabajador;

    if (RegPersonal.Trabajador == FIN_ARCHIVO) {
        FinPersonal = 1;
    } else {
        InPersonalFile >> RegPersonal.Grupo
                       >> RegPersonal.Empresa
                       >> RegPersonal.Planta
                       >> RegPersonal.Departamento
                       >> RegPersonal.CveOE
                       >> RegPersonal.Nombre
                       >> RegPersonal.SalarioBase
                       >> RegPersonal.FechaIngreso;
    }
}

void LeerRegistroMovimiento() {
    InMovimientosFile >> RegMovimiento.CveMovimiento
                      >> RegMovimiento.Trabajador;

    if (RegMovimiento.Trabajador == FIN_ARCHIVO) {
        FinMovimientos = 1;
    } else {
        InMovimientosFile >> RegMovimiento.Grupo
                          >> RegMovimiento.Empresa
                          >> RegMovimiento.Planta
                          >> RegMovimiento.Departamento
                          >> RegMovimiento.CveOE
                          >> RegMovimiento.Nombre
                          >> RegMovimiento.SalarioBase
                          >> RegMovimiento.FechaIngreso;
    }
}

void InsertarRegistro() {
    OutNuevoPersonalFile << RegNuevoPersonal.Trabajador   << " "
                         << RegNuevoPersonal.Grupo        << " "
                         << RegNuevoPersonal.Empresa      << " "
                         << RegNuevoPersonal.Planta       << " "
                         << RegNuevoPersonal.Departamento << " "
                         << RegNuevoPersonal.CveOE        << " "
                         << RegNuevoPersonal.Nombre       << " "
                         << fixed << setprecision(2)
                         << RegNuevoPersonal.SalarioBase  << " "
                         << RegNuevoPersonal.FechaIngreso << endl;
}

void ImprimirEncabezado() {
    if (HojaActual > 1) {
        OutReporteFile << "\f"; 
    }

    // Línea 1
    OutReporteFile << left << setw(31) << NOMBRE_PROGRAMA 
                   << left << setw(44) << "ACTUALIZACION DEL ARCHIVO DE PERSONAL"
                   << "HOJA " << right << setw(4) << setfill('0') << HojaActual << setfill(' ') << endl;
    
    // Línea 2
    OutReporteFile << endl;
    
    // Línea 3
    OutReporteFile << left << setw(34) << "ACME - DIV. BANCOS" 
                   << "MOVIMIENTOS EFECTUADOS" << endl;
                   
    // Línea 4, 5, 6 y 7
    OutReporteFile << "PERSONAL" << endl;
    OutReporteFile << endl;
    OutReporteFile << "        TRABAJADOR          MOVIMIENTO EFECTUADO" << endl;
    OutReporteFile << "        ----------          --------------------" << endl;

    LineaActual = 7;
    HojaActual++;
}

void GenerarReporte(string TipoMovimiento, bool Valido) {
    if (LineaActual >= MAX_LINEAS_PAGINA) {
        ImprimirEncabezado();
    }

    string Mensaje = "";
    
    if (TipoMovimiento == "ALTA") {
        Mensaje = Valido ? "A L T A" : "ALTA NO EXITOSA";
    } 
    else if (TipoMovimiento == "BAJA") {
        Mensaje = Valido ? "B A J A" : "BAJA NO EXITOSA";
    } 
    else if (TipoMovimiento == "CAMBIO") {
        Mensaje = Valido ? "C A M B I O" : "CAMBIO NO EXITOSO";
    } 
    else {
        Mensaje = "ERROR DESCONOCIDO";
    }

    OutReporteFile << "        " 
                   << left << setw(20) << RegMovimiento.Trabajador 
                   << Mensaje 
                   << endl << endl; 

    LineaActual += 2;
}

void ProcCopia() {
    RegNuevoPersonal = RegPersonal;
    InsertarRegistro();
    LeerRegistroPersonal();
}

void ProcAlta() {
    RegNuevoPersonal.Trabajador   = RegMovimiento.Trabajador;
    RegNuevoPersonal.Grupo        = (RegMovimiento.Grupo == SIN_CAMBIO) ? "0000" : RegMovimiento.Grupo;
    RegNuevoPersonal.Empresa      = (RegMovimiento.Empresa == SIN_CAMBIO)? "0000" : RegMovimiento.Empresa;
    RegNuevoPersonal.Planta       = (RegMovimiento.Planta == SIN_CAMBIO) ? "0000" : RegMovimiento.Planta;
    RegNuevoPersonal.Departamento = (RegMovimiento.Departamento == SIN_CAMBIO) ? "0000" : RegMovimiento.Departamento;
    RegNuevoPersonal.CveOE        = (RegMovimiento.CveOE == '0') ? 'O' : RegMovimiento.CveOE;
    RegNuevoPersonal.Nombre       = (RegMovimiento.Nombre == SIN_CAMBIO) ? "DESCONOCIDO" : RegMovimiento.Nombre;
    RegNuevoPersonal.SalarioBase  = (RegMovimiento.SalarioBase == SIN_SALARIO) ? 0.0 : RegMovimiento.SalarioBase;
    RegNuevoPersonal.FechaIngreso = (RegMovimiento.FechaIngreso == SIN_FECHA) ? 0 : RegMovimiento.FechaIngreso;

    InsertarRegistro();
    GenerarReporte("ALTA", true);
    LeerRegistroMovimiento();
}

void ProcBaja() {
    GenerarReporte("BAJA", true);
    LeerRegistroMovimiento();
    LeerRegistroPersonal();
}

void ProcCambio() {
    RegNuevoPersonal.Trabajador   = RegPersonal.Trabajador;
    RegNuevoPersonal.Grupo        = (RegMovimiento.Grupo == SIN_CAMBIO) ? RegPersonal.Grupo : RegMovimiento.Grupo;
    RegNuevoPersonal.Empresa      = (RegMovimiento.Empresa == SIN_CAMBIO) ? RegPersonal.Empresa : RegMovimiento.Empresa;
    RegNuevoPersonal.Planta       = (RegMovimiento.Planta == SIN_CAMBIO) ? RegPersonal.Planta : RegMovimiento.Planta;
    RegNuevoPersonal.Departamento = (RegMovimiento.Departamento == SIN_CAMBIO) ? RegPersonal.Departamento : RegMovimiento.Departamento;
    RegNuevoPersonal.CveOE        = (RegMovimiento.CveOE == '0') ? RegPersonal.CveOE : RegMovimiento.CveOE;
    RegNuevoPersonal.Nombre       = (RegMovimiento.Nombre == SIN_CAMBIO) ? RegPersonal.Nombre : RegMovimiento.Nombre;
    RegNuevoPersonal.SalarioBase  = (RegMovimiento.SalarioBase == SIN_SALARIO) ? RegPersonal.SalarioBase : RegMovimiento.SalarioBase;
    RegNuevoPersonal.FechaIngreso = (RegMovimiento.FechaIngreso == SIN_FECHA) ? RegPersonal.FechaIngreso : RegMovimiento.FechaIngreso;

    InsertarRegistro();
    GenerarReporte("CAMBIO", true);
    LeerRegistroMovimiento();
    LeerRegistroPersonal();
}

void ProcActualizacion() {
    if (RegMovimiento.Trabajador == RegPersonal.Trabajador) {

        switch (RegMovimiento.CveMovimiento) {
            case 'A':
                GenerarReporte("ALTA", false);
                ProcCopia();
                LeerRegistroMovimiento();
                break;
            case 'B':
                ProcBaja();
                break;
            case 'C':
                ProcCambio();
                break;
            default:
                GenerarReporte("DESCONOCIDO", false);
                LeerRegistroMovimiento();
                LeerRegistroPersonal();
                break;
        }

    } else if (RegMovimiento.Trabajador < RegPersonal.Trabajador) {

        switch (RegMovimiento.CveMovimiento) {
            case 'A':
                ProcAlta();
                break;
            case 'B':
                GenerarReporte("BAJA", false);
                LeerRegistroMovimiento();
                break;
            case 'C':
                GenerarReporte("CAMBIO", false);
                LeerRegistroMovimiento();
                break;
            default:
                GenerarReporte("DESCONOCIDO", false);
                LeerRegistroMovimiento();
                break;
        }

    } else {
        ProcCopia();
    }
}

void Inicio() {
    InPersonalFile.open("per.txt", ios::in);
    if (!InPersonalFile) {
        cerr << "ERROR: No se pudo abrir el archivo Personal (per.txt)." << endl;
        exit(-1);
    }

    InMovimientosFile.open("mov.txt", ios::in);
    if (!InMovimientosFile) {
        cerr << "ERROR: No se pudo abrir el archivo Movimientos (mov.txt)." << endl;
        exit(-1);
    }

    OutNuevoPersonalFile.open("np.txt", ios::out);
    if (!OutNuevoPersonalFile) {
        cerr << "ERROR: No se pudo crear el archivo NuevoPersonal (np.txt)." << endl;
        exit(-1);
    }

    OutReporteFile.open("reporte.txt", ios::out);
    if (!OutReporteFile) {
        cerr << "ERROR: No se pudo crear el archivo Reporte (reporte.txt)." << endl;
        exit(-1);
    }

    LeerRegistroPersonal();
    LeerRegistroMovimiento();
}

void Termina() {
    InPersonalFile.close();
    InMovimientosFile.close();
    OutNuevoPersonalFile.close();
    OutReporteFile.close();
}

int main() {
    Inicio();

    while (!(FinPersonal && FinMovimientos)) {
        ProcActualizacion();
    }

    Termina();

    return 0;
}