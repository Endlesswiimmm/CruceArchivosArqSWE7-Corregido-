/*
 * Programa : Cruce de archivos 
 * Objetivo : Actualizar el archivo de Personal con Movimientos
 * (altas, bajas y cambios) usando cruce de archivos.
 * Autor    : Andrea Escobar - Jesús Pérez - Adrián Vázquez - Gabriel Ortiz
 * Fecha    : 26 de Marzo de 2026
 */

/*
 * Módulo              Objetivo
 * Control             Módulo principal. Orquesta el cruce de archivos.
 * Inicio              Abrir archivos Personal, Movimientos y NuevoPersonal.
 * ProcActualizacion   Dirigir alta, baja o cambio según clave de movimiento.
 * ProcAlta            Dar de alta un trabajador nuevo en el archivo.
 * ProcBaja            Dar de baja un trabajador existente.
 * ProcCambio          Aplicar cambios por excepción a un registro.
 * ProcCopia           Copiar registro de Personal sin movimiento.
 * ImprimirEncabezado  Dibuja el formato del reporte y maneja las hojas.
 * GenerarReporte      Escribir línea en reporte (exitoso o inválido).
 * Termina             Cerrar archivos y terminar programa.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

/*
 * FIN_ARCHIVO : Centinela de fin de archivo en los registros.
 * SIN_CAMBIO  : Valor que indica "no modificar" en campos string de movimientos tipo Cambio.
 * SIN_SALARIO : Centinela numérico para salario sin cambio en movimiento Cambio.
 * SIN_FECHA   : Centinela numérico para fecha sin cambio en movimiento Cambio.
 */
const string FIN_ARCHIVO = "9999";
const string SIN_CAMBIO  = "0";
const double SIN_SALARIO = 0.0;
const int    SIN_FECHA   = 0;

/* --- Variables para el control del reporte --- */
const string NOMBRE_PROGRAMA = "PROG0001"; // Constante del programa
const int MAX_LINEAS_PAGINA = 40;          // Límite antes de saltar a otra hoja
int LineaActual = 99;                      // Inicia alto para forzar el 1er encabezado
int HojaActual = 1;                        // Contador de hojas

/*
 * RegistroPersonal : Estructura del archivo maestro Personal.
 * Trabajador       : No. de trabajador (llave primaria).
 * Grupo            : Grupo al que pertenece el trabajador.
 * Empresa          : Empresa del trabajador.
 * Planta           : Planta asignada.
 * Departamento     : Departamento asignado.
 * CveOE            : Clave Obrero/Empleado ('O'=Obrero, 'E'=Empleado).
 * Nombre           : Nombre completo del trabajador.
 * SalarioBase      : Salario base del trabajador.
 * FechaIngreso     : Fecha de ingreso en formato AAAAMMDD.
 */
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

/*
 * RegistroMovimiento : Estructura del archivo de Movimientos.
 * CveMovimiento      : Clave del movimiento ('A'=Alta,'B'=Baja,'C'=Cambio).
 * Trabajador         : No. de trabajador (llave primaria).
 * [resto de campos igual que RegistroPersonal, usados según tipo de movimiento]
 */
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

/*
 * RegPersonal      : Registro activo leído del archivo Personal.
 * RegMovimiento    : Registro activo leído del archivo Movimientos.
 * RegNuevoPersonal : Registro listo para escribirse en NuevoPersonal.
 * FinPersonal      : Bandera fin de archivo Personal (0=activo, 1=fin).
 * FinMovimientos   : Bandera fin de archivo Movimientos (0=activo, 1=fin).
 */
RegistroPersonal  RegPersonal;
RegistroMovimiento RegMovimiento;
RegistroPersonal  RegNuevoPersonal;

int FinPersonal    = 0;
int FinMovimientos = 0;

ifstream InPersonalFile;
ifstream InMovimientosFile;
ofstream OutNuevoPersonalFile;
ofstream OutReporteFile;

// Declaraciones anticipadas
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

/*
 * Módulo  : LeerRegistroPersonal
 * Objetivo: Leer un registro del archivo Personal.
 * Al detectar el centinela FIN_ARCHIVO activa FinPersonal.
 */
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

/*
 * Módulo  : LeerRegistroMovimiento
 * Objetivo: Leer un registro del archivo Movimientos.
 * Al detectar el centinela FIN_ARCHIVO activa FinMovimientos.
 */
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

