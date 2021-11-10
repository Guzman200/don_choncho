/* Estos son los ficheros de cabecera usuales */
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <string.h>
#include <libpq-fe.h>
#include <sys/sem.h> //libreria para semaforos

#define PORT 3550 /* El puerto que será abierto */
#define BACKLOG 2 /* El número de conexiones permitidas */
#define MAXDATASIZE 500

/*
  Funciones con semaforo
  semget: Crea el semaforo
  semctl: obtener el control del semaforo
  semop: Aplicar las operaciones de bloquero y desbloquero
*/

#define clave 4000 // de 3000 al infinito
int num, idsem = 0;

int crear_semaforo()
{

    int val = 1;
    if ((idsem = semget(clave, 1, IPC_CREAT | 0777)) == -1) //  esta funcion crea el semaforo debe tener sem.h
    {
        perror("\n No se puede crear el semaforo");
        exit(1);
    }

    if ((semctl(idsem, 0, SETVAL, val)) == -1)
    {
        printf("No se puede tener el control del semaforo");
        exit(1);
    }

    printf("Valor idsem=%d \n", idsem);

    return idsem;

}

void remover_semaforo(int idsem)
{
    if ((semctl(idsem, 0, IPC_RMID)) == -1)
    {
        perror("Error al remover el semaforo");
        exit(1);
    }
    else
        printf("Semaforo eliminado");
}

void up(int idsem)
{
    struct sembuf listo = {0, 1, SEM_UNDO};
    if ((semop(idsem, &listo, 1)) == -1)
    {
        perror("\nError al activar semaforo");
        remover_semaforo(idsem);
        exit(1);
    }
    else
        printf("Semaforo Liberado (Verde)==>");
}

void down(int idsem)                             
{    
	//bloquea el semaforo aqui va lo de actualizar update 
	// .. eliminar insertar meter en el down hacer la consulta y al salir hacer el up(id_sem)                                            
	// -1 bloquea , 1 listo
    struct sembuf bloqueado = {0, -1, SEM_UNDO}; //
    if ((semop(idsem, &bloqueado, 1)) == -1)
    {
        perror("\nError al tratar de bloquear");
        remover_semaforo(idsem);
        exit(1);
    }
    else
        printf("Semaforo Ocupado (Rojo- en RC) ==>");
}


