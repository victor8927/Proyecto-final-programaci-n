#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAX_VEHICULOS 100
#define CLEAR "cls"
#define REVISIONES_REQUERIDAS 3

const char *tiposVehiculos[] = {"Automovil", "Moto", "Bus", "Trailer"};

typedef struct {
	char propietario[50];
	char placa[10];
	char cedula[11];
	int anio;
	int tipo; // 0: Automovil, 1: Moto, 2: Bus, 3: Trailer
	float avaluo;
	int revisiones; // entre 0 y 3
} Vehiculo;

// Prototipos
void limpiarPantalla();
void esperar();
void pausar();
void inicializarVehiculos(Vehiculo[], int);
void registrarVehiculo(Vehiculo[], int *);
int validarNombre(const char *);
int validarCedula(const char *);
int validarPlaca(const char *);
int validarTipo(int);
float calcularMulta(int);
float calcularDescuento(float);
void calcularMatricula(float *, int);
void mostrarComprobante(const Vehiculo *, float, float, float);
void generarComprobanteArchivo(const Vehiculo *, float, float, float);
void mostrarTodosVehiculos(const Vehiculo[], int);
void buscarVehiculoPorPlaca(const Vehiculo[], int);
void registrarRevisionTecnica(Vehiculo[], int);
void guardarVehiculoEnArchivo(const Vehiculo *);
void cargarVehiculosDesdeArchivo(Vehiculo[], int *);

void limpiarPantalla() { system(CLEAR); }
void esperar() { Sleep(2000); }
void pausar() { printf("\nPresione Enter para continuar..."); getchar(); }

void inicializarVehiculos(Vehiculo vehiculos[], int len) {
	for (int i = 0; i < len; i++) {
		vehiculos[i].propietario[0] = '\0';
		vehiculos[i].placa[0] = '\0';
		vehiculos[i].cedula[0] = '\0';
		vehiculos[i].anio = 0;
		vehiculos[i].tipo = -1;
		vehiculos[i].avaluo = 0.0f;
		vehiculos[i].revisiones = 0;
	}
}

int validarNombre(const char *nombre) {
	for (int i = 0; nombre[i]; i++) {
		if (!isalpha(nombre[i]) && nombre[i] != ' ') return 0;
	}
	return strlen(nombre) > 0;
}

int validarCedula(const char *cedula) {
	if (strlen(cedula) != 10) return 0;
	for (int i = 0; i < 10; i++) {
		if (!isdigit(cedula[i])) return 0;
	}
	return 1;
}

int validarPlaca(const char *placa) {
	return strlen(placa) == 8 && isupper(placa[0]) && isupper(placa[1]) && isupper(placa[2]) && placa[3] == '-' && isdigit(placa[4]) && isdigit(placa[5]) && isdigit(placa[6]) && isdigit(placa[7]);
}

int validarTipo(int tipo) {
	return tipo >= 0 && tipo <= 3;
}

