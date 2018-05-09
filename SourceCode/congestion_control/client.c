#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#define BUFSIZE 1024
#define PORTNUM 9026

int congwin =1;
int threshold = 1024;
int flag = 0;

void error_handling(char *message);

// Timeout handler �Լ�
void handler (int signo){
	printf("\n\033[91m\t\t------------------------\033[0m\n");
	printf("\033[91m\t\t|\tTime Out!!\t|\033[0m\n");
	printf("\033[91m\t\t-------------------------\033[0m\n");
	printf(" \033[93m-------------------------------------------\033[0m\n");
	printf(" \033[93m| WindowSize | Threshold | SendTimes | ACK |\033[0m\n");
	printf(" \033[93m-------------------------------------------\033[0m\n");
	if(flag == 0) // Conwin�� ���������� ������ �� Threshold �缳��
		threshold = congwin /4;
	else //Congwin�� ���������� ������ �� Threshold �缳��
		threshold = (congwin-1)/2;
	congwin =1; // TimeOut �߻��� Congwin 1�� �缳��
}

// 3���� �ߺ� ACK �߻��� handler �Լ�
void dupack(){
	printf("\033[91m\t\t-------------------------\033[0m\n");
	printf("\033[91m\t\t|\tOccur 3ACK!!\t|\033[0m\n");
	printf("\033[91m\t\t-------------------------\033[0m\n");
	printf(" \033[93m-------------------------------------------\033[0m\n");
	printf(" \033[93m| WindowSize | Threshold | SendTimes | ACK |\033[0m\n");
	printf(" \033[93m-------------------------------------------\033[0m\n");
	congwin = congwin/2; // conwin 1/2 
	threshold = congwin;
}

int main(int argc, char **argv)
{
	int sock;
	struct sockaddr_in serv_addr;
	int recvmsg;
	int recvarr[1024];
	int recv_index =0;
	int i;

	sigset(SIGALRM, handler);

	sock = socket(PF_INET, SOCK_STREAM, 0); 
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("210.93.57.72");
	serv_addr.sin_port = htons(PORTNUM);

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	printf(" \033[93m-------------------------------------------\033[0m\n");
	printf(" \033[93m| WindowSize | Threshold | SendTimes | ACK |\033[0m\n");
	printf(" \033[93m-------------------------------------------\033[0m\n");

	// Server���� ��Ŷ �۽�
	while (1) {
		printf("\t%d\t  %d", congwin, threshold); 
		if(congwin<threshold){ // congwin�� threshold���� ���� �� conwin ���� ����
			for(i=0; i<congwin; i++){
				write(sock, &congwin, sizeof(int)); // ��Ŷ ���� ��Ŷ�� ���ǻ� conwin������ ����
			}
			printf("\t\t%d",i);
			congwin *= 2;
			flag = 0;
		}
		else{ //congwin�� threshold ���� ũ�� ��������
			for(i=1; i<=congwin; i++){
				write(sock, &congwin, sizeof(int));
			}
			printf("\t\t%d",i-1);
			congwin++;
			flag = 1;
		}
		
		sleep(1);

		alarm(5); // �� congwin��ŭ ��Ŷ ������ Ÿ�̸� �缳��

		read(sock, &recvmsg, sizeof(int)); // ACK Recv
		printf("\t %d \n",recvmsg);
		recvarr[recv_index++] = recvmsg;
		
		if(recv_index>2){ // 3 �ߺ� ACK �Ǻ�
			if((recvarr[recv_index-3] == recvarr[recv_index-2]) && (recvarr[recv_index-1] == recvarr[recv_index-2])){
				dupack();
			}
		}
		
	}

	close(sock);

	return 0;
}
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
