#ifndef USUARIO_H
#define USUARIO_H

#define MAX_USUARIOS 50
#define MAX_USERNAME_LEN 8
#define MAX_PASSWORD_LEN 8

typedef struct {
	char username[MAX_USERNAME_LEN + 1]; // +1 para el terminador nulo '\0'
	char password[MAX_PASSWORD_LEN + 1]; // +1 para el terminador nulo '\0'
} Usuario;

// Prototipos de funciones de usuario
void inicializarUsuarios(Usuario usuarios[], int len);
int registrarUsuario(Usuario usuarios[], int *numUsuarios);
int iniciarSesion(const Usuario usuarios[], int numUsuarios);
void guardarUsuariosEnArchivo(const Usuario usuarios[], int numUsuarios);
void cargarUsuariosDesdeArchivo(Usuario usuarios[], int *numUsuarios);
int validarCredenciales(const char *username, const char *password);

#endif // USUARIO_H