void registrarVehiculo(Vehiculo vehiculos[], int *numVehiculos) {
	Vehiculo nuevo;
	limpiarPantalla();
	
	do {
		printf("Ingrese nombre del propietario (solo letras): ");
		fgets(nuevo.propietario, sizeof(nuevo.propietario), stdin);
		nuevo.propietario[strcspn(nuevo.propietario, "\n")] = '\0';
		if (!validarNombre(nuevo.propietario)) {
			printf("\n[Error] Nombre invalido. Debe contener solo letras.\n"); esperar(); limpiarPantalla();
		} else break;
	} while (1);
	
	do {
		printf("Ingrese cedula del propietario (10 digitos): ");
		scanf("%s", nuevo.cedula);
		if (!validarCedula(nuevo.cedula)) {
			printf("\n[Error] Cedula invalida.\n"); esperar(); limpiarPantalla();
		} else break;
	} while (1);
	
	do {
		printf("Ingrese placa del vehiculo (formato ABC-1234): ");
		scanf("%s", nuevo.placa);
		if (!validarPlaca(nuevo.placa)) {
			printf("\n[Error] Placa invalida.\n"); esperar(); limpiarPantalla();
		} else break;
	} while (1);
	
	do {
		printf("Ingrese anio del vehiculo (1900-2025): ");
		if (scanf("%d", &nuevo.anio) != 1 || nuevo.anio < 1900 || nuevo.anio > 2025) {
			printf("\n[Error] Anio invalido.\n"); while (getchar() != '\n'); esperar(); limpiarPantalla();
		} else break;
	} while (1);
	
	do {
		printf("Seleccione tipo de vehiculo:\n0. Automovil\n1. Moto\n2. Bus\n3. Trailer\nIngrese opcion: ");
		if (scanf("%d", &nuevo.tipo) != 1 || !validarTipo(nuevo.tipo)) {
			printf("\n[Error] Tipo invalido.\n"); while (getchar() != '\n'); esperar(); limpiarPantalla();
		} else break;
	} while (1);
	
	do {
		printf("Ingrese avaluo (>0): ");
		if (scanf("%f", &nuevo.avaluo) != 1 || nuevo.avaluo <= 0) {
			printf("\n[Error] Avaluo invalido.\n"); while (getchar() != '\n'); esperar(); limpiarPantalla();
		} else break;
	} while (1);
	while (getchar() != '\n');
	
	nuevo.revisiones = 0;
	calcularMatricula(&nuevo.avaluo, nuevo.anio);
	vehiculos[(*numVehiculos)++] = nuevo;
	guardarVehiculoEnArchivo(&nuevo);
	printf("\n[Registro Exitoso]\n");
	pausar();
}

float calcularMulta(int anio) {
	if (anio < 2000) return 150.0f;
	else if (anio < 2010) return 75.0f;
	return 0.0f;
}

float calcularDescuento(float avaluo) {
	if (avaluo > 30000) return 0.10f * avaluo;
	else if (avaluo > 15000) return 0.05f * avaluo;
	return 0.0f;
}

void calcularMatricula(float *avaluo, int anio) {
	if (anio < 2015) *avaluo *= 0.85f;
	else if (anio <= 2022) *avaluo *= 0.95f;
}

void mostrarComprobante(const Vehiculo *v, float multa, float descuento, float total) {
	printf("\n--- COMPROBANTE DE MATRICULA ---\n");
	printf("Propietario: %s\nPlaca: %s\nAvaluo Base: %.2f\nTipo: %s\nMulta: %.2f\nDescuento: %.2f\nTOTAL A PAGAR: %.2f\n",
		   v->propietario, v->placa, v->avaluo, tiposVehiculos[v->tipo], multa, descuento, total);
}

void generarComprobanteArchivo(const Vehiculo *v, float multa, float descuento, float total) {
	FILE *f = fopen("comprobante.txt", "w");
	if (!f) return;
	fprintf(f, "COMPROBANTE DE MATRICULA\nPropietario: %s\nPlaca: %s\nTipo: %s\nAvaluo Base: %.2f\nMulta: %.2f\nDescuento: %.2f\nTOTAL A PAGAR: %.2f\n",
			v->propietario, v->placa, tiposVehiculos[v->tipo], v->avaluo, multa, descuento, total);
	fclose(f);
}

void guardarVehiculoEnArchivo(const Vehiculo *v) {
	FILE *archivo = fopen("vehiculos.txt", "a");
	if (!archivo) return;
	fprintf(archivo, "%s|%s|%s|%d|%d|%.2f|%d\n",
			v->propietario, v->placa, v->cedula, v->anio, v->tipo, v->avaluo, v->revisiones);
	fclose(archivo);
}

void cargarVehiculosDesdeArchivo(Vehiculo vehiculos[], int *numVehiculos) {
	FILE *archivo = fopen("vehiculos.txt", "r");
	if (!archivo) return;
	Vehiculo v;
	while (fscanf(archivo, "%49[^|]|%9[^|]|%10[^|]|%d|%d|%f|%d\n",
				  v.propietario, v.placa, v.cedula, &v.anio, &v.tipo, &v.avaluo, &v.revisiones) == 7) {
		vehiculos[(*numVehiculos)++] = v;
	}
	fclose(archivo);
}

