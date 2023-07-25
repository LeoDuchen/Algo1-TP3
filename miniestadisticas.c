#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "miniestadisticas.h"

const char* RUTA_CENSO = "censo.csv";
const char* RUTA_TEMPORAL = "temporal.csv";
const char* RUTA_RANGOS = "rangos.dat";
const char* RUTA_MISION = "mision.dat";
const char* COMANDO_ACTUALIZAR_CENSO = "actualizar_censo";
const char* COMANDO_ELEGIR_ENANOS = "integrantes_mision";
const char* COMANDO_PROMOVER_ENANOS = "promover_enanos";
const char* COMANDO_LISTAR_ENANOS = "listar_enanos";
const char* COMANDO_MOSTRAR_COMANDOS = "help";
const char* EXTENSION_ARCHIVO = ".dat";
const int COINCIDENCIA = 0;
const int MINIMO_DE_ENANOS = 0;
const int ERROR_DE_APERTURA = -1, ERROR_DE_LECTURA = -2, ERROR_DE_ESCRITURA = -3;
const int INSUFICIENTES_ENANOS = -4, ARCHIVO_EXISTE = -5;
const int OBRERO = 1, APRENDIZ = 2, GUERRERO = 3, LIDER = 4, GENERAL = 5;
const int REQUISITO_MISIONES_GUERRERO = 10;
const int REQUISITO_MISIONES_LIDER = 100;
const int REQUISITO_EDAD_LIDER_MIN = 40, REQUISITO_EDAD_LIDER_MAX = 50;
const int REQUISITO_MISIONES_GENERAL = 250;
const int REQUISITO_EDAD_GENERAL_MIN = 50, REQUISITO_EDAD_GENERAL_MAX = 60;
const int CANT_APRENDICES_MAX = 10;
const int ENANO_MUERTO = -1;
#define MAX_ARGUMENTO 50
#define MAX_RUTA 50

/* Pre: Recibe por referencia una ruta y un char con el nombre de la mision. 
Post: Cambia la ruta agregandole una extension. */
void crear_nueva_ruta(char* nueva_ruta, char* argumento) {
	nueva_ruta[0] = '\0';
	strcat(nueva_ruta, argumento);
	strcat(nueva_ruta, EXTENSION_ARCHIVO);
}

int actualizar_censo(char* argumento) {
	int cantidad_leidos = 0;
	size_t leidos = 0;
	enano_t enano_censo;
	enano_t enano_mision;

	char nueva_ruta[MAX_RUTA];
	crear_nueva_ruta(nueva_ruta, argumento);

	FILE* archivo_mision = fopen(nueva_ruta, "r");
	if (!archivo_mision) {
		perror("Error de apertura de archivo.\n");
		return ERROR_DE_APERTURA;
	}

	FILE* archivo_censo = fopen(RUTA_CENSO, "r");
	if (!archivo_censo) {
		perror("Error de apertura de archivo.\n");
		fclose(archivo_mision);
		return ERROR_DE_APERTURA;
	}

	FILE* archivo_temporal = fopen(RUTA_TEMPORAL, "w");
	if (!archivo_temporal) {
		perror("Error de apertura de archivo.\n");
		fclose(archivo_mision);
		fclose(archivo_censo);
		return ERROR_DE_APERTURA;
	}

	cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
    if (!cantidad_leidos) {
    	perror("Error de lectura en el archivo.\n");
    	fclose(archivo_mision);
		fclose(archivo_censo);
		fclose(archivo_temporal);
    	return ERROR_DE_LECTURA;
    }

	leidos = fread(&enano_mision, sizeof(enano_t), 1, archivo_mision);
	if (!leidos) {
		perror("Error de lectura en el archivo.\n");
		fclose(archivo_mision);
		fclose(archivo_censo);
		fclose(archivo_temporal);
		return ERROR_DE_LECTURA;
	}
    while ((!feof(archivo_mision)) && (cantidad_leidos != EOF)) {
    	if (strcmp(enano_censo.nombre, enano_mision.nombre) == COINCIDENCIA) {
    		if (enano_mision.edad != ENANO_MUERTO) {
    			fprintf(archivo_temporal, "%s;%i;%i;%i\n", enano_censo.nombre, enano_censo.edad, enano_censo.cantidad_misiones + 1, enano_censo.id_rango);
    		}
    		leidos = fread(&enano_mision, sizeof(enano_t), 1, archivo_mision);
       		cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
    	} else if (strcmp(enano_censo.nombre, enano_mision.nombre) > COINCIDENCIA) {
    		if (enano_mision.edad != ENANO_MUERTO) {
    			fprintf(archivo_temporal, "%s;%i;%i;%i\n", enano_mision.nombre, enano_mision.edad, enano_mision.cantidad_misiones + 1, enano_mision.id_rango);
    		}
    		leidos = fread(&enano_mision, sizeof(enano_t), 1, archivo_mision);
    	} else {
			fprintf(archivo_temporal, "%s;%i;%i;%i\n", enano_censo.nombre, enano_censo.edad, enano_censo.cantidad_misiones, enano_censo.id_rango);	
    		cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
    	}
    }

	while (cantidad_leidos != EOF) {
		fprintf(archivo_temporal, "%s;%i;%i;%i\n", enano_censo.nombre, enano_censo.edad, enano_censo.cantidad_misiones, enano_censo.id_rango);
    	cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano_censo.nombre, &enano_censo.edad, &enano_censo.cantidad_misiones, &enano_censo.id_rango);
	}

	while (!feof(archivo_mision)) {
		if (enano_mision.edad != ENANO_MUERTO) {
			fprintf(archivo_temporal, "%s;%i;%i;%i\n", enano_mision.nombre, enano_censo.edad, enano_censo.cantidad_misiones++, enano_censo.id_rango);
		}		
    	leidos = fread(&enano_mision, sizeof(enano_t), 1, archivo_mision);
	}

	fclose(archivo_mision);
	fclose(archivo_censo);
	fclose(archivo_temporal);

    remove(RUTA_CENSO);
    rename(RUTA_TEMPORAL, RUTA_CENSO);

	return 0;
}

