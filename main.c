#include <sys/sem.h> //libreria para semaforos
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <time.h>

/* netbd.h es necesitada por la estructura hostent ;-) */
#define PORT 3550
/* El Puerto Abierto del nodo remoto */
#define MAXDATASIZE 100
/* El número máximo de datos en bytes */

/* Prototipo de funciones */
void insert_user(int argc, char *argv[], char cadena[100]);

/* Menu */
void ventas();
void clientes();
void categorias();
void productos();
void reportes();


clock_t t_ini, t_fin;
double total;

int main(int argc, char *argv[])
{

	int opc = 0;

	do{
		printf("\n\t1.-Ventas\n\n");
		printf("\t2.-Clientes\n\n");
		printf("\t3.-Categorias\n\n");
		printf("\t4.-Productos\n\n");
		printf("\t5.-Reportes\n\n");
		printf("\t6.-Salir\n\n");

		printf("\nElige una opcion:");
		scanf("%d", &opc);
		system("clear");

		switch (opc){

			case 1:
				ventas();
				break;

			case 2:
				clientes(argc, argv);
				break;

			case 3:
				categorias();
				break;

			case 4:
				productos();
				break;

			case 5:
				reportes(argc, argv);
				break;

			case 6:
				exit(0);
				break;

			default:
				fflush(stdin);
		}

	} while (opc != 6);
}

int executeServidor(int argc, char *argv[], char comando[])
{

	int fd, numbytes;
	/* ficheros descriptores */
	char envio[MAXDATASIZE], recibe[MAXDATASIZE];
	/* en donde es almacenará el texto recibido */
	struct hostent *he;
	/* estructura que recibirá información sobre el nodo remoto */
	struct sockaddr_in server;
	/* información sobre la dirección del servidor */
	if (argc != 2){
		/* esto es porque nuestro programa sólo necesitará un
		argumento, (la IP) */
		printf("Uso: %s <Dirección IP>\n", argv[0]);
		exit(-1);
	}

	if ((he = gethostbyname(argv[1])) == NULL){
		/* llamada a gethostbyname() */
		printf("gethostbyname() error\n");
		exit(-1);
	}

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
		/* llamada a socket() */
		printf("socket() error\n");
		exit(-1);
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	/* htons() es necesaria nuevamente ;-o */
	server.sin_addr = *((struct in_addr *)he->h_addr);
	/*he->h_addr pasa la información de ``*he'' a "h_addr" */
	bzero(&(server.sin_zero), 8);

	if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1){
		/* llamada a connect() */
		printf("connect() error\n");
		exit(-1);
	}

	bzero(envio, sizeof(envio));
	bzero(recibe, sizeof(recibe));

	char mensaje[200];

	copiar(mensaje, comando);

	write(fd, mensaje, sizeof(mensaje));

	read(fd, recibe, sizeof(recibe));

	close(fd); /* cerramos fd =) */

	if (strstr(recibe, "200")){
		//printf("\nOperacion exitosa...\n");
		return 200;
	}else if(strstr(recibe, "404")){
		//printf("\nRecurso no encontrado...");
		return 404;
	}else{
		printf("\nOcurrio un error...");
	}

	return 500;
}

