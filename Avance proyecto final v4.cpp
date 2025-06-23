#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

#define MAX_VEHICULOS 100

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

typedef struct {
	char propietario[50];
	char placa[10];
	int anio;
	char tipo[20];
	float avaluo;
} Vehiculo;

// Prototipos
void inicializarVehiculos(Vehiculo vehiculos[], int len);
void registrarVehiculo(Vehiculo vehiculos[], int *numVehiculos);
void mostrarVehiculo(const Vehiculo *v);
void mostrarTodosVehiculos(const Vehiculo vehiculos[], int numVehiculos);
void pausar();
void mostrarDatos(char propietario[], char placa[], float valorBase, int anio);
void calcularMatricula(float *valorBase, int anio);
int validarNombre(char *nombre);
int validarPlaca(char *placa);
void limpiarPantalla();
void esperar();
void guardarVehiculoEnArchivo(const Vehiculo *v);
void cargarVehiculosDesdeArchivo(Vehiculo vehiculos[], int *numVehiculos);
float calcularMulta(int anio);
float calcularDescuento(float avaluo);

void inicializarVehiculos(Vehiculo vehiculos[], int len) {
	for (int i = 0; i < len; i++) {
		strcpy(vehiculos[i].propietario, "");
		strcpy(vehiculos[i].placa, "");
		vehiculos[i].anio = 0;
		strcpy(vehiculos[i].tipo, "");
		vehiculos[i].avaluo = 0.0f;
	}
}

void registrarVehiculo(Vehiculo vehiculos[], int *numVehiculos) {
	if (*numVehiculos >= MAX_VEHICULOS) {
		limpiarPantalla();
		printf("\n¡Capacidad maxima alcanzada!\n");
		esperar();
		return;
	}
	
	Vehiculo nuevo;
	
	do {
		printf("Ingrese el nombre del propietario: ");
		fgets(nuevo.propietario, sizeof(nuevo.propietario), stdin);
		nuevo.propietario[strcspn(nuevo.propietario, "\n")] = '\0';
		if (!validarNombre(nuevo.propietario)) {
			limpiarPantalla();
			printf("Error: El nombre no debe contener numeros.\n");
			esperar();
			system(CLEAR);
		} else {
			break;
		}
	} while (1);
	
	do {
		printf("Ingrese la placa del vehiculo: ");
		scanf("%s", nuevo.placa);
		if (!validarPlaca(nuevo.placa)) {
			limpiarPantalla();
			printf("Error: Placa invalida.\n");
			esperar();
			system(CLEAR);
		} else {
			break;
		}
	} while (1);
	
	do {
		printf("Ingrese el ava1uo del vehiculo (1900-2025): ");
		if (scanf("%d", &nuevo.anio) != 1 || nuevo.anio < 1900 || nuevo.anio > 2025) {
			limpiarPantalla();
			printf("Error: Ava1uo fuera de rango o invalido.\n");
			while (getchar() != '\n');
			esperar();
			system(CLEAR);
		} else {
			break;
		}
	} while (1);
	
	printf("Ingrese tipo de vehiculo (e.j., Automovil, Moto): ");
	scanf("%s", nuevo.tipo);
	
	do {
		printf("Ingrese avaluo base (>0): ");
		if (scanf("%f", &nuevo.avaluo) != 1 || nuevo.avaluo <= 0) {
			limpiarPantalla();
			printf("Error: Avalauo invalido.\n");
			while (getchar() != '\n');
			esperar();
			system(CLEAR);
		} else {
			break;
		}
	} while (1);
	
	while (getchar() != '\n');
	
	calcularMatricula(&nuevo.avaluo, nuevo.anio);
	
	vehiculos[*numVehiculos] = nuevo;
	(*numVehiculos)++;
	guardarVehiculoEnArchivo(&nuevo);
	
	printf("\nVehiculo registrado exitosamente.\n");
	mostrarDatos(nuevo.propietario, nuevo.placa, nuevo.avaluo, nuevo.anio);
	printf("Total base (sin multas/descuentos): %.2f\n", nuevo.avaluo);
}

void mostrarVehiculo(const Vehiculo *v) {
	printf("\n--- Datos del Vehiculo ---\n");
	printf("Propietario: %s\n", v->propietario);
	printf("Placa: %s\n", v->placa);
	printf("Avaluo: %d\n", v->anio);
	printf("Tipo: %s\n", v->tipo);
	printf("Avaluo: %.2f\n", v->avaluo);
}

void mostrarTodosVehiculos(const Vehiculo vehiculos[], int numVehiculos) {
	if (numVehiculos == 0) {
		printf("No hay vehiculos registrados.\n");
		return;
	}
	for (int i = 0; i < numVehiculos; i++) {
		mostrarVehiculo(&vehiculos[i]);
	}
}

void mostrarDatos(char propietario[], char placa[], float valorBase, int anio) {
	printf("\n--- DATOS DEL VEHICULO ---\n");
	printf("Propietario: %s\n", propietario);
	printf("Placa: %s\n", placa);
	printf("Avaluo: %d\n", anio);
	printf("Valor base: %.2f\n", valorBase);
}