int elegir_enanos(char* argumento1, char* argumento2, char* argumento3, char* argumento4) {
	int cantidad_leidos = 0;
	enano_t enano;

	int cantidad_guerreros = atoi(argumento1);
	int cantidad_lideres = atoi(argumento2);
	int cantidad_generales = atoi(argumento3);

	int contador_guerreros = 0;
	int contador_lideres = 0;
	int contador_generales = 0;
	int contador_aprendices = 0;

	char nueva_ruta[MAX_RUTA];
	crear_nueva_ruta(nueva_ruta, argumento4);

	FILE* archivo_existente = fopen(nueva_ruta, "r");
	if (archivo_existente) {
		perror("Ya existe un archivo con ese nombre.\n");
		fclose(archivo_existente);
		return ARCHIVO_EXISTE;
	}

	FILE* archivo_mision = fopen(nueva_ruta, "w");
	if (!archivo_mision) {
		perror("Error de apertura de archivo.\n");
		return ERROR_DE_APERTURA;
	}

	FILE* archivo_censo = fopen(RUTA_CENSO, "r");
	if (!archivo_censo) {
		perror("Error de apertura de archivo.\n");
		fclose(archivo_censo);
		return ERROR_DE_APERTURA;
	}

	cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano.nombre, &enano.edad, &enano.cantidad_misiones, &enano.id_rango);
	if (!cantidad_leidos) {
		perror("Error de lectura en el archivo.\n");
		fclose(archivo_mision);
		fclose(archivo_censo);
		return ERROR_DE_ESCRITURA;
	}

	while (cantidad_leidos != EOF) {
		if (enano.id_rango == GUERRERO && contador_guerreros < cantidad_guerreros) {
			fwrite(&enano, sizeof(enano_t), 1, archivo_mision);
			contador_guerreros++;
		} else if (enano.id_rango == LIDER && contador_lideres < cantidad_lideres) {
			fwrite(&enano, sizeof(enano_t), 1, archivo_mision);
			contador_lideres++;
		} else if (enano.id_rango == GENERAL && contador_generales < cantidad_generales) {
			fwrite(&enano, sizeof(enano_t), 1, archivo_mision);
			contador_generales++;
		} else if (enano.id_rango == APRENDIZ && contador_aprendices < CANT_APRENDICES_MAX) {
			fwrite(&enano, sizeof(enano_t), 1, archivo_mision);
			contador_aprendices++;
		}
	cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano.nombre, &enano.edad, &enano.cantidad_misiones, &enano.id_rango);
	}

	if (contador_guerreros < cantidad_guerreros || contador_lideres < cantidad_lideres || contador_generales < cantidad_generales) {
		perror("No hay enanos suficientes.");
		fclose(archivo_mision);
		fclose(archivo_censo);
		remove(nueva_ruta);
		return INSUFICIENTES_ENANOS;
	}

	printf("El archivo %s fue creado exitosamente.\n", nueva_ruta);

	fclose(archivo_mision);
	fclose(archivo_censo);
	return 0;
}