void executeServidorSelects(int argc, char *argv[], char comando[])
{

	int fd, numbytes;
	/* ficheros descriptores */
	char envio[MAXDATASIZE], recibe[MAXDATASIZE];
	/* en donde es almacenará el texto recibido */
	struct hostent *he;
	/* estructura que recibirá información sobre el nodo remoto */
	struct sockaddr_in server;
	/* información sobre la dirección del servidor */
	if (argc != 2)
	{
		/* esto es porque nuestro programa sólo necesitará un
		argumento, (la IP) */
		printf("Uso: %s <Dirección IP>\n", argv[0]);
		exit(-1);
	}
	if ((he = gethostbyname(argv[1])) == NULL)
	{
		/* llamada a gethostbyname() */
		printf("gethostbyname() error\n");
		exit(-1);
	}
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		/* llamada a socket() */
		printf("socket() error\n");
		exit(-1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	/* htons() es necesaria nuevamente ;-o */
	server.sin_addr = *((struct in_addr *)he->h_addr);
	/*he->h_addr pasa la información de ``*he'' a "h_addr" */
	bzero(&(server.sin_zero), 8);

	if (connect(fd, (struct sockaddr *)&server, sizeof(struct sockaddr)) == -1)
	{
		/* llamada a connect() */
		printf("connect() error\n");
		exit(-1);
	}

	bzero(envio, sizeof(envio));
	bzero(recibe, sizeof(recibe));

	char mensaje[200];

	copiar(mensaje, comando);

	write(fd, mensaje, sizeof(mensaje));

	while (1){
		
		read(fd, recibe, sizeof(recibe));

		if(strstr(recibe,"terminar")){
			break;
		}else{
			printf("%s", recibe);
		}
	}

	close(fd); /* cerramos fd =) */
}

void copiar(char *destino, char *origen)
{
	while (*destino++ = *origen++)
		;
}

/** ======== Funciones de MENU ======== */

void ventas()
{
	int opcVentas;
	do
	{
		///////////////////////////MENU DE VENTAS///////////////////////////
		printf("VENTAS");
		printf("\n\n\t1.-Crear Venta\n\n");
		printf("\t2.-Abonos\n\n");
		printf("\t3.-Consultar Ventas\n\n");
		printf("\t4.-Mofificar Ventas *\n\n");
		printf("\t5.-Regresar al menu principal\n\n");

		printf("Elige una opcion:");
		scanf("%d", &opcVentas);
		system("clear");

		switch (opcVentas)
		{
		case 1:
			printf("\n\n\n\n\tCrear Venta\n");
			break;

		case 2:
			printf("\n\n\n\n\tAbonos\n");
			break;

		case 3:
			printf("\n\n\n\n\tConsultar Ventas\n");
			break;

		case 4:
			printf("\n\n\n\n\tModificar Ventas\n");
			break;

		case 5:
			break;

		default:
			fflush(stdin);
		}

	} while (opcVentas != 5);
}

void clientes(int argc, char *argv[])
{

	int opcClientes, respuesta;

	char nombre[100], paterno[100], materno[100], telefono[100];
	int cliente_id;
	char cadena[200];
	float limite_credito;

	do
	{

		printf("CLIENTES");
		printf("\n\n\t1.-Alta Clientes\n\n");
		printf("\t2.-Consulta de Clientes\n\n");
		printf("\t3.-Modificacion de Datos del Cliente\n\n");
		printf("\t4.-Baja de Clientes\n\n");
		printf("\t5.-Regresar al menu principal\n\n");

		printf("Elige una opcion:");
		scanf("%d", &opcClientes);
		system("clear");

		switch (opcClientes){
			case 1:

				printf("\n\n\n\n\tAlta Clientes\n\n");

				printf("Nombre: ");
				scanf("%s", nombre);

				printf("Apellido paterno: ");
				scanf("%s", paterno);

				printf("Apellido materno: ");
				scanf("%s", materno);

				printf("Telefono: ");
				scanf("%s", telefono);

				printf("Limite de credito: ");
				scanf("%f", &limite_credito);

				sprintf(cadena, "insert|INSERT INTO clientes (nombres, aPaterno, aMaterno, telefono, limite_credito) VALUES ('%s', '%s', '%s', '%s', %f);", nombre, paterno, materno, telefono, limite_credito);

				t_ini = clock();
				respuesta = executeServidor(argc, argv, cadena);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

				if(respuesta == 200){
					printf("\n==== CLIENTE CREADO ====\n\n");
				}else{
					printf("\n==== CLIENTE NO CREADO :( ====\n\n");
				}

			break;

			case 2:
				printf("\n\n\n\n\tConsulta de Clientes\n\n");

				sprintf(cadena, "select|SELECT * FROM clientes;");

				t_ini = clock();
				executeServidorSelects(argc, argv, cadena);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

			break;

			case 3:
				printf("\n\n\n\n\tModificacion de datos de Clientes\n\n");

				printf("Dame el ID del cliente: ");
				scanf("%d", &cliente_id);
				sprintf(cadena, "findById|SELECT * FROM clientes WHERE id_cliente = %d;", cliente_id);

				t_ini = clock();
				respuesta = executeServidor(argc, argv, cadena);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);
				

				if(respuesta == 200){

					printf("Cliente encontrado\n\n");

					printf("Nombre: ");
					scanf("%s", nombre);

					printf("Apellido paterno: ");
					scanf("%s", paterno);

					printf("Apellido materno: ");
					scanf("%s", materno);

					printf("Telefono: ");
					scanf("%s", telefono);

					printf("Limite de creadito: ");
					scanf("%f", &limite_credito);

					sprintf(cadena, "insert|UPDATE clientes set nombres = '%s', aPaterno = '%s', aMaterno = '%s', telefono = '%s', limite_credito = %f where id_cliente = %d", nombre, paterno, materno, telefono, limite_credito, cliente_id);

					t_ini = clock();
					respuesta = executeServidor(argc, argv, cadena);
					t_fin = clock();
					total= t_fin - t_ini;
					printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

					printf("\n ==== Cliente modificado correctamente ====\n\n");

				}else{
					printf("\n ==== Cliente no encontrado ====\n\n");
				}

				break;

			case 4:
				printf("\n\n\n\n\tBaja de Clientes\n\n");

				printf("Dame el ID del cliente: ");
				scanf("%d", &cliente_id);
				sprintf(cadena, "findById|SELECT * FROM clientes WHERE id_cliente = %d;", cliente_id);

				respuesta = executeServidor(argc, argv, cadena);

				if(respuesta == 200){

					sprintf(cadena, "insert|DELETE FROM clientes where id_cliente = %d", cliente_id);

					t_ini = clock();
					respuesta = executeServidor(argc, argv, cadena);
					t_fin = clock();
					total= t_fin - t_ini;
					printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

					printf("\n==== Cliente eliminado ==== \n\n");

				}else{
					printf("\n==== Cliente no encontrado :( ====\n\n");
				}
				break;

			case 5:
				break;

			default:
				fflush(stdin);
		}

	} while (opcClientes != 5);
}

