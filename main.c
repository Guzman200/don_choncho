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
#define MAXDATASIZE 500
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
char tabu;

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
				ventas(argc, argv);
				break;

			case 2:
				clientes(argc, argv);
				break;

			case 3:
				categorias(argc, argv);
				break;

			case 4:
				productos(argc, argv);
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
	}else if(strstr(recibe, "201")){
		//printf("\nOperacion exitosa...\n");
		return 201;
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

	char mensaje[500];

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

void selectMesOTodosClientes(int argc, char *argv[], int ban, int mes)
{
	char cadenaC[500];
	/*ban = bandera utilizada para crear los distintos tipos de consultas dentro de reportes, Clientes*/
	switch (ban)
	{
	case 1: /*CLIENTES CON DESCUENTO POR MES*/
		sprintf(cadenaC, "select|SELECT DISTINCT c.id_cliente,c.nombres, c.aPaterno, c.aMaterno, c.telefono, c.fecha_registro, c.limite_credito FROM clientes c LEFT JOIN ventas v ON v.id_cliente = c.id_cliente LEFT JOIN detalle_venta dt ON dt.id_venta = v.id_venta WHERE dt.porcentaje > 0  AND EXTRACT(MONTH FROM v.fecha_registro) = %d;",mes);
		break;
	
	case 2: /*CLIENTES CON DESCUENTO*/
		sprintf(cadenaC, "select|SELECT DISTINCT c.id_cliente,c.nombres,c.aPaterno,c.aMaterno,c.telefono,c.fecha_registro,c.limite_credito FROM clientes c LEFT JOIN ventas v ON v.id_cliente = c.id_cliente LEFT JOIN detalle_venta dt ON dt.id_venta = v.id_venta WHERE dt.porcentaje > 0;");
		break;
	
	case 3: /*CLIENTES CON CREDITO POR MES*/
		sprintf(cadenaC, "select|SELECT DISTINCT c.id_cliente,c.nombres,c.aPaterno,c.aMaterno,c.telefono,c.fecha_registro,c.limite_credito FROM clientes c LEFT JOIN ventas v ON v.id_cliente = c.id_cliente LEFT JOIN detalle_venta dt ON dt.id_venta = v.id_venta WHERE v.credito = TRUE AND EXTRACT(MONTH FROM v.fecha_registro) = %d;",mes);
		break;

	case 4: /*CLIENTES CON CREDITO*/
		sprintf(cadenaC, "select|SELECT DISTINCT c.id_cliente,c.nombres,c.aPaterno,c.aMaterno,c.telefono,c.fecha_registro,c.limite_credito FROM clientes c LEFT JOIN ventas v ON v.id_cliente = c.id_cliente LEFT JOIN detalle_venta dt ON dt.id_venta = v.id_venta WHERE v.credito = TRUE;");
		break;

	case 5: /*CLIENTES CON VENTAS*/
		sprintf(cadenaC, "select|SELECT DISTINCT c.id_cliente,c.nombres,c.aPaterno,c.aMaterno,c.telefono,c.fecha_registro,c.limite_credito FROM clientes c LEFT JOIN ventas v ON v.id_cliente = c.id_cliente LEFT JOIN detalle_venta dt ON dt.id_venta = v.id_venta;");

	default:
		break;
	}

	t_ini = clock();
	executeServidorSelects(argc, argv, cadenaC);
	t_fin = clock();
	total= t_fin - t_ini;
	printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);
	
}
/** ======== Funciones de MENU ======== */