/* Pre: Recibe un enano cargado. 
Post: Devuelve si cumple los requisitos para ser guerrero. */
bool puede_ser_guerrero(enano_t enano) {
	bool cumple_requisitos = false;

	if (enano.id_rango == APRENDIZ && enano.cantidad_misiones > REQUISITO_MISIONES_GUERRERO) {
		cumple_requisitos = true;
	}

	return cumple_requisitos;
}

/* Pre: Recibe un enano cargado. 
Post: Devuelve si cumple los requisitos para ser líder. */
bool puede_ser_lider(enano_t enano) {
	bool cumple_requisitos = false;

	if (enano.id_rango == GUERRERO && enano.cantidad_misiones > REQUISITO_MISIONES_LIDER && enano.edad >= REQUISITO_EDAD_LIDER_MIN && enano.edad < REQUISITO_EDAD_LIDER_MAX) {
		cumple_requisitos = true;
	}

	return cumple_requisitos;
}

/* Pre: Recibe un enano cargado. 
Post: Devuelve si cumple los requisitos para ser general. */
bool puede_ser_general(enano_t enano) {
	bool cumple_requisitos = false;

	if (enano.id_rango == LIDER && enano.cantidad_misiones > REQUISITO_MISIONES_GENERAL && enano.edad >= REQUISITO_EDAD_GENERAL_MIN && enano.edad < REQUISITO_EDAD_GENERAL_MAX) {
		cumple_requisitos = true;
	}

	return cumple_requisitos;
}

int promover_enanos() {
	int cantidad_leidos = 0;
	enano_t enano;

	FILE* archivo_censo = fopen(RUTA_CENSO, "r");
	if (!archivo_censo) {
		perror("Error de apertura de archivo.\n");
		return ERROR_DE_APERTURA;
	}

	FILE* archivo_temporal = fopen(RUTA_TEMPORAL, "w");
	if (!archivo_temporal) {
		fclose(archivo_censo);
		perror("Error de apertura de archivo.\n");
		return ERROR_DE_APERTURA;
	}	

	cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano.nombre, &enano.edad, &enano.cantidad_misiones, &enano.id_rango);
	if (!cantidad_leidos) {
		perror("Error de lectura en el archivo.\n");
		fclose(archivo_censo);
		fclose(archivo_temporal);
		remove(RUTA_TEMPORAL);
		return ERROR_DE_ESCRITURA;
	}
	while (cantidad_leidos != EOF) {
		if (puede_ser_guerrero(enano)) {
			cantidad_leidos = fprintf(archivo_temporal, "%s;%i;%i;%i\n", enano.nombre, enano.edad, enano.cantidad_misiones, enano.id_rango = GUERRERO);
		} else if (puede_ser_lider(enano)) {
			cantidad_leidos = fprintf(archivo_temporal, "%s;%i;%i;%i\n", enano.nombre, enano.edad, enano.cantidad_misiones, enano.id_rango = LIDER);
		} else if (puede_ser_general(enano)) {
			cantidad_leidos = fprintf(archivo_temporal, "%s;%i;%i;%i\n", enano.nombre, enano.edad, enano.cantidad_misiones, enano.id_rango = GENERAL);
		} else {
			cantidad_leidos = fprintf(archivo_temporal, "%s;%i;%i;%i\n", enano.nombre, enano.edad, enano.cantidad_misiones, enano.id_rango);
		}
		cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano.nombre, &enano.edad, &enano.cantidad_misiones, &enano.id_rango);
	}

	printf("Los enanos fueron promovidos exitosamente.\n");

	fclose(archivo_censo);
	fclose(archivo_temporal);

	remove(RUTA_CENSO);
 	rename(RUTA_TEMPORAL, RUTA_CENSO);

	return 0;
}

