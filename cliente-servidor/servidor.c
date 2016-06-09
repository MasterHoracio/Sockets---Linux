#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <math.h>
#define MAX 5
#define WLK "Bienvenido ATT: MASTER HORACIO"
#define TAM 2

float Mat[TAM];
float sumatoria;

void split(char *buff){
	char *number;
	float num;
	sumatoria = 0;
	for(number = strtok(buff," "); number != NULL; number = strtok(NULL," ")){
		num = atof(number);
		sumatoria += num;
	}
}


int main(int argc, char *argv[]){
	int sock;
	struct sockaddr_in servidor, cliente;
	int mysock;
	//char res[100];
	char buff[1024];
	int value;

	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0){
		perror("Fallo al crear socket");
		exit(1);
	}

	servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = inet_addr("192.168.140.129");
	servidor.sin_port = htons(5000);

	if( bind(sock, (struct sockaddr *) &servidor, sizeof(servidor)) < 0){
		perror("Fallo al crear bind");
		exit(1);
	}

	listen(sock, MAX);
	int c = sizeof(struct sockaddr_in);
	
	printf("||||||||Esperando Peticion del cliente||||||||\n");
	mysock = accept(sock, (struct sockaddr *) &cliente, (socklen_t*)&c);
	if(mysock == -1){
		perror("Fallo al aceptar");
		exit(1);
	}else{
		printf("|*|*|*|*|*|*|Un cliente se ha conectado|*|*|*|*|*|*|\n\n");
	}
	
	memset(buff,0,sizeof(buff));
	
	while( ( value = recv(mysock,buff,sizeof(buff),0) ) > 0){
		if(value < 0)
			perror("Error al leer el stream");
		else if(value == 0)
			printf("Aun en conexion\n");
		else{//si se recibe la informacion con exito
			printf("Cadena Recibida con exito\n");
			printf("Cadena: %s",buff);
			printf("Procesando las operaciones...\n");
			//mandar a la funcion de purificacion
			split(buff);
			char res[100];
			sprintf(res,"%.3f",sumatoria);
			//enviar respuesta
			if(write(mysock,res,sizeof(res)) < 0){
				perror("Error al contestar");
				exit(1);
			}else{
				printf("El resultado de la operacion es: %s\n\n",res);
			}
			memset(buff,0,sizeof(buff));
		}

	}
	printf("\n\n******Se perdio la conexion con el cliente******\n");
	close(mysock);

	return 0;
}