void calcularMatricula(float *valorBase, int anio) {
	if (anio < 2015) {
		*valorBase *= 0.85f;
	} else if (anio <= 2022) {
		*valorBase *= 0.95f;
	}
}

int validarNombre(char *nombre) {
	for (int i = 0; nombre[i] != '\0'; i++) {
		if (isdigit((unsigned char)nombre[i])) return 0;
	}
	return 1;
}

int validarPlaca(char *placa) {
	int len = strlen(placa);
	if (len != 7 && len != 8) return 0;
	for (int i = 0; i < 3; i++) {
		if (!isupper((unsigned char)placa[i]) || !isalpha((unsigned char)placa[i])) return 0;
	}
	if (placa[3] != '-') return 0;
	for (int i = 4; i < len; i++) {
		if (!isdigit((unsigned char)placa[i])) return 0;
	}
	return 1;
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

void pausar() {
	printf("\nPresione Enter para volver al menu...");
	getchar();
}

void limpiarPantalla() {
	system(CLEAR);
}

void esperar() {
#ifdef _WIN32
	Sleep(2000);
#else
	sleep(2);
#endif
}

void guardarVehiculoEnArchivo(const Vehiculo *v) {
	char ruta[260];
	const char *userProfile = getenv("USERPROFILE");
	if (userProfile != NULL) {
		snprintf(ruta, sizeof(ruta), "%s\\Downloads\\vehiculos.txt", userProfile);
	} else {
		strcpy(ruta, "vehiculos.txt");
	}
	
	FILE *archivo = fopen(ruta, "a");
	if (archivo == NULL) {
		perror("No se pudo abrir el archivo para guardar el vehiculo");
		return;
	}
	
	fprintf(archivo, "%s|%s|%d|%s|%.2f\n",
			v->propietario, v->placa, v->anio, v->tipo, v->avaluo);
	fclose(archivo);
	printf("Vehiculo guardado en: %s\n", ruta);
}

void cargarVehiculosDesdeArchivo(Vehiculo vehiculos[], int *numVehiculos) {
	char ruta[260];
	const char *userProfile = getenv("USERPROFILE");
	if (userProfile != NULL) {
		snprintf(ruta, sizeof(ruta), "%s\\Downloads\\vehiculos.txt", userProfile);
	} else {
		strcpy(ruta, "vehiculos.txt");
	}
	
	FILE *archivo = fopen(ruta, "r");
	if (archivo == NULL) return;
	Vehiculo v;
	while (fscanf(archivo, "%49[^|]|%9[^|]|%d|%19[^|]|%f\n",
				  v.propietario, v.placa, &v.anio, v.tipo, &v.avaluo) == 5) {
		if (*numVehiculos < MAX_VEHICULOS) {
			vehiculos[*numVehiculos] = v;
			(*numVehiculos)++;
		}
	}
	fclose(archivo);
}

int main() {
	Vehiculo vehiculos[MAX_VEHICULOS];
	int numVehiculos = 0, opcion;
	
	inicializarVehiculos(vehiculos, MAX_VEHICULOS);
	cargarVehiculosDesdeArchivo(vehiculos, &numVehiculos);
	
	do {
		system(CLEAR);
		printf("\n--- Sistema de Matriculacion Vehicular ---\n");
		printf("1. Registrar nuevo vehiculo\n");
		printf("2. Mostrar todos los vehiculos registrados\n");
		printf("3. Calcular valor a pagar (con multas y descuentos)\n");
		printf("4. Salir\n");
		printf("Seleccione una opcion: ");
		
		if (scanf("%d", &opcion) != 1) {
			limpiarPantalla();
			printf("Entrada invalida. Intente nuevamente.\n");
			while (getchar() != '\n');
			esperar();
			continue;
		}
		
		while (getchar() != '\n');
		system(CLEAR);
		
		switch (opcion) {
		case 1:
			registrarVehiculo(vehiculos, &numVehiculos);
			break;
		case 2:
			mostrarTodosVehiculos(vehiculos, numVehiculos);
			break;
		case 3:
			if (numVehiculos == 0) {
				printf("No hay vehiculos registrados.\n");
				break;
			}
			for (int i = 0; i < numVehiculos; i++) {
				float multa = calcularMulta(vehiculos[i].anio);
				float descuento = calcularDescuento(vehiculos[i].avaluo);
				float total = vehiculos[i].avaluo + multa - descuento;
				printf("\n--- Matricula de ---\n", vehiculos[i].propietario, vehiculos[i].placa);
				printf("Avaluo base: %.2f\n", vehiculos[i].avaluo);
				printf("Multa: %.2f\n", multa);
				printf("Descuento: %.2f\n", descuento);
				printf("TOTAL A PAGAR: %.2f\n", total);
			}
			break;
		case 4:
			printf("Saliendo del sistema...\n");
			break;
		default:
			limpiarPantalla();
			printf("Opcion invalida. Intente nuevamente.\n");
			esperar();
		}
		if (opcion != 4) pausar();
		system(CLEAR);
	} while (opcion != 4);
	
	return 0;
}
