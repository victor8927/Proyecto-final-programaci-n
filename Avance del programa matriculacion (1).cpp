#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Función para mostrar los datos
void mostrarDatos(char propietario[], char placa[], float valorBase, int anio) {
	printf("\n--- DATOS DEL VEHICULO ---\n");
	printf("Propietario: %s\n", propietario);  // <- cambio aquí
	printf("Placa: %s\n", placa);
	printf("Anio: %d\n", anio);
	printf("Valor base: %.2f\n", valorBase);
}



// Función para calcular la matrícula
void calcularMatricula(float *valorBase, int anio) {
	if (anio < 2015) {
		*valorBase *= 0.85;
	} else if (anio <= 2022) {
		*valorBase *= 0.95;
	}
}

// Validar que el nombre no contenga números
int validarNombre(char *nombre) {
	for (int i = 0; nombre[i] != '\0'; i++) {
		if (isdigit((unsigned char)nombre[i])) {
			return 0;
		}
	}
	return 1;
}

// Validar la placa (máximo 8 caracteres, solo letras, números y guion)
int validarPlaca(char *placa) {
	int len = strlen(placa);
	if (len > 8 || len == 0) return 0;
	for (int i = 0; i < len; i++) {
		if (!isalnum(placa[i]) && placa[i] != '-') return 0;
	}
	return 1;
}

// Registrar errores con fecha y hora
void registrarError(const char *mensaje) {
	FILE *log = fopen("errores.log", "a");
	if (log != NULL) {
		time_t ahora = time(NULL);
		struct tm *tinfo = localtime(&ahora);
		char fechaHora[100];
		strftime(fechaHora, sizeof(fechaHora), "[%Y-%m-%d %H:%M:%S]", tinfo);
		fprintf(log, "%s %s\n", fechaHora, mensaje);
		fclose(log);
	}
}

int main() {
	char propietario[50];
	char placa[10];
	float valorBase;
	int anio;
	
	// Ingreso y validación del nombre del propietario
	do {
		printf("Ingrese el nombre del propietario: ");
		fgets(propietario, sizeof(propietario), stdin);
		propietario[strcspn(propietario, "\n")] = '\0';  // Eliminar salto de línea
		
		if (!validarNombre(propietario)) {
			printf("Error: El nombre no debe contener numeros.\n");
			registrarError("Error: El usuario ingreso numeros en el nombre del propietario.");
		} else {
			break;
		}
	} while (1);
	
	// Ingreso y validación de la placa
	do {
		printf("Ingrese la placa del vehiculo (max. 8 caracteres, letras/numeros y guion '-'): ");
		scanf("%s", placa);
		
		if (!validarPlaca(placa)) {
			printf("Error: La placa debe contener solo letras, numeros o guion (max. 8 caracteres).\n");
			registrarError("Error: Placa invalida ingresada.");
		} else {
			break;
		}
	} while (1);
	
	// Ingreso y validación del año
	do {
		printf("Ingrese el año del vehiculo (entre 1900 y 2025): ");
		if (scanf("%d", &anio) != 1 || anio < 1900 || anio > 2025) {
			printf("Error: El año debe estar entre 1900 y 2025.\n");
			registrarError("Error: Anio fuera del rango permitido o invalido.");
			while (getchar() != '\n'); // limpiar buffer
		} else {
			break;
		}
	} while (1);
	
	// Ingreso y validación del valor base
	do {
		printf("Ingrese el valor base de la matricula (mayor a 0): ");
		if (scanf("%f", &valorBase) != 1 || valorBase <= 0) {
			printf("Error: El valor base debe ser mayor a cero.\n");
			registrarError("Error: Valor base no valido.");
			while (getchar() != '\n'); // limpiar buffer
		} else {
			break;
		}
	} while (1);
	
	mostrarDatos(propietario, placa, valorBase, anio);
	calcularMatricula(&valorBase, anio);
	printf("\n--- VALOR ACTUALIZADO DE MATRICULA ---\n");
	printf("Total a pagar: %.2f\n", valorBase);
	
	// Esperar antes de salir
	printf("\nPresione ENTER para salir...");
	getchar(); getchar();
	return 0;
}

