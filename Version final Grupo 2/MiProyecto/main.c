#include <stdio.h>
#include <stdlib.h>
#include "vehiculo.h"
#include "utilidades.h"
#include "comprobante.h"
#include "usuario.h" // Incluimos el nuevo header para los usuarios

int main() {
	Vehiculo vehiculos[MAX_VEHICULOS];
	int numVehiculos = 0;
	
	Usuario usuarios[MAX_USUARIOS];
	int numUsuarios = 0;
	
	// Cargar veh�culos y usuarios al inicio del programa
	inicializarVehiculos(vehiculos, MAX_VEHICULOS);
	cargarVehiculosDesdeArchivo(vehiculos, &numVehiculos);
	
	inicializarUsuarios(usuarios, MAX_USUARIOS);
	cargarUsuariosDesdeArchivo(usuarios, &numUsuarios);
	
	int sesionIniciada = 0; // Bandera para controlar si hay una sesi�n activa
	int opcion_auth;
	
	do {
		limpiarPantalla();
		printf("--- BIENVENIDO AL SISTEMA DE GESTION DE VEHICULOS ---\n");
		printf("1. Iniciar Sesion\n");
		printf("2. Registrar Usuario\n");
		printf("3. Salir\n");
		printf("Seleccione una opcion (1-3): ");
		
		if (scanf("%d", &opcion_auth) != 1 || opcion_auth < 1 || opcion_auth > 3) {
			while (getchar() != '\n');
			printf("\n[Error] Opcion invalida. Intente de nuevo.\n");
			esperar();
			continue;
		}
		while (getchar() != '\n'); // Limpiar buffer
		
		switch (opcion_auth) {
		case 1: // Iniciar Sesi�n
			if (iniciarSesion(usuarios, numUsuarios)) {
				sesionIniciada = 1; // Si el inicio de sesi�n es exitoso
			}
			break;
		case 2: // Registrar Usuario
			registrarUsuario(usuarios, &numUsuarios);
			break;
		case 3: // Salir
			printf("Saliendo del programa. �Hasta pronto!\n");
			break;
		}
		
		// Si la sesi�n fue iniciada con �xito, mostrar el men� principal del sistema
		if (sesionIniciada) {
			int opcion_main;
			do {
				limpiarPantalla();
				printf("--- MENU PRINCIPAL ---\n");
				printf("1. Registrar vehiculo\n");
				printf("2. Registrar revision tecnica\n");
				printf("3. Calcular valor a pagar (con multas y descuentos)\n");
				printf("4. Buscar vehiculo por placa\n");
				printf("5. Mostrar todos los vehiculos\n");
				printf("6. Cerrar Sesion\n"); // Cambiado de "Salir" a "Cerrar Sesion"
				printf("Seleccione una opcion (1-6): ");
				if (scanf("%d", &opcion_main) != 1 || opcion_main < 1 || opcion_main > 6) {
					while (getchar() != '\n');
					printf("\n[Error] Opcion invalida.\n"); esperar(); continue;
				}
				while (getchar() != '\n');
				
				switch (opcion_main) {
				case 1:
					registrarVehiculo(vehiculos, &numVehiculos);
					break;
				case 2:
					if (numVehiculos == 0) {
						printf("\n[Advertencia] Debe registrar al menos un vehiculo antes de usar esta opcion.\n");
						pausar();
					} else {
						registrarRevisionTecnica(vehiculos, numVehiculos);
					}
					break;
				case 3:
					if (numVehiculos == 0) {
						printf("\n[Advertencia] Debe registrar al menos un vehiculo antes de usar esta opcion.\n");
						pausar();
					} else {
						for (int i = 0; i < numVehiculos; i++) {
							float multa = calcularMulta(vehiculos[i].anio);
							float descuento = calcularDescuento(vehiculos[i].avaluo);
							float total = vehiculos[i].avaluo + multa - descuento;
							mostrarComprobante(&vehiculos[i], multa, descuento, total);
							generarComprobanteArchivo(&vehiculos[i], multa, descuento, total);
						}
						pausar();
					}
					break;
				case 4:
					if (numVehiculos == 0) {
						printf("\n[Advertencia] Debe registrar al menos un vehiculo antes de usar esta opcion.\n");
						pausar();
					} else {
						buscarVehiculoPorPlaca(vehiculos, numVehiculos);
					}
					break;
				case 5:
					if (numVehiculos == 0) {
						printf("\n[Advertencia] Debe registrar al menos un vehiculo antes de usar esta opcion.\n");
						pausar();
					} else {
						mostrarTodosVehiculos(vehiculos, numVehiculos);
					}
					break;
				case 6: // Cerrar Sesi�n
					printf("Cerrando sesion...\n");
					sesionIniciada = 0; // Regresar al men� de autenticaci�n
					break;
				}
			} while (opcion_main != 6); // Bucle del men� principal
		}
		
	} while (opcion_auth != 3); // Bucle del men� de autenticaci�n
	return 0;
}
