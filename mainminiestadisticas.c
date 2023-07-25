#include <stdio.h>
#include "miniestadisticas.h"

#define UN_ARGUMENTO 2
#define DOS_ARGUMENTOS 3
#define CUATRO_ARGUMENTOS 5
#define CINCO_ARGUMENTOS 6
#define NOMBRE_POR_DEFECTO "mision"

int main(int argc, char* argv[]) {
	switch(argc) {
		case UN_ARGUMENTO:
			if (es_promover_enanos(argv[1])) {
				promover_enanos();
				break;
			}  else if (es_mostrar_comandos(argv[1])) {
				mostrar_comandos();
				break;
			} else {
				mostrar_mensaje();
				break;
			}
		case DOS_ARGUMENTOS:
			if (es_actualizar_censo(argv[1])) {
				actualizar_censo(argv[2]);
				break;
			} else if (es_listar_enanos(argv[1], argv[2])) {
				listar_enanos(argv[2]);
				break;
			} else {
				mostrar_mensaje();
				break;
			}
		case CUATRO_ARGUMENTOS:
			if (es_elegir_enanos(argv[1])) {
				elegir_enanos(argv[2], argv[3], argv[4], NOMBRE_POR_DEFECTO);
				break;
			} else {
				mostrar_mensaje();
				break;
			}
		case CINCO_ARGUMENTOS:
			if (es_elegir_enanos(argv[1])) {
				elegir_enanos(argv[2], argv[3], argv[4], argv[5]);
				break;
			} else {
				mostrar_mensaje();
				break;
			}
		default:
			mostrar_mensaje();
			break;
	}

	return 0;
}