void categorias()
{
	int opcCategorias;
	do
	{
		///////////////////////////MENU DE CATEGORIAS///////////////////////////
		printf("CATEGORIAS");
		printf("\n\n\t1.-Alta Categorias\n\n");
		printf("\t2.-Consulta de Categorias\n\n");
		printf("\t3.-Modificacion de Datos de Categorias\n\n");
		printf("\t4.-Baja de Categorias\n\n");
		printf("\t5.-Regresar al menu principal\n\n");

		printf("Elige una opcion:");
		scanf("%d", &opcCategorias);
		system("clear");

		switch (opcCategorias)
		{
		case 1:
			printf("\n\n\n\n\tAlta de Categorias\n");
			break;

		case 2:
			printf("\n\n\n\n\tConsulta de Categorias\n");
			break;

		case 3:
			printf("\n\n\n\n\tModificacion de datos de Categorias\n");
			break;

		case 4:
			printf("\n\n\n\n\tBaja de Categorias\n");
			break;

		case 5:
			break;

		default:
			fflush(stdin);
		}

	} while (opcCategorias != 5);
}

void productos()
{
	int opcProducto;
	do
	{
		printf("PRODUCTOS");
		printf("\n\n\t1.-Alta de Productos\n\n");
		printf("\t2.-Consulta de Productos\n\n");
		printf("\t3.-Modificacion de datos de Productos\n\n");
		printf("\t4.-Baja de Productos\n\n");
		printf("\t5.- Salir\n\n");

		printf("Elige una opcion:");
		scanf("%d", &opcProducto);
		system("clear");

		switch (opcProducto)
		{
		case 1:
			printf("\n\n\n\n\tAlta de Productos\n");
			break;

		case 2:
			printf("\n\n\n\n\tConsulta de Productos\n");
			break;

		case 3:
			printf("\n\n\n\n\tModificacion de datos de Productos\n");
			break;

		case 4:
			printf("\n\n\n\n\tBaja de Productos\n");
			break;

		case 5:
			break;

		default:
			fflush(stdin);

		} //FIN SWITCH

	} while (opcProducto != 5);
} //FIN VOID PRODUCTOS

