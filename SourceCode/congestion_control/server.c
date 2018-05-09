#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
 
#define BUFSIZE 1024
#define PORTNUM 9026

void error_handling(char *message);
 
int main()
{
	int serv_sock;
    int clnt_sock;
    char message[BUFSIZE];
	int index[BUFSIZE];
    int str_len;
	int val, i;
	int flag = 0;
	int flag2 = 0;
	int var;
	int dex;
	int count=0; 
	int temp=0;
	int temp2;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    int clnt_addr_size;

 
 
    serv_sock = socket(PF_INET, SOCK_STREAM, 0); 
    if(serv_sock == -1)
        error_handling("socket() error");
	
	if(setsockopt (serv_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&val, sizeof(val))<0){
		perror("setsockopt");
		close(serv_sock);
		return -1;
	}

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PORTNUM);
 
	if( bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) )==-1)
		error_handling("bind() error");
 
    if(listen(serv_sock, 5) == -1) 
        error_handling("listen() error");
 
    clnt_addr_size = sizeof(clnt_addr);
          
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
		
    if(clnt_sock == -1)
        error_handling("accept() error");
	
	// 패킷 Recv
	while((str_len = read(clnt_sock, &var, sizeof(int))) !=0){
		printf("Recv MSG : %d\n", var);

		if(var!=temp){ //클라이언트 측 conwin값이 바뀜을 확인

			// congwin 값이 32의 배수일 때마다 timeout과 3ACK 이벤트 발생 유발
			if(var%32==0 && flag==0){ // Timeout 유발
				sleep(3);
				temp = var;
				flag = 1;
				continue;
			}
			 if(var%32==0 && flag==1 && flag2==0){ //3ACK 유발
				dex = var;
				flag2 = 1;
			}
			if(flag2 ==1){
				write(clnt_sock, &dex, sizeof(int));
				if(++count==3){
					flag = 0;
					flag2 = 0;
					count = 0;
				}
			}
			else
				write(clnt_sock, &var, sizeof(int)); // ACK 송신
		}
		temp=var;
	}		

    close(clnt_sock);  
 
    return 0;
}
 
void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}
