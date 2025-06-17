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
	int revisiones;
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

void inicializarVehiculos(Vehiculo vehiculos[], int len) {
	for (int i = 0; i < len; i++) {
		strcpy(vehiculos[i].propietario, "");
		strcpy(vehiculos[i].placa, "");
		vehiculos[i].anio = 0;
		strcpy(vehiculos[i].tipo, "");
		vehiculos[i].avaluo = 0.0f;
		vehiculos[i].revisiones = 0;
	}
}

void registrarVehiculo(Vehiculo vehiculos[], int *numVehiculos) {
	if (*numVehiculos >= MAX_VEHICULOS) {
		limpiarPantalla();
		printf("�Capacidad m�xima alcanzada!\n");
		esperar();
		return;
	}
	
	Vehiculo nuevo;
	
	// Nombre
	do {
		printf("Ingrese el nombre del propietario: ");
		fgets(nuevo.propietario, sizeof(nuevo.propietario), stdin);
		nuevo.propietario[strcspn(nuevo.propietario, "\n")] = '\0';
		if (!validarNombre(nuevo.propietario)) {
			limpiarPantalla();
			printf("Error: El nombre no debe contener n�meros.\n");
			esperar();
			system(CLEAR);
		} else {
			break;
		}
	} while (1);
	
	// Placa
	do {
		printf("Ingrese la placa del veh�culo: ");
		scanf("%s", nuevo.placa);
		if (!validarPlaca(nuevo.placa)) {
			limpiarPantalla();
			printf("Error: Placa inv�lida.\n");
			esperar();
			system(CLEAR);
		} else {
			break;
		}
	} while (1);
	
	// A�o
	do {
		printf("Ingrese el a�o del veh�culo (1900-2025): ");
		if (scanf("%d", &nuevo.anio) != 1 || nuevo.anio < 1900 || nuevo.anio > 2025) {
			limpiarPantalla();
			printf("Error: A�o fuera de rango o inv�lido.\n");
			while (getchar() != '\n');
			esperar();
			system(CLEAR);
		} else {
			break;
		}
	} while (1);
	
	// Tipo
	printf("Ingrese tipo de veh�culo (e.g., Autom�vil, Moto): ");
	scanf("%s", nuevo.tipo);
	
	// Aval�o
	do {
		printf("Ingrese aval�o base (>0): ");
		if (scanf("%f", &nuevo.avaluo) != 1 || nuevo.avaluo <= 0) {
			limpiarPantalla();
			printf("Error: Aval�o inv�lido.\n");
			while (getchar() != '\n');
			esperar();
			system(CLEAR);
		} else {
			break;
		}
	} while (1);
	
	while (getchar() != '\n'); // limpiar buffer
	
	calcularMatricula(&nuevo.avaluo, nuevo.anio);
	nuevo.revisiones = 0;
	
	vehiculos[*numVehiculos] = nuevo;
	(*numVehiculos)++;
	
	guardarVehiculoEnArchivo(&nuevo);
	
	printf("\nVeh�culo registrado exitosamente.\n");
	mostrarDatos(nuevo.propietario, nuevo.placa, nuevo.avaluo, nuevo.anio);
	printf("Total a pagar por matr�cula: %.2f\n", nuevo.avaluo);
}

void mostrarVehiculo(const Vehiculo *v) {
	printf("\n--- Datos del Veh�culo ---\n");
	printf("Propietario: %s\n", v->propietario);
	printf("Placa: %s\n", v->placa);
	printf("A�o: %d\n", v->anio);
	printf("Tipo: %s\n", v->tipo);
	printf("Aval�o: %.2f\n", v->avaluo);
	printf("Revisiones: %d\n", v->revisiones);
}