void reportes(int argc, char *argv[])
{

	int opcReportes, opcF, opcV, opcC, opcP, opcCF, opcCF2, opcC1, opcC2, opcC3;
	char cadena[200];

	do
	{
		printf("REPORTES\n");
		printf("\n\n\t1.-Facturas\n\n");
		printf("\t2.-Ventas\n\n");
		printf("\t3.-Clientes\n\n");
		printf("\t4.-Productos\n\n");
		printf("\t5.- Salir\n\n");

		printf("Elige una opcion:");
		scanf("%d", &opcReportes);
		system("clear");

		switch (opcReportes)
		{
		case 1:

			do
			{
				printf("FACTURAS\n");
				printf("\n\n\t1.-Crear Facturas (Credito)\n\n");
				printf("\t2.-Crear Facturas (Contado)\n\n");
				printf("\t3.-Consultar Facturas\n\n");
				printf("\t4.-Salir\n\n");

				printf("Elige una opcion:");
				scanf("%d", &opcF);
				system("clear");

				switch (opcF)
				{
				case 1:
					printf("\n\n\n\n\tCrear Facturas (Credito)\n");

					break;

				case 2:
					printf("\n\n\n\n\tCrear Facturas (Contado)\n");

					break;

				case 3:

					do
					{
						printf("Consultar Facturas\n");
						printf("\n\n\t1.-Credito\n\n");
						printf("\t2.-Contado\n\n");
						printf("\t3.-Ver todas\n\n");
						printf("\t4.-Salir\n\n");

						printf("Elige una opcion:");
						scanf("%d", &opcCF2);
						system("clear");

						switch (opcCF2)
						{
						case 1:
							printf("\n\n\n\n\tCredito\n");

							break;

						case 2:
							printf("\n\n\n\n\tContado\n");

							break;

						case 3:
							printf("\n\n\n\n\tVer todas\n");

							break;

						case 4:
							break;

						default:
							fflush(stdin);
						}
					} while (opcCF2 != 4);

					break;

				case 4:
					break;

				default:
					fflush(stdin);
				}

			} while (opcF != 4);

			system("clear");
			break;
		case 2:

			do{
				printf("VENTAS\n");
				printf("\n\n\t1.-Consultar ventas (Credito)\n\n");
				printf("\t2.-Consultar ventas (Contado)\n\n");
				printf("\t3.-Ver todas\n\n");
				printf("\t4.-Salir\n\n");

				printf("Elige una opcion:");
				scanf("%d", &opcV);
				system("clear");

				switch (opcV){
					case 1:
						printf("\n\n\n\n\t===== VENTAS A CREDITO =====\n\n");

						sprintf(cadena, "ventas|select * from ventas v inner join clientes c on c.id_cliente = v.id_cliente where v.credito = true;");

						t_ini = clock();
						executeServidorSelects(argc, argv, cadena);
						t_fin = clock();
						total= t_fin - t_ini;
						printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

						break;

					case 2:
						printf("\n\n\n\n\t ==== VENTAS DE CONTADO =====\n");

						sprintf(cadena, "ventas|select * from ventas v inner join clientes c on c.id_cliente = v.id_cliente where v.credito = false;");

						t_ini = clock();
						executeServidorSelects(argc, argv, cadena);
						t_fin = clock();
						total= t_fin - t_ini;
						printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

						break;

					case 3:
						printf("\n\n\n\n\t===== TODAS LAS VENTAS =====\n");

						sprintf(cadena, "ventas|select * from ventas v inner join clientes c on c.id_cliente = v.id_cliente;");

						t_ini = clock();
						executeServidorSelects(argc, argv, cadena);
						t_fin = clock();
						total= t_fin - t_ini;
						printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

						break;

					case 4:
						break;

					default:
						fflush(stdin);
				}

			} while (opcV != 4);

			system("clear");
			break;
		case 3:

			do
			{
				printf("CLIENTES\n");
				printf("\n\n\t1.-Consultar clientes\n\n");
				printf("\t2.-Descuento de clientes\n\n");
				printf("\t3.-Creditos de clientes\n\n");
				printf("\t4.-Salir\n\n");

				printf("Elige una opcion:");
				scanf("%d", &opcC);
				system("clear");

				switch (opcC)
				{
				case 1:
					do
					{
						printf("Consultar clientes\n");
						printf("\n\n\t1.-Clientes con credito\n\n");
						printf("\t2.-Clientes con descuentos\n\n");
						printf("\t3.-Ver todos\n\n");
						printf("\t4.-Salir\n\n");

						printf("Elige una opcion:");
						scanf("%d", &opcC1);
						system("clear");

						switch (opcC1)
						{
						case 1:
							printf("\n\n\n\n\tClientes con credito\n");

							break;

						case 2:
							printf("\n\n\n\n\tClientes con descuentos\n");

							break;

						case 3:
							printf("\n\n\n\n\tVer todos\n");

							break;

						case 4:
							break;

						default:
							fflush(stdin);
						}
					} while (opcC1 != 4);
					break;

				case 2:
					do
					{
						printf("Descuento de clientes\n");
						printf("\n\n\t1.-Por mes\n\n");
						printf("\t2.-Ver todos\n\n");
						printf("\t3.-Salir\n\n");

						printf("Elige una opcion:");
						scanf("%d", &opcC2);
						system("clear");

						switch (opcC2)
						{
						case 1:
							printf("\n\n\n\n\tPor mes\n");

							break;

						case 2:
							printf("\n\n\n\n\tVer todos\n");

							break;

						case 3:
							break;

						default:
							fflush(stdin);
						}
					} while (opcC2 != 3);
					break;

				case 3:

					do
					{
						printf("Creditos de clientes\n");
						printf("\n\n\t1.-Por mes\n\n");
						printf("\t2.-Ver todos\n\n");
						printf("\t3.-Salir\n\n");

						printf("Elige una opcion:");
						scanf("%d", &opcC3);
						system("clear");

						switch (opcC3)
						{
						case 1:
							printf("\n\n\n\n\tPor mes\n");

							break;

						case 2:
							printf("\n\n\n\n\tVer todos\n");

							break;

						case 3:
							break;

						default:
							fflush(stdin);
						}
					} while (opcC3 != 3);

					break;

				case 4:
					break;

				default:
					fflush(stdin);
				}

			} while (opcC != 4);

			system("clear");
			break;
		case 4:

			do{
				printf("\n\t==== REPORTE PRODUCTOS ====\n");
				printf("\n\n\t1.-Ver productos mas vendidos\n\n");
				printf("\t2.-Ver productos menos vendidos\n\n");
				printf("\t3.-Ver productos en descuentos\n\n");
				printf("\t4.-Ver productos a credito\n\n");
				printf("\t5.-Ver todos\n\n");
				printf("\t6.-Salir\n\n");

				printf("Elige una opcion:");
				scanf("%d", &opcP);
				system("clear");

				switch (opcP){

					case 1:
						printf("\n\n\n\n\t ====== VER PRODUCTOS MAS VENDIDOS ======\n");

						sprintf(cadena, "reporte_productos|select m.id_mat,  m.nombre, sum(unidades) as cantidad from detalle_venta dv inner join materiales m on m.id_mat = dv.id_mat group by m.id_mat order by cantidad desc limit 1;");

						t_ini = clock();
						executeServidorSelects(argc, argv, cadena);
						t_fin = clock();
						total= t_fin - t_ini;
						printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

						break;

					case 2:
						printf("\n\n\n\n\t===== VER PRODUCTOS MENOS VENDIDOS =====\n");

						sprintf(cadena, "reporte_productos|select m.id_mat,  m.nombre, sum(unidades) as cantidad from detalle_venta dv inner join materiales m on m.id_mat = dv.id_mat group by m.id_mat order by cantidad limit 1;");

						t_ini = clock();
						executeServidorSelects(argc, argv, cadena);
						t_fin = clock();
						total= t_fin - t_ini;
						printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);


						break;

					case 3:
						printf("\n\n\n\n\tVer productos en descuentos\n");

						break;

					case 4:
						printf("\n\n\n\n\tVer productos a credito\n");

						break;

					case 5:
						printf("\n\n\n\n\tVer todos\n");

						break;

					case 6:
						break;

					default:
						fflush(stdin);
				}

			} while (opcP != 6);

			system("clear");
			break;

		case 5:
			break;

		default:
			fflush(stdin);

		} //FIN SWITCH

	} while (opcReportes != 5);
}