void ventas(int argc, char *argv[])
{

    int listaMaterial[100],listaCantidad[100];
    int material_ID,cantidadMaterial,cliente_ID;
    char opcProducto;
    char cadena[200];

	int opcVentas,respuesta,opcFormaDePago;
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
			
			int i=0;

            printf("Ingresa el ID del cliente: ");
            scanf("%d",&cliente_ID);

            sprintf(cadena, "findById|SELECT * FROM clientes WHERE id_cliente=%d",cliente_ID);
            respuesta = executeServidor(argc, argv, cadena);

            if(respuesta==200){

                do{
                    printf("Ingresa el ID del material: ");
                    scanf("%d", &material_ID);

                    sprintf(cadena, "findById|SELECT * FROM materiales WHERE id_mat=%d",material_ID);
                    respuesta = executeServidor(argc, argv, cadena);

                    if(respuesta==200){

                        printf("Ingresa la cantidad: ");
                        scanf("%d", &cantidadMaterial);
						sprintf(cadena, "funcStock|SELECT VAL_STOCK(%d,%d);",material_ID,cantidadMaterial);
						respuesta = executeServidor(argc, argv, cadena);

						int bandera=0;

						if(respuesta==200){
							bandera=1;
						}else if(respuesta==201){
							printf("Advertencia: El stock esta por agotarse!");
							bandera=1;
						}else if(respuesta==404){
							printf("No hay stock suficiente!");
							bandera=0;
						}
                        
						if(bandera==1){
                            listaMaterial[i]=material_ID;
							listaCantidad[i]=cantidadMaterial;
                            i++;
						}
                        
                    }else{
                        printf("Material no encontrado!\n");
                    }

                    printf("\nDeseas agregar otro producto? (S/N) ");
                    scanf("%s",&opcProducto);
                }while(opcProducto=='S'||opcProducto=='s');
				
                printf("La compra sera a credito (1) o de contado (2)? ");
                scanf("%d",&opcFormaDePago);

				if(opcFormaDePago==1||opcFormaDePago==2){

					if(opcFormaDePago==1){
						
						sprintf(cadena, "insert|INSERT INTO ventas (id_cliente, total_venta, credito) VALUES (%d, 0, true);", cliente_ID);
						respuesta = executeServidor(argc, argv, cadena);

                        for(int j=0;j<i;j++){
                            sprintf(cadena, "insert|SELECT Insert_DetalleVenta(%d,true,%d);",listaMaterial[j],listaCantidad[j]);
						    respuesta = executeServidor(argc, argv, cadena);
                        }

					}else if(opcFormaDePago==2){
						
						sprintf(cadena, "insert|INSERT INTO ventas (id_cliente, total_venta, credito) VALUES (%d, 0, false);", cliente_ID);
						respuesta = executeServidor(argc, argv, cadena);
						
						for(int j=0;j<i;j++){
                            sprintf(cadena, "insert|SELECT Insert_DetalleVenta(%d,false,%d);",listaMaterial[j],listaCantidad[j]);
						    respuesta = executeServidor(argc, argv, cadena);
                        }

					}

				}else{
					printf("El metodo de pago no existe!\n");
				}

            }else{
                printf("Cliente no encontrado!\n");
            }   

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

	int opcClientes, respuesta, opcEdit;

	char nombre[100], paterno[100], materno[100], telefono[100];
	int cliente_id;
	char cadena[200];
	float limite_credito;

	do
	{

		printf("\t ===== CLIENTES ===== \n");
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
				scanf(" %[^\n]", nombre);

				printf("Apellido paterno: ");
				scanf(" %[^\n]", paterno);

				printf("Apellido materno: ");
				scanf(" %[^\n]", materno);

				printf("Telefono: ");
				scanf(" %[^\n]", telefono);

				printf("Limite de credito: ");
				scanf("%f", &limite_credito);

				sprintf(cadena, "insert|INSERT INTO clientes (nombres, aPaterno, aMaterno, telefono, limite_credito) VALUES ('%s', '%s', '%s', '%s', '%f');", nombre, paterno, materno, telefono, limite_credito);

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
				printf("\n\n\n\n\t ==== CONSULTA DE CLIENTES ====\n\n");

				sprintf(cadena, "select|SELECT * FROM clientes;");

				t_ini = clock();
				executeServidorSelects(argc, argv, cadena);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

			break;

			case 3:
				printf("\n\n\n\n\t ===== MODIFICACION DE CLIENTES ======\n\n");

				printf("Dame el ID del cliente: ");
				scanf("%d", &cliente_id);
				sprintf(cadena, "findById|SELECT * FROM clientes WHERE id_cliente = %d;", cliente_id);

				t_ini = clock();
				respuesta = executeServidor(argc, argv, cadena);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);
				

				if(respuesta == 200){

					printf("==== Cliente encontrado ==== \n\n");

					printf("[1] Nombre\n");
					printf("[2] Apellido paterno\n");
					printf("[3] Apellido materno\n");
					printf("[4] Telefono\n");
					printf("[5] Limite de credito\n\n");
					printf("Opcion: ");
					scanf("%d", &opcEdit);

					printf("\n");

					switch (opcEdit){

						case 1:
							printf("Nombre: ");
							scanf(" %[^\n]", nombre);

							sprintf(cadena, "insert|UPDATE clientes set nombres = '%s' where id_cliente = '%d'", nombre, cliente_id);
							break;

						case 2:
							printf("Apellido paterno: ");
							scanf(" %[^\n]", paterno);
							sprintf(cadena, "insert|UPDATE clientes set aPaterno = '%s' where id_cliente = '%d'", paterno, cliente_id);
							break;

						case 3:
							printf("Apellido materno: ");
							scanf(" %[^\n]", materno);
							sprintf(cadena, "insert|UPDATE clientes set aMaterno = '%s' where id_cliente = '%d'", materno, cliente_id);
							break;

						case 4:
							printf("Telefono: ");
							scanf(" %[^\n]", telefono);
							sprintf(cadena, "insert|UPDATE clientes set telefono = '%s' where id_cliente = '%d'", telefono, cliente_id);
							break;

						case 5:
							printf("Limite de credito: ");
							scanf("%f", &limite_credito);
							sprintf(cadena, "insert|UPDATE clientes set limite_credito = '%f' where id_cliente = '%d'", limite_credito, cliente_id);
							break;
						
						default:
							fflush(stdin);
					}

					if( !(opcEdit >= 1 && opcEdit <= 5)){
						break;
					}

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

void categorias(int argc, char *argv[])
{
	char nombre[100];
	int id_cat;
	int porcentaje, unidades, tabulador;
	char cadenaCat[200], cadenaTab[10];


	int opcCategorias, respuestaCat;
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
			printf("\n\n\n\n\t ==== CREACION DE CATEGORIA ====\n");

				printf("Nombre Categoria: ");
				scanf(" %[^\n]", nombre);

				printf("Porcentaje: ");
				scanf("%d", &porcentaje);

				printf("Unidades: ");
				scanf("%d", &unidades);

				sprintf(cadenaCat, "insert|SELECT insertar_categoria('%s','%d','%d');", nombre,porcentaje,unidades);

				t_ini = clock();
				respuestaCat = executeServidor(argc, argv, cadenaCat);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);


				if(respuestaCat == 200){
					printf("\n==== Categoria CREADA ====\n\n");
				}else{
					printf("\n==== Categoria NO CREADA :( ====\n\n");
				}

				
			break;

		case 2:
			printf("\n\n\n\n\t ==== CONSULTA DE CATEGORIAS ====\n\n");

				sprintf(cadenaCat, "sel_catego|SELECT * FROM categorias;");

				t_ini = clock();
				executeServidorSelects(argc, argv, cadenaCat);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

			break;

		case 3:
			printf("\n\n\n\n\t ==== MODIFICACION DE CATEGORIAS ====\n");
			printf("Dame el ID de la categoria: ");
				scanf("%d", &id_cat);
				sprintf(cadenaCat, "findById|SELECT * FROM categorias WHERE id_cat = %d;", id_cat);

				t_ini = clock();
				respuestaCat = executeServidor(argc, argv, cadenaCat);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);
				

				if(respuestaCat == 200){

					printf("==== Categoria encontrada ==== \n\n");

					printf("Nombre: ");
					scanf(" %[^\n]", nombre);

					sprintf(cadenaCat, "insert|UPDATE categorias set nombre = '%s' where id_cat = '%d'", nombre, id_cat);

					t_ini = clock();
					respuestaCat = executeServidor(argc, argv, cadenaCat);
					t_fin = clock();
					total= t_fin - t_ini;
					printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

					printf("\n ==== Categoria modificada correctamente ====\n\n");

				}else{
					printf("\n ==== Categoria no encontrada ====\n\n");
				}
			break;

		case 4:
			printf("\n\n\n\n\t ==== ELIMINACION DE CATEGORIA ====\n\n");
			printf("Dame el ID de la Categoria: ");
				scanf("%d", &id_cat);
				sprintf(cadenaCat, "findById|SELECT * FROM categorias WHERE id_cat = %d;", id_cat);

				respuestaCat = executeServidor(argc, argv, cadenaCat);

				if(respuestaCat == 200){

					sprintf(cadenaCat, "insert|DELETE FROM categorias where id_cat = %d", id_cat);

					t_ini = clock();
					respuestaCat = executeServidor(argc, argv, cadenaCat);
					t_fin = clock();
					total= t_fin - t_ini;
					printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

					printf("\n==== Categoria eliminada ==== \n\n");

				}else{
					printf("\n==== Categoria no encontrado :( ====\n\n");
				}
			break;

		case 5:
			break;

		default:
			fflush(stdin);
		}

	} while (opcCategorias != 5);
}