void mostrarTodosVehiculos(const Vehiculo vehiculos[], int numVehiculos) {
	if (numVehiculos == 0) {
		printf("No hay veh�culos registrados.\n");
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
	printf("A�o: %d\n", anio);
	printf("Valor base: %.2f\n", valorBase);
}

//Calculo de avaluo
void calcularMatricula(float *valorBase, int anio) {
	if (anio < 2015) {
		*valorBase *= 0.85f;
	} else if (anio <= 2022) {
		*valorBase *= 0.95f;
	}
}

//Validacion de nombre
int validarNombre(char *nombre) {
	for (int i = 0; nombre[i] != '\0'; i++) {
		if (isdigit((unsigned char)nombre[i])) return 0;
	}
	return 1;
}

//Validacion de placa
int validarPlaca(char *placa) {
	int len = strlen(placa);
	
	// Debe tener longitud 7 (ABC-123) u 8 (ABC-1234)
	if (len != 7 && len != 8) return 0;
	
	// Validar las 3 primeras letras (may�sculas)
	for (int i = 0; i < 3; i++) {
		if (!isupper((unsigned char)placa[i]) || !isalpha((unsigned char)placa[i])) {
			return 0;
		}
	}
	
	// Validar el guion en la posici�n 3
	if (placa[3] != '-') return 0;
	
	// Validar los �ltimos 3 o 4 caracteres como d�gitos
	for (int i = 4; i < len; i++) {
		if (!isdigit((unsigned char)placa[i])) {
			return 0;
		}
	}
	
	return 1;
}


void pausar() {
	printf("\nPresione Enter para volver al men�...");
	getchar();
}

void limpiarPantalla() {
	system(CLEAR);
}

//Limpiar mensaje de error
void esperar() {
#ifdef _WIN32
	Sleep(2000);
#else
	sleep(2);
#endif
}

//Guardar los vehiculos registrados en archivo txt.
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
		perror("No se pudo abrir el archivo para guardar el veh�culo");
		return;
	}
	
	fprintf(archivo, "%s|%s|%d|%s|%.2f|%d\n",
			v->propietario, v->placa, v->anio, v->tipo, v->avaluo, v->revisiones);
	
	fclose(archivo);
	printf("Veh�culo guardado en: %s\n", ruta);
}

//Cargar vehiculos desde el archivo
void cargarVehiculosDesdeArchivo(Vehiculo vehiculos[], int *numVehiculos) {
	char ruta[260];
	const char *userProfile = getenv("USERPROFILE");
	if (userProfile != NULL) {
		snprintf(ruta, sizeof(ruta), "%s\\Downloads\\vehiculos.txt", userProfile);
	} else {
		strcpy(ruta, "vehiculos.txt");
	}
	
	FILE *archivo = fopen(ruta, "r");
	if (archivo == NULL) {
		return;
	}
	
	Vehiculo v;
	while (fscanf(archivo, "%49[^|]|%9[^|]|%d|%19[^|]|%f|%d\n",
				  v.propietario, v.placa, &v.anio, v.tipo, &v.avaluo, &v.revisiones) == 6) {
		if (*numVehiculos < MAX_VEHICULOS) {
			vehiculos[*numVehiculos] = v;
			(*numVehiculos)++;
		}
	}
	
	fclose(archivo);
}

//Lo que se ve en pantalla al ejecutar el codigo
int main() {
	Vehiculo vehiculos[MAX_VEHICULOS];
	int numVehiculos = 0, opcion;
	
	inicializarVehiculos(vehiculos, MAX_VEHICULOS);
	cargarVehiculosDesdeArchivo(vehiculos, &numVehiculos);
	
	do {
		system(CLEAR);
		printf("\n--- Sistema de Matriculaci�n Vehicular ---\n");
		printf("1. Registrar nuevo veh�culo\n");
		printf("2. Mostrar todos los veh�culos registrados\n");
		printf("3. Salir\n");
		printf("Seleccione una opci�n: ");
		
		if (scanf("%d", &opcion) != 1) {
			limpiarPantalla();
			printf("Entrada inv�lida. Intente nuevamente.\n");
			while (getchar() != '\n');
			esperar();
			continue;
		}
		
		while (getchar() != '\n'); // limpiar buffer
		system(CLEAR);
		
		switch (opcion) {
		case 1:
			registrarVehiculo(vehiculos, &numVehiculos);
			break;
		case 2:
			mostrarTodosVehiculos(vehiculos, numVehiculos);
			break;
		case 3:
			printf("Saliendo del sistema...\n");
			break;
		default:
			limpiarPantalla();
			printf("Opci�n inv�lida. Intente nuevamente.\n");
			esperar();
		}
		
		if (opcion != 3) pausar();
		system(CLEAR);
	} while (opcion != 3);
	
	return 0;
}