int main()
{
	int fd, fd2; /* los ficheros descriptores */

	char envio[MAXDATASIZE], recibe[MAXDATASIZE];

	int i, j;

	char row[500];

	/* para la información de la dirección del servidor */
	struct sockaddr_in server;

	/* para la información de la dirección del cliente */
	struct sockaddr_in client;

	int sin_size;

	/* A continuación la llamada a socket() */
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("error en socket()\n");
		exit(-1);
	}

	server.sin_family = AF_INET;

	/* ¿Recuerdas a htons() de la sección "Conversiones"? =) */
	server.sin_port = htons(PORT);

	/* INADDR_ANY coloca nuestra dirección IP automáticamente */
	server.sin_addr.s_addr = INADDR_ANY;

	/* escribimos ceros en el reto de la estructura */
	/* A continuación la llamada a bind() */
	bzero(&(server.sin_zero), 8);

	if (bind(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		printf("error en bind() \n");
		exit(-1);
	}

	if (listen(fd, BACKLOG) == -1)
	{ /* llamada a listen() */
		printf("error en listen()\n");
		exit(-1);
	}

	while (1)
	{

		sin_size = sizeof(struct sockaddr_in);

		/* A continuación la llamada a accept() */
		if ((fd2 = accept(fd, (struct sockaddr *)&client, &sin_size)) == -1)
		{
			printf("error en accept()\n");
			exit(-1);
		}

		/* que mostrará la IP del cliente */
		printf("Se obtuvo una conexión desde %s\n", inet_ntoa(client.sin_addr));

		bzero(envio, sizeof(envio));
		bzero(recibe, sizeof(recibe));

		read(fd2, recibe, sizeof(recibe));

		printf("Peticion: %s\n", recibe);

		//execute(recibe);

		//write(fd2, "200", 2);

		char delimitador[] = "|";

		char *token = strtok(recibe, delimitador);

		if (token != NULL)
		{
			/* Si la instruccion es igual a insertar usuario */
			if (strstr(token, "insert")){

				idsem = crear_semaforo();

				if (idsem < 0){
					perror("El semaforo no existe\n");
					exit(0);
				}

				down(idsem);
				printf("\n ==== En region critica ====\n");

				

				token = strtok(NULL, delimitador); // obtenemos el comando sql
				execute(token);
				write(fd2, "200", 3);

				up(idsem);
				printf("\n ==== Saliendo de region critica ==== \n");

			}else if(strstr(token, "findById")){

				token = strtok(NULL, delimitador); // obtnemos el comando sql

				idsem = crear_semaforo();

				if (idsem < 0){
					perror("El semaforo no existe\n");
					exit(0);
				}

				down(idsem);
				printf("\n ==== En region critica ====\n");

				int respuesta = findById(token);

				if(respuesta == 1){
					write(fd2, "200", 3);
				}else{
					write(fd2, "404", 3);
				}

				up(idsem);
				printf("\n ==== Saliendo de region critica ==== \n");

			}else if(strstr(token, "select")){

				token = strtok(NULL, delimitador); // obtenemos el comando sql

				PGconn *conn;
				PGresult *ress;

				idsem = crear_semaforo();

				if (idsem < 0){
					perror("El semaforo no existe\n");
					exit(0);
				}

				down(idsem);
				printf("\n ==== En region critica ====\n");

				conn = PQsetdbLogin("localhost", "5432", NULL, NULL, "don_concho", "postgres", "123456");

				if (PQstatus(conn) != CONNECTION_BAD){

					printf("Conectado a la base de datos\n");

					ress = PQexec(conn, token);

					if(ress != NULL){

						sprintf(row, "ID  |        NOMBRE COMPLETO        |   TELEFONO    |   REGISTRO   |     LIMITE CREDITO");
						write(fd2, row, sizeof(row));

						for (i = 0; i < PQntuples(ress); i++){ //filas

							sprintf(row, "%s   |%s %s %s | %s | %s | %s", PQgetvalue(ress,i,0),  PQgetvalue(ress,i,1), PQgetvalue(ress,i,2), PQgetvalue(ress,i,3), PQgetvalue(ress,i,4), PQgetvalue(ress,i,5),  PQgetvalue(ress,i,6));

							write(fd2, row, sizeof(row));
						}

						write(fd2, "terminar", 8);

					}else{
						printf("Error al conectar a la base de datos");
					}
				}

				up(idsem);
				printf("\n ==== Saliendo de region critica ==== \n");
			}
		}
	}
}

/* Modificaciones e insercciones **/
void execute(char *sql)
{
	PGconn *conn;
	PGresult *ress;

	conn = PQsetdbLogin("localhost", "5432", NULL, NULL, "don_concho", "postgres", "123456");

	if (PQstatus(conn) != CONNECTION_BAD){

		printf("Conectado a la base de datos\n");

		//sprintf(cad, "INSERT INTO usuarios VALUES ('%s', '%s', '%s')", nombre, apellido, telefono);

		ress = PQexec(conn, sql);

		if (PQresultStatus(ress) == PGRES_COMMAND_OK){
			printf("Operacion exitosa");
		}
		else{
			printf("%s", PQresultErrorMessage(ress));
		}

		//PQclear(res);

	}else{
		printf("Error al conectar a la base de datos");
	}
}

/* Verificacion de recursos **/
int findById(char *sql)
{
	PGconn *conn;
	PGresult *ress;

	conn = PQsetdbLogin("localhost", "5432", NULL, NULL, "don_concho", "postgres", "123456");

	if (PQstatus(conn) != CONNECTION_BAD){

		printf("Conectado a la base de datos\n");

		ress = PQexec(conn, sql);

		if (PQntuples(ress) == 1){
			return 1;
		}

	}else{
		printf("Error al conectar a la base de datos");
	}

	return 0;
}