void productos(int argc, char *argv[])
{
	int opcProducto;
	int respuesta;

	char nombre[100], marca[100];
	int id_mat, id_cat;
	int stock, stock_min;
	char cadenaMat[200];
	float precio;
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
			printf("\n\n\n\n\t ==== CREACION DE PRODUCTO =====\n\n");


				respuesta = 0;

				do{

					printf("ID de la categoria: ");
					scanf("%d", &id_cat);

					sprintf(cadenaMat, "findById|SELECT * FROM categorias WHERE id_cat = %d;", id_cat);

					respuesta = executeServidor(argc, argv, cadenaMat);

					if(respuesta != 200){
						printf("==== Categoria no encontrada, intente de nuevo ==== \n");
						printf("Presione 1 para continuar, 2 para cancelar: ");
						scanf("%d", &respuesta);

						if(respuesta == 2 || respuesta != 1){
							break;
						}
					}

				}while (respuesta != 200);

				if(respuesta != 200){
					break;
				}
				
			
				printf("Nombre del Producto: ");
				scanf(" %[^\n]",nombre);

				printf("Marca: ");
				scanf(" %[^\n]", marca);

				printf("Precio: ");
				scanf("%f", &precio);

				printf("Stock inicial: ");
				scanf("%d", &stock);

				printf("Stock minimo: ");
				scanf("%d", &stock_min);


				sprintf(cadenaMat, "insert|INSERT INTO materiales (id_cat, nombre, marca, precio, stock, stock_min) VALUES ('%d', '%s', '%s', '%f', '%d', '%d');",id_cat, nombre, marca, precio, stock, stock_min);

				t_ini = clock();
				respuesta = executeServidor(argc, argv, cadenaMat);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

				if(respuesta == 200){
					printf("\n==== PRODUCTO CREADO ====\n\n");
				}else{
					printf("\n==== PRODUCTO NO CREADO :( ====\n\n");
				}

			break;

		case 2:
				printf("\n\n\n\n\t ==== CONSULTA DE PRODUCTOS ===== \n\n");
				sprintf(cadenaMat, "sel_mate|SELECT * FROM materiales;");

				t_ini = clock();
				executeServidorSelects(argc, argv, cadenaMat);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);
			break;

		case 3:
				printf("\n\n\n\n\t===== MODIFICACION DE PRODUCTO ===== \n\n");
				printf("Dame el ID del Producto: ");
				scanf("%d", &id_mat);
				sprintf(cadenaMat, "findById|SELECT * FROM materiales WHERE id_mat = %d;", id_mat);

				t_ini = clock();
				respuesta = executeServidor(argc, argv, cadenaMat);
				t_fin = clock();
				total= t_fin - t_ini;
				printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);
				

				if(respuesta == 200){

					printf("Producto encontrado\n\n");

					respuesta = 0;

					do{

						
						printf("ID de la categoria: ");
						scanf("%d", &id_cat);

						sprintf(cadenaMat, "findById|SELECT * FROM categorias WHERE id_cat = %d;", id_cat);

						respuesta = executeServidor(argc, argv, cadenaMat);

						if(respuesta != 200){
							printf("==== Categoria no encontrada, intente de nuevo ==== \n");
							printf("Presione 1 para continuar, 2 para cancelar: ");
							scanf("%d", &respuesta);

							if(respuesta == 2 || respuesta != 1){
								break;
							}
						}

					}while (respuesta != 200);

					if(respuesta != 200){
						break;
					}

					printf("Nombre del Producto: ");
					scanf(" %[^\n]", nombre);

					printf("Marca: ");
					scanf(" %[^\n]", marca);

					printf("Precio: ");
					scanf("%f", &precio);

					printf("Stock: ");
					scanf("%d", &stock);

					printf("Stock minimo: ");
					scanf("%d", &stock_min);


					sprintf(cadenaMat, "insert|UPDATE materiales set nombre = '%s', marca = '%s', precio = %f, stock = %d, stock_min = %d, id_cat = %d where id_mat = %d", nombre, marca, precio, stock, stock_min, id_cat, id_mat);

					t_ini = clock();
					respuesta = executeServidor(argc, argv, cadenaMat);
					t_fin = clock();
					total= t_fin - t_ini;
					printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

					printf("\n ==== Producto modificado correctamente ====\n\n");

				}else{
					printf("\n ==== Producto no encontrado ====\n\n");
				}

			break;

		case 4:
			printf("\n\n\n\n\t ==== ELIMINACION DE PRODUCTO ==== \n\n");
			printf("Dame el ID del Producto: ");
				scanf("%d", &id_mat);
				sprintf(cadenaMat, "findById|SELECT * FROM materiales WHERE id_mat = %d;", id_mat);

				respuesta = executeServidor(argc, argv, cadenaMat);

				if(respuesta == 200){

					sprintf(cadenaMat, "insert|DELETE FROM materiales where id_mat = %d", id_mat);

					t_ini = clock();
					respuesta = executeServidor(argc, argv, cadenaMat);
					t_fin = clock();
					total= t_fin - t_ini;
					printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

					printf("\n==== Producto eliminado ==== \n\n");

				}else{
					printf("\n==== Producto no encontrado :( ====\n\n");
				}
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

	int opcReportes, opcF, opcV, opcC, opcP, opcCF, opcCF2, opcC1, opcC2, opcC3, opcMes;
	char cadenaC[500], cadena[500];
	do
	{
		printf("\t==== REPORTES ===== \n\n");
		printf("\t1.-Ventas\n\n");
		printf("\t2.-Clientes\n\n");
		printf("\t3.-Productos\n\n");
		printf("\t4.-Regresar al menu principal\n\n");

		printf("Elige una opcion:");
		scanf("%d", &opcReportes);
		system("clear");

		switch (opcReportes){
		
		case 1:

			do{
				printf("VENTAS\n");
				printf("\n\n\t1.-Consultar ventas (Credito)\n\n");
				printf("\t2.-Consultar ventas (Contado)\n\n");
				printf("\t3.-Ver todas\n\n");
				printf("\t4.- <- Atras\n\n");

				printf("Elige una opcion: ");
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
		case 2:

			do
			{
				printf("\t ==== CLIENTES ==== \n");
				printf("\n\n\t1.- Consultar clientes\n\n");
				printf("\t2.- Descuento de clientes\n\n");
				printf("\t3.- Creditos de clientes\n\n");
				printf("\t4.- <- Atras\n\n");

				printf("Elige una opcion:");
				scanf("%d", &opcC);
				system("clear");

				switch (opcC)
				{
				case 1:
					do
					{
						printf("\t==== CONSULTAR CLIENTES\n\n");
						printf("\n\n\t1.- Clientes con descuentos\n\n");
						printf("\t2.- Ver todos\n\n");
						printf("\t3.- <- Atras\n\n");

						printf("Elige una opcion:");
						scanf("%d", &opcC1);
						system("clear");

						switch (opcC1)
						{
						case 1:
							printf("\n\n\n\n\t==== CLIENTES CON DESCUENTO ==== \n");

							selectMesOTodosClientes(argc,argv,2,opcMes);

							break;

						case 2:
							printf("\n\n\n\n\t ===== VER TODOS =====\n");

							selectMesOTodosClientes(argc,argv,5,opcMes);

							break;

						default:
							fflush(stdin);
						}
					} while (opcC1 != 3);
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
							printf("\n\n\n\n\t ===== POR MES ==== \n");
								do
								{
									printf("\n\t==== MESES ====");
									printf("\n\n\t1.-Enero\n");
									printf("\t2.-Febrero\n");
									printf("\t3.-Marzo\n");
									printf("\t4.-Abril\n");
									printf("\t5.-Mayo\n");
									printf("\t6.-Junio\n");
									printf("\t7.-Julio\n");
									printf("\t8.-Agosto\n");
									printf("\t9.-Septiembre\n");
									printf("\t10.-Octubre\n");
									printf("\t11.-Noviembre\n");
									printf("\t12.-Diciembre\n");
									printf("\t13.-Salir\n\n");

									printf("Elige una opcion: ");
									scanf("%d", &opcMes);
									system("clear");

									switch (opcMes)
									{
									case 1:
										printf("\n\n\n\n\tEnero\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;

									case 2:
										printf("\n\n\n\n\tFebrero\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;
									
									case 3:
										printf("\n\n\n\n\tMarzo\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;
									case 4:
										printf("\n\n\n\n\tAbril\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;
									
									case 5:
										printf("\n\n\n\n\tMayo\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;
									
									case 6:
										printf("\n\n\n\n\tJunio\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;
									
									case 7:
										printf("\n\n\n\n\tJulio\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;
									
									case 8:
										printf("\n\n\n\n\tAgosto\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;

									case 9:
										printf("\n\n\n\n\tSeptiembre\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;
									
									case 10:
										printf("\n\n\n\n\tOctubre\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;
									case 11:
										printf("\n\n\n\n\tNoviembre\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);

										break;
									
									case 12:
										printf("\n\n\n\n\tDiciembre\n\n");

										selectMesOTodosClientes(argc,argv,1,opcMes);										

										break;

									default:
										fflush(stdin);
									}
								} while (opcMes != 13);
							

							break;

						case 2:
							printf("\n\n\n\n\tVer todos\n\n");

							selectMesOTodosClientes(argc,argv,2,opcMes);

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
							printf("\n\n\n\n\t===== POR MES ==== \n");
								do
								{
									printf("\t\n ====== MESES =====");
									printf("\n\n\t1.-Enero\n");
									printf("\t2.-Febrero\n");
									printf("\t3.-Marzo\n");
									printf("\t4.-Abril\n");
									printf("\t5.-Mayo\n");
									printf("\t6.-Junio\n");
									printf("\t7.-Julio\n");
									printf("\t8.-Agosto\n");
									printf("\t9.-Septiembre\n");
									printf("\t10.-Octubre\n");
									printf("\t11.-Noviembre\n");
									printf("\t12.-Diciembre\n");
									printf("\t13.- <- Atras\n\n");

									printf("Elige una opcion: ");
									scanf("%d", &opcMes);
									system("clear");

									switch (opcMes)
									{
									case 1:
										printf("\n\n\n\n\tEnero\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;

									case 2:
										printf("\n\n\n\n\tFebrero\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;
									
									case 3:
										printf("\n\n\n\n\tMarzo\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;
									case 4:
										printf("\n\n\n\n\tAbril\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;
									
									case 5:
										printf("\n\n\n\n\tMayo\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;
									
									case 6:
										printf("\n\n\n\n\tJunio\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;
									
									case 7:
										printf("\n\n\n\n\tJulio\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;
									
									case 8:
										printf("\n\n\n\n\tAgosto\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;

									case 9:
										printf("\n\n\n\n\tSeptiembre\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;
									
									case 10:
										printf("\n\n\n\n\tOctubre\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;
									case 11:
										printf("\n\n\n\n\tNoviembre\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);

										break;
									
									case 12:
										printf("\n\n\n\n\tDiciembre\n\n");

										selectMesOTodosClientes(argc,argv,3,opcMes);										

										break;

									default:
										fflush(stdin);
									}
								} while (opcMes != 13);
							

							break;

						case 2:
							printf("\n\n\n\n\tVer todos\n\n");

							selectMesOTodosClientes(argc,argv,4,opcMes);

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
		case 3:

			do{
				printf("\n\t==== REPORTE PRODUCTOS ====\n");
				printf("\n\n\t1.- Ver productos mas vendidos\n\n");
				printf("\t2.- Ver productos menos vendidos\n\n");
				printf("\t3.- Ver productos en descuentos\n\n");
				printf("\t4.- Ver todos\n\n");
				printf("\t5.- <- Atras\n\n");

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
						printf("\n\n\n\n\t ==== VER PRODUCTOS CON DESCUENTO ====\n");

						sprintf(cadena, "prod_descuento|select m.id_mat, m.nombre as material, c.id_cat, c.nombre as categoria, t.porcentaje_max, t.unidades from materiales m inner join categorias c on c.id_cat = m.id_cat inner join tabulador t  on t.id_cat = c.id_cat;");

						t_ini = clock();
						executeServidorSelects(argc, argv, cadena);
						t_fin = clock();
						total= t_fin - t_ini;
						printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

						break;

					case 4:
						printf("\n\n\n\n\t==== VER TODOS LOS PRODUCTOS ==== \n");

						sprintf(cadena, "prod_all|select m.id_mat, m.nombre as material, m.marca, m.precio, m.stock, m.stock_min ,c.id_cat, c.nombre as categoria, t.porcentaje_max, t.unidades from materiales m inner join categorias c on c.id_cat = m.id_cat inner join tabulador t  on t.id_cat = c.id_cat;");

						t_ini = clock();
						executeServidorSelects(argc, argv, cadena);
						t_fin = clock();
						total= t_fin - t_ini;
						printf("\n==== Tiempo de ejecucion: %lf ====\n\n", total/ CLOCKS_PER_SEC);

						break;
					case 5:
						break;

					default:
						fflush(stdin);
				}

			} while (opcP != 5);

			system("clear");
			break;

		case 4:
			break;

		default:
			fflush(stdin);

		} //FIN SWITCH

	} while (opcReportes != 4);
}
