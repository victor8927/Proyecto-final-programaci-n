#ifndef VEHICULO_H
#define VEHICULO_H

#define MAX_VEHICULOS 100
#define REVISIONES_REQUERIDAS 3

// Declaración externa de tiposVehiculos (definida en vehiculo.c)
extern const char *tiposVehiculos[];

typedef struct {
	char propietario[50];
	char placa[10];
	char cedula[11];
	int anio;
	int tipo; // 1: Automovil, 2: Moto, 3: Bus, 4: Trailer
	float avaluo;
	int revisiones;
} Vehiculo;

// Prototipos de funciones relacionadas con vehículos
void inicializarVehiculos(Vehiculo[], int);
void registrarVehiculo(Vehiculo[], int *);
int validarNombre(const char *);
int validarCedula(const char *);
int validarPlaca(const char *);
int validarTipo(int);
float obtenerTopePorTipo(int);
float calcularMulta(int);
float calcularDescuento(float);
void calcularMatricula(float *, int);
void mostrarTodosVehiculos(const Vehiculo[], int);
void buscarVehiculoPorPlaca(const Vehiculo[], int);
void registrarRevisionTecnica(Vehiculo[], int);
void guardarVehiculoEnArchivo(const Vehiculo *);
void cargarVehiculosDesdeArchivo(Vehiculo[], int *);

#endif // VEHICULO_H
