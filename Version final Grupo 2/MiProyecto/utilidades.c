#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h> // Para Sleep() en Windows
#include "utilidades.h"

void limpiarPantalla() { system(CLEAR); }
void esperar() { Sleep(1000); } // 1000 milisegundos = 1 segundo
void pausar() { printf("\nPresione Enter para continuar..."); getchar(); }

void convertirMayusculas(char *cadena) {
	for (int i = 0; cadena[i]; i++) {
		cadena[i] = toupper((unsigned char)cadena[i]);
	}
}
