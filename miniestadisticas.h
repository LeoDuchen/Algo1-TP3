#ifndef __MINIESTADISTICAS_H__
#define __MINIESTADISTICAS_H__

#define MAX_NOMBRE 50
#define MAX_RANGO 50
#define MAX_DESCRIPCION 200

typedef struct enano {
	char nombre[MAX_NOMBRE];
	int edad;
	int cantidad_misiones;
	int id_rango;
} enano_t;

typedef struct rango {
	int id;
	char nombre[MAX_RANGO];
	char descripcion[MAX_DESCRIPCION];
} rango_t;

/* Pre: Debe existir un archivo censo cargado y un archivo mision cargado. 
Recibe un char con el nombre de una mision. 
Post: Actualiza el archivo censo. */
int actualizar_censo(char* argumento);

/* Pre: Debe existir un archivo censo cargado. 
Recibe tres char con el número de guerreros, líderes y generales 
respectivamente y un char con el nombre de la mision. 
Post: Crea el archivo mision. */
int elegir_enanos(char* argumento1, char* argumento2, char* argumento3, char* argumento4);

/* Pre: Debe existir un archivo censo cargado. 
Post: Actualiza el archivo censo. */
int promover_enanos();

/* Pre: Debe existir un archivo censo cargado y un archivo rango cargado. 
Recibe un char con el número de rango. 
Post: Muestra por pantalla el nombre y descripción del rango recibido, 
y los datos de los enanos con ese rango. */
int listar_enanos(char* argumento);

/* Post: Muestra por pantalla los comandos existentes. */
void mostrar_comandos();

/* Post: Muestra por pantalla un mensaje si un comando 
fue ingresado incorrectamente. */
void mostrar_mensaje();

/* Pre: Recibe un char con el nombre del comando. 
Post: Devuelve si el comando es el de actualizar censo. */
_Bool es_actualizar_censo(char* argumento1);

/* Pre: Recibe un char con el nombre del comando. 
Post: Devuelve si el comando es el de elegir enanos para la misión. */
_Bool es_elegir_enanos(char* argumento);

/* Pre: Recibe un char con el nombre del comando. 
Post: Devuelve si el comando es el de promover enanos. */
_Bool es_promover_enanos(char* argumento);

/* Pre: Recibe un char con el nombre del comando. 
Post: Devuelve si el comando es el de listar enanos de un rango. */
_Bool es_listar_enanos(char* argumento1, char* argumento2);

/* Pre: Recibe un char con el nombre del comando. 
Post: Devuelve si el comando es el mostrar los comandos existentes. */
_Bool es_mostrar_comandos(char* argumento);

#endif /* __MINIESTADISTICAS_H__ */