#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "usuario.h"
#include "utilidades.h" // Para limpiarPantalla, esperar, pausar, convertirMayusculas

void inicializarUsuarios(Usuario usuarios[], int len) {
	for (int i = 0; i < len; i++) {
		usuarios[i].username[0] = '\0';
		usuarios[i].password[0] = '\0';
	}
}

// Valida que el nombre de usuario y la contraseña no superen MAX_USERNAME_LEN/MAX_PASSWORD_LEN
// y que no estén vacíos.
int validarCredenciales(const char *username, const char *password) {
	if (strlen(username) == 0 || strlen(username) > MAX_USERNAME_LEN) {
		printf("\n[Error] El usuario debe tener entre 1 y %d caracteres.\n", MAX_USERNAME_LEN);
		return 0;
	}
	if (strlen(password) == 0 || strlen(password) > MAX_PASSWORD_LEN) {
		printf("\n[Error] La contrasena debe tener entre 1 y %d caracteres.\n", MAX_PASSWORD_LEN);
		return 0;
	}
	return 1;
}

int registrarUsuario(Usuario usuarios[], int *numUsuarios) {
	if (*numUsuarios >= MAX_USUARIOS) {
		printf("\n[Error] No se pueden registrar mas usuarios. Limite alcanzado.\n");
		pausar();
		return 0;
	}
	
	Usuario nuevoUsuario;
	char temp_username[100]; // Buffer temporal para entrada del usuario
	char temp_password[100]; // Buffer temporal para entrada del usuario
	int valido = 0;
	
	limpiarPantalla();
	printf("--- REGISTRO DE USUARIO ---\n");
	
	do {
		printf("Ingrese nuevo nombre de usuario (max %d caracteres): ", MAX_USERNAME_LEN);
		fgets(temp_username, sizeof(temp_username), stdin);
		temp_username[strcspn(temp_username, "\n")] = '\0'; // Eliminar salto de línea
		
		printf("Ingrese nueva contrasena (max %d caracteres): ", MAX_PASSWORD_LEN);
		fgets(temp_password, sizeof(temp_password), stdin);
		temp_password[strcspn(temp_password, "\n")] = '\0'; // Eliminar salto de línea
		
		valido = validarCredenciales(temp_username, temp_password);
		if (!valido) {
			esperar();
			limpiarPantalla();
			printf("--- REGISTRO DE USUARIO ---\n"); // Volver a mostrar el encabezado
			continue;
		}
		
		// Convertir a mayúsculas si es necesario, o mantener como está si quieres sensibilidad a mayús/minús
		// convertirMayusculas(temp_username); // Descomenta si quieres usuarios case-insensitive
		
		// Verificar si el usuario ya existe
		for (int i = 0; i < *numUsuarios; i++) {
			if (strcmp(usuarios[i].username, temp_username) == 0) {
				printf("\n[Error] El nombre de usuario '%s' ya existe. Intente con otro.\n", temp_username);
				valido = 0; // Marcar como inválido para repetir el bucle
				esperar();
				limpiarPantalla();
				printf("--- REGISTRO DE USUARIO ---\n"); // Volver a mostrar el encabezado
				break;
			}
		}
		
	} while (!valido);
	
	strcpy(nuevoUsuario.username, temp_username);
	strcpy(nuevoUsuario.password, temp_password);
	
	usuarios[(*numUsuarios)++] = nuevoUsuario;
	guardarUsuariosEnArchivo(usuarios, *numUsuarios);
	printf("\n[Registro Exitoso] Usuario '%s' registrado.\n", nuevoUsuario.username);
	pausar();
	return 1;
}

int iniciarSesion(const Usuario usuarios[], int numUsuarios) {
	char username_input[100];
	char password_input[100];
	int intentos = 1;
	
	limpiarPantalla();
	printf("--- INICIO DE SESION ---\n");
	
	while (intentos > 0) {
		printf("Usuario: ");
		fgets(username_input, sizeof(username_input), stdin);
		username_input[strcspn(username_input, "\n")] = '\0';
		
		printf("Contrasena: ");
		fgets(password_input, sizeof(password_input), stdin);
		password_input[strcspn(password_input, "\n")] = '\0';
		
		// convertirMayusculas(username_input); // Descomenta si los usuarios son case-insensitive
		
		for (int i = 0; i < numUsuarios; i++) {
			if (strcmp(usuarios[i].username, username_input) == 0 &&
				strcmp(usuarios[i].password, password_input) == 0) {
				printf("\n[Inicio de Sesion Exitoso] Bienvenido, %s!\n", username_input);
				pausar();
				return 1; // Sesión iniciada correctamente
			}
		}
		
		intentos--;
		printf("\n[Error] Credenciales incorrectas. Intentos restantes: %d\n", intentos);
		esperar();
		limpiarPantalla();
		printf("--- INICIO DE SESION ---\n");
	}
	
	printf("\n[Bloqueado] Demasiados intentos fallidos. Volviendo al menu principal.\n");
	pausar();
	return 0; // Falló el inicio de sesión
}


void guardarUsuariosEnArchivo(const Usuario usuarios[], int numUsuarios) {
	FILE *archivo = fopen("usuarios.txt", "w"); // Modo 'w' para sobrescribir y guardar el estado actual
	if (!archivo) {
		perror("Error al abrir el archivo de usuarios para guardar");
		return;
	}
	for (int i = 0; i < numUsuarios; i++) {
		fprintf(archivo, "%s|%s\n", usuarios[i].username, usuarios[i].password);
	}
	fclose(archivo);
}

void cargarUsuariosDesdeArchivo(Usuario usuarios[], int *numUsuarios) {
	FILE *archivo = fopen("usuarios.txt", "r");
	if (!archivo) {
		// El archivo no existe o no se pudo abrir, se inicia sin usuarios cargados
		*numUsuarios = 0;
		return;
	}
	Usuario u;
	*numUsuarios = 0;
	while (fscanf(archivo, "%8[^|]|%8[^\n]\n", u.username, u.password) == 2 && *numUsuarios < MAX_USUARIOS) {
		usuarios[(*numUsuarios)++] = u;
	}
	fclose(archivo);
}
