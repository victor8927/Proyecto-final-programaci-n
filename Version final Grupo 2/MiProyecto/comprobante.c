#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comprobante.h" // Incluimos nuestro propio header
#include "vehiculo.h"   // Para acceder a la estructura Vehiculo y tiposVehiculos

void mostrarComprobante(const Vehiculo *v, float multa, float descuento, float total) {
	printf("\n--- COMPROBANTE DE MATRICULA ---\n");
	printf("Propietario: %s\nPlaca: %s\nAvaluo Base: %.2f\nTipo: %s\nMulta: %.2f\nDescuento: %.2f\nTOTAL A PAGAR: %.2f\n",
		   v->propietario, v->placa, v->avaluo, tiposVehiculos[v->tipo - 1], multa, descuento, total);
}

void generarComprobanteArchivo(const Vehiculo *v, float multa, float descuento, float total) {
	char nombreArchivo[64];
	sprintf(nombreArchivo, "comprobante_%s.txt", v->placa);
	FILE *f = fopen(nombreArchivo, "w");
	if (!f) {
		perror("Error al crear el archivo de comprobante");
		return;
	}
	fprintf(f, "COMPROBANTE DE MATRICULA\nPropietario: %s\nPlaca: %s\nTipo: %s\nAvaluo Base: %.2f\nMulta: %.2f\nDescuento: %.2f\nTOTAL A PAGAR: %.2f\n",
			v->propietario, v->placa, tiposVehiculos[v->tipo - 1], v->avaluo, multa, descuento, total);
	fclose(f);
}