int listar_enanos(char* argumento) {
	size_t leidos = 0;
	int cantidad_leidos = 0;
	int numero = atoi(argumento);
	rango_t rango;
	enano_t enano;

	FILE* archivo_rangos = fopen(RUTA_RANGOS, "r");
	if (!archivo_rangos) {
		perror("Error de apertura de archivo.\n");
		return ERROR_DE_APERTURA;
	}

	FILE* archivo_censo = fopen(RUTA_CENSO, "r");
	if (!archivo_censo) {
		perror("Error de apertura de archivo.\n");
		fclose(archivo_rangos);
		return ERROR_DE_APERTURA;
	}
	
	leidos = fread(&rango.id, sizeof(rango_t), 1, archivo_rangos);
	if (!leidos) {
		perror("Error de lectura en el archivo.\n");
		fclose(archivo_rangos);
		fclose(archivo_censo);
		return ERROR_DE_LECTURA;
	}
    while (!feof(archivo_rangos)) {
    	if (rango.id == numero) {
  			printf("%s\n", rango.nombre);
  			printf("%s\n", rango.descripcion);
    	}
        leidos = fread(&rango.id, sizeof(rango_t), 1, archivo_rangos);
    }

    cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano.nombre, &enano.edad, &enano.cantidad_misiones, &enano.id_rango);
    if (!cantidad_leidos) {
    	perror("Error de lectura en el archivo.\n");
    	fclose(archivo_rangos);
		fclose(archivo_censo);
    	return ERROR_DE_LECTURA;
    }
    while (cantidad_leidos != EOF) {
    	if (enano.id_rango == numero) {
    		printf("Nombre: %s\n", enano.nombre);
    		printf("Edad: %i\n", enano.edad);
    		printf("Cantidad de misiones: %i\n", enano.cantidad_misiones);
    	}
    	cantidad_leidos = fscanf(archivo_censo, "%[^;];%i;%i;%i\n", enano.nombre, &enano.edad, &enano.cantidad_misiones, &enano.id_rango);
    }

	fclose(archivo_rangos);
	fclose(archivo_censo);

	return 0;
}

void mostrar_comandos() {
	printf("Para actualizar el censo: %s MISIÓN\nPara elegir a los integrantes de una misión: %s GUERRERO LÍDER GENERAL MISIÓN\nPara promover a los enanos: %s\nPara listar a los enanos: %s RANGO\n", COMANDO_ACTUALIZAR_CENSO, COMANDO_ELEGIR_ENANOS, COMANDO_PROMOVER_ENANOS, COMANDO_LISTAR_ENANOS);
}

void mostrar_mensaje() {
	printf("Ingresaste un comando incorrecto.\n");
}

bool es_actualizar_censo(char* argumento1) {
	bool es_comando_correcto = false;

	if (strcmp(argumento1, COMANDO_ACTUALIZAR_CENSO) == COINCIDENCIA) {
		es_comando_correcto = true;
	}

	return es_comando_correcto;
}

bool es_elegir_enanos(char* argumento) {
	bool es_comando_correcto = false;

	if (strcmp(argumento, COMANDO_ELEGIR_ENANOS) == COINCIDENCIA) {
		es_comando_correcto = true;
	}

	return es_comando_correcto;
}

bool es_promover_enanos(char* argumento) {
	bool es_comando_correcto = false;

	if (strcmp(argumento, COMANDO_PROMOVER_ENANOS) == COINCIDENCIA) {
		es_comando_correcto = true;
	}

	return es_comando_correcto;
}

bool es_listar_enanos(char* argumento1, char* argumento2) {
	bool es_comando_correcto = false;
	int numero = atoi(argumento2);

	if ((strcmp(argumento1, COMANDO_LISTAR_ENANOS) == COINCIDENCIA) && ((numero >= 1) && (numero) <= 5)) {
		es_comando_correcto = true;
	}

	return es_comando_correcto;
}

bool es_mostrar_comandos(char* argumento) {
	bool es_comando_correcto = false;

	if (strcmp(argumento, COMANDO_MOSTRAR_COMANDOS) == COINCIDENCIA) {
		es_comando_correcto = true;
	}

	return es_comando_correcto;
}