/*
 * Módulo  : InsertarRegistro
 * Objetivo: Escribir RegNuevoPersonal en el archivo NuevoPersonal.
 */
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

/*
 * Módulo  : ImprimirEncabezado
 * Objetivo: Imprimir el formato superior del reporte y manejar la paginación.
 */
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

/*
 * Módulo  : GenerarReporte
 * Objetivo: Escribir una línea en el reporte indicando el trabajador,
 * el tipo de movimiento y si fue válido o inválido, con doble espacio.
 */
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

/*
 * Módulo  : ProcCopia
 * Objetivo: Copiar el registro de Personal tal como está a NuevoPersonal.
 * Se usa cuando no hay movimiento para ese trabajador (M > P).
 * Avanza el puntero del archivo Personal.
 */
void ProcCopia() {
    RegNuevoPersonal = RegPersonal;
    InsertarRegistro();
    LeerRegistroPersonal();
}

/*
 * Módulo  : ProcAlta
 * Objetivo: Dar de alta un trabajador nuevo en el archivo de Personal.
 * El trabajador NO existe en Personal (M < P), por lo que el
 * alta es válida. Se inserta el registro y se reporta.
 * Avanza el puntero del archivo Movimientos.
 */
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

/*
 * Módulo  : ProcBaja
 * Objetivo: Dar de baja un trabajador existente.
 * El trabajador SÍ existe en Personal (M == P).
 * No se copia a NuevoPersonal (el trabajador queda eliminado).
 * Avanza los punteros de ambos archivos.
 */
void ProcBaja() {
    GenerarReporte("BAJA", true);
    LeerRegistroMovimiento();
    LeerRegistroPersonal();
}

/*
 * Módulo  : ProcCambio
 * Objetivo: Aplicar cambios por excepción a un registro existente.
 * El trabajador SÍ existe en Personal (M == P).
 * Solo se actualizan los campos que difieren de SIN_CAMBIO.
 * Avanza los punteros de ambos archivos.
 */
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

/*
 * Módulo  : ProcActualizacion
 * Objetivo: Dirigir alta, baja o cambio según la clave de movimiento y
 * la comparación entre los trabajadores de ambos archivos.
 *
 * M == P : Trabajador encontrado en ambos archivos.
 * Alta → inválida (ya existe). Baja/Cambio → válidos.
 * M < P  : Movimiento sin registro en Personal.
 * Alta → válida (nuevo trabajador). Baja/Cambio → inválidos.
 * M > P  : Registro en Personal sin movimiento → ProcCopia.
 */
void ProcActualizacion() {
    if (RegMovimiento.Trabajador == RegPersonal.Trabajador) {

        switch (RegMovimiento.CveMovimiento) {
            case 'A':
                // Alta inválida: el trabajador ya existe en Personal
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
                // Baja inválida: el trabajador no existe en Personal
                GenerarReporte("BAJA", false);
                LeerRegistroMovimiento();
                break;

            case 'C':
                // Cambio inválido: el trabajador no existe en Personal
                GenerarReporte("CAMBIO", false);
                LeerRegistroMovimiento();
                break;

            default:
                GenerarReporte("DESCONOCIDO", false);
                LeerRegistroMovimiento();
                break;
        }

    } else {
        // M > P: registro en Personal sin movimiento, copiar directo
        ProcCopia();
    }
}

/*
 * Módulo  : Inicio
 * Objetivo: Abrir archivos Personal, Movimientos, NuevoPersonal y Reporte.
 * Leer el primer registro de Personal y Movimientos.
 */
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

/*
 * Módulo  : Termina
 * Objetivo: Cerrar todos los archivos abiertos y terminar el programa.
 */
void Termina() {
    InPersonalFile.close();
    InMovimientosFile.close();
    OutNuevoPersonalFile.close();
    OutReporteFile.close();
}

/*
 * Módulo  : Control (main)
 * Objetivo: Módulo principal. Orquesta el cruce de archivos.
 * Invoca Inicio, ejecuta el ciclo de cruce y llama Termina.
 */
int main() {
    Inicio();

    while (!(FinPersonal && FinMovimientos)) {
        ProcActualizacion();
    }

    Termina();

    return 0;
}