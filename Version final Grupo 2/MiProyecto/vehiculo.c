#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "vehiculo.h"
#include "utilidades.h" // Incluye utilidades para usar funciones como convertirMayusculas

const char *tiposVehiculos[] = {"Automovil", "Moto", "Bus", "Trailer"};

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
	return strlen(placa) == 8 && isupper(placa[0]) && isupper(placa[1]) && isupper(placa[2]) &&
		placa[3] == '-' && isdigit(placa[4]) && isdigit(placa[5]) && isdigit(placa[6]) && isdigit(placa[7]);
}

int validarTipo(int tipo) {
	return tipo >= 1 && tipo <= 4;
}

float obtenerTopePorTipo(int tipo) {
	switch (tipo) {
	case 1: return 60000.0f;  // Automóvil
	case 2: return 15000.0f;  // Moto
	case 3: return 120000.0f; // Bus
	case 4: return 150000.0f; // Trailer
	default: return 50000.0f;
	}
}

void registrarVehiculo(Vehiculo vehiculos[], int *numVehiculos) {
	Vehiculo nuevo;
	limpiarPantalla();
	
	do {
		printf("Ingrese nombre del propietario (solo letras): ");
		fgets(nuevo.propietario, sizeof(nuevo.propietario), stdin);
		nuevo.propietario[strcspn(nuevo.propietario, "\n")] = '\0';
		if (!validarNombre(nuevo.propietario)) {
			printf("\n[Error] Nombre invalido.\n"); esperar(); limpiarPantalla();
		} else break;
	} while (1);
	convertirMayusculas(nuevo.propietario);
	
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
		convertirMayusculas(nuevo.placa);
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
		printf("Seleccione tipo de vehiculo:\n1. Automovil\n2. Moto\n3. Bus\n4. Trailer\nIngrese opcion: ");
		if (scanf("%d", &nuevo.tipo) != 1 || !validarTipo(nuevo.tipo)) {
			printf("\n[Error] Tipo invalido.\n"); while (getchar() != '\n'); esperar(); limpiarPantalla();
		} else break;
	} while (1);
	
	float tope = obtenerTopePorTipo(nuevo.tipo);
	do {
		printf("Ingrese avaluo (>0 y <= %.2f): ", tope);
		if (scanf("%f", &nuevo.avaluo) != 1 || nuevo.avaluo <= 0 || nuevo.avaluo > tope) {
			printf("\n[Error] Avaluo invalido. Tope para este tipo: %.2f\n", tope);
			while (getchar() != '\n'); esperar(); limpiarPantalla();
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

void guardarVehiculoEnArchivo(const Vehiculo *v) {
	FILE *archivo = fopen("vehiculos.txt", "a");
	if (!archivo) {
		perror("Error al abrir el archivo de vehiculos para guardar");
		return;
	}
	fprintf(archivo, "%s|%s|%s|%d|%d|%.2f|%d\n",
			v->propietario, v->placa, v->cedula, v->anio, v->tipo, v->avaluo, v->revisiones);
	fclose(archivo);
}

void cargarVehiculosDesdeArchivo(Vehiculo vehiculos[], int *numVehiculos) {
	FILE *archivo = fopen("vehiculos.txt", "r");
	if (!archivo) {
		return;
	}
	Vehiculo v;
	*numVehiculos = 0;
	while (fscanf(archivo, "%49[^|]|%9[^|]|%10[^|]|%d|%d|%f|%d\n",
				  v.propietario, v.placa, v.cedula, &v.anio, &v.tipo, &v.avaluo, &v.revisiones) == 7 &&
		   *numVehiculos < MAX_VEHICULOS) {
		vehiculos[(*numVehiculos)++] = v;
	}
	fclose(archivo);
}

void mostrarTodosVehiculos(const Vehiculo vehiculos[], int numVehiculos) {
	limpiarPantalla();
	if (numVehiculos == 0) {
		printf("No hay vehiculos registrados.\n");
	} else {
		printf("--- LISTA DE VEHICULOS REGISTRADOS ---\n");
		for (int i = 0; i < numVehiculos; i++) {
			printf("\nVehiculo %d:\n", i + 1);
			printf("  Propietario: %s\n", vehiculos[i].propietario);
			printf("  Placa: %s\n", vehiculos[i].placa);
			printf("  Cedula: %s\n", vehiculos[i].cedula);
			printf("  Anio: %d\n", vehiculos[i].anio);
			printf("  Tipo: %s\n", tiposVehiculos[vehiculos[i].tipo - 1]);
			printf("  Avaluo: %.2f\n", vehiculos[i].avaluo);
			printf("  Revisiones: %d/%d\n", vehiculos[i].revisiones, REVISIONES_REQUERIDAS);
		}
	}
	pausar();
}

// En vehiculo.c
void buscarVehiculoPorPlaca(const Vehiculo vehiculos[], int numVehiculos) { // ¡Añade 'const' aquí!
	char placa[10];
	while (getchar() != '\n'); // limpia cualquier basura en el búfer
	printf("Ingrese la placa a buscar: ");
	scanf("%s", placa);
	while (getchar() != '\n'); // limpia el enter después de scanf
	
	convertirMayusculas(placa);
	
	int encontrado = 0;
	for (int i = 0; i < numVehiculos; i++) {
		if (strcmp(vehiculos[i].placa, placa) == 0) {
			printf("\nVehiculo encontrado:\n");
			printf("Placa: %s\n", vehiculos[i].placa);
			printf("Propietario: %s\n", vehiculos[i].propietario);
			printf("Anio: %d\n", vehiculos[i].anio); // Añadido para mostrar más detalles
			printf("Tipo: %s\n", tiposVehiculos[vehiculos[i].tipo - 1]); // Añadido para mostrar más detalles
			printf("Avaluo: %.2f\n", vehiculos[i].avaluo); // Añadido para mostrar más detalles
			printf("Revisiones: %d/%d\n", vehiculos[i].revisiones, REVISIONES_REQUERIDAS);
			encontrado = 1;
			break;
		}
	}
	
	if (!encontrado) {
		printf("\n[Error] Vehiculo con placa %s no encontrado.\n", placa);
	}
	
	pausar();
}
void registrarRevisionTecnica(Vehiculo vehiculos[], int numVehiculos) {
	char placa[10];
	printf("Ingrese la placa del vehiculo para registrar revision: ");
	scanf("%s", placa);
	while (getchar() != '\n');
	
	convertirMayusculas(placa);
	
	int encontrado = 0;
	for (int i = 0; i < numVehiculos; i++) {
		if (strcmp(vehiculos[i].placa, placa) == 0) {
			encontrado = 1;
			if (vehiculos[i].revisiones >= REVISIONES_REQUERIDAS) {
				printf("\nYa se han registrado las %d revisiones anuales para el vehiculo con placa %s.\n", REVISIONES_REQUERIDAS, vehiculos[i].placa);
			} else {
				vehiculos[i].revisiones++;
				printf("\n[Éxito] Revision tecnica registrada correctamente para %s. Total de revisiones: %d/%d\n",
					   vehiculos[i].placa, vehiculos[i].revisiones, REVISIONES_REQUERIDAS);
			}
			break;
		}
	}
	
	if (!encontrado) {
		printf("\n[Error] Vehiculo con placa %s no encontrado. Asegurese de que este registrado.\n", placa);
	}
	pausar();
}
