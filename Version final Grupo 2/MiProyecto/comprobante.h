#ifndef COMPROBANTE_H
#define COMPROBANTE_H

#include "vehiculo.h" // Necesitamos la estructura Vehiculo

// Prototipos de funciones de comprobante
void mostrarComprobante(const Vehiculo *v, float multa, float descuento, float total);
void generarComprobanteArchivo(const Vehiculo *v, float multa, float descuento, float total);

#endif // COMPROBANTE_H