void mostrarTodosVehiculos(const Vehiculo vehiculos[], int numVehiculos) {
	for (int i = 0; i < numVehiculos; i++) {
		printf("\nPropietario: %s\nPlaca: %s\nCedula: %s\nAnio: %d\nTipo: %s\nAvaluo: %.2f\nRevisiones: %d\n",
			   vehiculos[i].propietario, vehiculos[i].placa, vehiculos[i].cedula,
			   vehiculos[i].anio, tiposVehiculos[vehiculos[i].tipo], vehiculos[i].avaluo, vehiculos[i].revisiones);
	}
	pausar();
}

void buscarVehiculoPorPlaca(const Vehiculo vehiculos[], int numVehiculos) {
	char placa[10];
	printf("Ingrese la placa a buscar: ");
	scanf("%s", placa);
	for (int i = 0; i < numVehiculos; i++) {
		if (strcmp(vehiculos[i].placa, placa) == 0) {
			printf("\nPropietario: %s\nPlaca: %s\nCedula: %s\nAnio: %d\nTipo: %s\nAvaluo: %.2f\nRevisiones: %d\n",
				   vehiculos[i].propietario, vehiculos[i].placa, vehiculos[i].cedula,
				   vehiculos[i].anio, tiposVehiculos[vehiculos[i].tipo], vehiculos[i].avaluo, vehiculos[i].revisiones);
			pausar(); return;
		}
	}
	printf("Vehiculo no encontrado.\n");
	pausar();
}

void registrarRevisionTecnica(Vehiculo vehiculos[], int numVehiculos) {
	char placa[10];
	printf("Ingrese la placa del vehiculo para registrar revision: ");
	scanf("%s", placa);
	for (int i = 0; i < numVehiculos; i++) {
		if (strcmp(vehiculos[i].placa, placa) == 0) {
			if (vehiculos[i].revisiones >= REVISIONES_REQUERIDAS) {
				printf("\nYa se han registrado las 3 revisiones anuales.\n");
			} else {
				vehiculos[i].revisiones++;
				printf("\nRevision registrada exitosamente. Total: %d/3\n", vehiculos[i].revisiones);
			}
			pausar(); return;
		}
	}
	printf("Vehiculo no encontrado.\n");
	pausar();
}

int main() {
	Vehiculo vehiculos[MAX_VEHICULOS];
	int numVehiculos = 0, opcion;
	inicializarVehiculos(vehiculos, MAX_VEHICULOS);
	cargarVehiculosDesdeArchivo(vehiculos, &numVehiculos);
	
	do {
		limpiarPantalla();
		printf("--- MENU PRINCIPAL ---\n");
		printf("1. Registrar vehiculo\n");
		printf("2. Mostrar todos los vehiculos\n");
		printf("3. Calcular valor a pagar (con multas y descuentos)\n");
		printf("4. Buscar vehiculo por placa\n");
		printf("5. Registrar revision tecnica\n");
		printf("6. Salir\n");
		printf("Seleccione una opcion (1-6): ");
		if (scanf("%d", &opcion) != 1 || opcion < 1 || opcion > 6) {
			while (getchar() != '\n');
			printf("\n[Error] Opcion invalida.\n"); esperar(); continue;
		}
		while (getchar() != '\n');
		
		switch (opcion) {
		case 1: registrarVehiculo(vehiculos, &numVehiculos); break;
		case 2: mostrarTodosVehiculos(vehiculos, numVehiculos); break;
		case 3:
			for (int i = 0; i < numVehiculos; i++) {
				float multa = calcularMulta(vehiculos[i].anio);
				float descuento = calcularDescuento(vehiculos[i].avaluo);
				float total = vehiculos[i].avaluo + multa - descuento;
				mostrarComprobante(&vehiculos[i], multa, descuento, total);
				generarComprobanteArchivo(&vehiculos[i], multa, descuento, total);
			}
			pausar(); break;
		case 4: buscarVehiculoPorPlaca(vehiculos, numVehiculos); break;
		case 5: registrarRevisionTecnica(vehiculos, numVehiculos); break;
		case 6: printf("Saliendo del sistema...\n"); break;
		}
	} while (opcion != 6);
	return 0;
}
