#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>

#define IP_SERV "192.168.140.129"
#define DATA "Creado por Master Horacio"

int split(char *buff){
	char *number;
	int tam,punto,menos,mas;
	for(number = strtok(buff," "); number != NULL; number = strtok(NULL," ")){
		tam = strlen(number);
		punto = 0;
		menos = 0;
		mas = 0;
		//para cada numero se comprueba
		for(int i = 0; i < tam; i++){
			if( isdigit(number[i]) ){//si es un numero
				continue;
			}else{//si no es numero
				
				if(number[i] == '+')
					mas++;
				else if(number[i] == '-')
					menos++;
				else if(number[i] == '.')
					punto++;
				else if(number[i] == '\n')
					return 1;
				else{
					printf("\n********Error en el formato de Numero********\n\n");
					return 0;
				}
					
				if( (mas > 1) || (menos > 1) || (punto > 1) ){
					printf("\n********Error en el formato de Numero********\n\n");
					return 0;
				}
				if( (mas >= 1) && (menos >= 1) ){
					printf("\n********Error en el formato de Numero********\n\n");
					return 0;
				}
			}
		}
	}
	return 1;
}

int main(int argc, char *argv[]){
	int sock,snd,rcv;
	struct sockaddr_in servidor;
	//struct hostent *hp;
	char buff[1024];
	char input[1024];
	char Input[1024];

	sock = socket(AF_INET,SOCK_STREAM,0);
	if(sock < 0){
		perror("Error al crear socket");
		exit(1);
	}
	//dar de alta el servidor
	servidor.sin_family = AF_INET;
	servidor.sin_addr.s_addr = inet_addr(IP_SERV);
	servidor.sin_port = htons(5000);

	printf("Conectando con el servidor %s...\n",IP_SERV);
	if(connect(sock, (struct sockaddr *) &servidor, sizeof(servidor)) <0){
		perror("Error al conectar al servidor");
		exit(1);
	}
	printf("Conexion exitosa \n\n");

	memset(input,0,1024);

	while(1){
		//lectura y verficacion de cadena
		do{
			printf("Ingresa numeros (num num ...): ");
			fgets(input,1024,stdin);
			strcpy(Input,input);
		}while(!split(input));

		if(send(sock, Input, sizeof(Input), 0) < 0){
			perror("Error al enviar datos");
			exit(1);
		}

		//recibir respuesta
		memset(buff,0,1024);
		if( (rcv = recv(sock,buff,sizeof(buff),0)) < 0){
			perror("Error al recibir respuesta");
			exit(1);
		}if(rcv == 0){
			printf("Error al enviar la cadena: %s",Input);
			printf("\n\n*********Se perdio la conexion con el servidor*********\n");
			break;
		}else{
			printf("Cadena enviada: %s\n",Input);
			printf("Respuesta: %s\n\n",buff);
		}

	}
	close(sock);

	return 0;
}
