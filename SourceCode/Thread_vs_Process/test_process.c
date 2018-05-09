#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/times.h>
#include <limits.h>

#define FILE_NAME_MAX 1024

void     test(void*);

int main() {
    clock_t         startTime, finishTime;
    struct tms      mytms;
    char*           startDir;
    
    // 시작 시간 측정
    if((startTime = times(&mytms)) == -1) {
        perror("startTime");
        exit(1);
    }
    
    // 학교 서버내 탐색 시작할 주소
    startDir = "/u4/cestu";
    
    // 파일 탐색 시작
    test(startDir);
    
    
    // 종료 시간 측정
    if ((finishTime = times(&mytms)) == -1) {
        perror("finishTime");
        exit(1);
    }
    
    
    // 생성한 공유 메모리 삭제
    if (shmctl(shmget((key_t)2222, sizeof(char*), 0), IPC_RMID, NULL)) {
        perror("shmctl failed : ");
        exit(1);
    }

	printf("\n");
	printf("*************************************************\n");
	printf("*               PROCESS COMPLETE                *\n");
	printf("*            Running Time : %1.f sec            *\n",(double)(finishTime - startTime) / CLK_TCK);
	printf("*************************************************\n");
    
    return 0;
}

void test(void* fdName) {
    DIR             *pDir = NULL;
    pid_t           pid;
    struct  dirent  *pFile = NULL;
    struct  stat    buf;
    char*           filePath;
    char*           nextDirPath;
    char*           nextDirList[1024];
    void*           shared_name = (void *)0;
    int             countDir = 0, i;
    int             shmid_name;
    
    filePath = (char*)malloc(sizeof(char)*FILE_NAME_MAX);
    
    // 디렉토리 'startDir'의 정보를 pDir에 저장
    pDir = opendir((char*)fdName);
    
    // 디렉토리 열기 실패
    if(!pDir) {
        printf("ERROR\n");
    }
    
    // 디렉토리 내 폴더 및 파일을 전체 탐색
    while( (pFile = readdir(pDir)) != NULL ) {
        
        // 상위 폴더 "..", 현재 폴더 "." , 실행중 오류가 났던 02~09backup.tar 폴더 탐색하지 않음
        if(strcmp(pFile->d_name, ".") == 0 || strcmp(pFile->d_name,".." ) == 0
           || strcmp(pFile->d_name, "02~09backup.tar") == 0) continue;
        
        // 현재 위치한 폴더의 경로에 현재 읽고 있는 폴더 및 파일 이름을 붙여 읽고 있는 폴더 및 파일의 절대 경로 생성
        strcpy(filePath, (char*)fdName);
        strcat(filePath,"/");
        strcat(filePath,pFile->d_name);
        
        // buf 초기화
        memset(&buf, 0, sizeof(struct stat));
        
        // 파일의 전체 경로를 통해 파일 정보를 buf 에 저장
        stat(filePath, &buf);
        
        // 권한이 없는 폴더 및 파일의 경우 PASS
        if((buf.st_mode & S_IROTH) == 0 ) continue;
        if((buf.st_mode & S_IRGRP) == 0 ) continue;
        
        // 실행 중 오류가 나는 폴더 및 부분 PASS
        if(strstr(pFile->d_name, ".png") != NULL) continue;
        if(strcmp(filePath, "/u4/cestu/02~09backup/04/yaho789/test") == 0) continue;
        
        if(S_ISDIR(buf.st_mode)) {
            
            // pFile->d_name == 현재 읽고 있는 폴더 및 파일의 이름 , buf.st_size == 현재 읽고 있는 폴더 및 파일의 크기
            printf("[DIR]%s (SIZE)%lld bytes\n", pFile->d_name, (long long)buf.st_size);
            
            // 현재 디렉토리 내 존재하는 디렉토리들의 경로를 nextDirList []  에 저장
            nextDirList[countDir] = (char*)malloc(sizeof(char)*256);
            strcpy(nextDirList[countDir], filePath);
            
            // 현재 디렉토리 내 존재하는 디렉토리 개수
            countDir += 1;
        }
        // 파일일 경우
        else if(S_ISREG(buf.st_mode)) {
            printf("[FILE]%s (SIZE)%lld bytes\n", pFile->d_name, (long long)buf.st_size);
        }
        // 그외의 경우
        else    {
            printf("[ELSE]%s (SIZE)%lld bytes\n", pFile->d_name, (long long)buf.st_size);
        }
    }
    
    // 부모 프로세스, 공유 메모리 생성
    shmid_name = shmget((key_t)2222, sizeof(char*), IPC_CREAT | 0666);
    
    if (shmid_name == -1)
    {
        perror("shmget failed : ");
        exit(1);
    }
    
    // 공유 메모리를 프로세스에 첨부
    shared_name = shmat(shmid_name, (void *)0, 0);
    
    if (shared_name == (void*)-1)
    {
        perror("shmat failed : ");
        exit(1);
    }
    
    // 공유 메모리 주소를 nextDirPath 에 저장
    nextDirPath = (char*)shared_name;
    
    
    /* nextDirList 에 저장된 디렉토리 개수만큼              */
    /* 자식 프로세스 생성 (공유 메모리를 통해 디렉토리 경로를 전달) */
    for(i=0; i<countDir; i++){
        strcpy(nextDirPath,nextDirList[i]);
        
        pid = fork();
        
        if (pid == -1) {
            perror("fork : ");
            exit(1);
        }
        
        // 자식 프로세스 수행
        if (pid == 0) {
            shmid_name = shmget((key_t)2222, sizeof(char*), 0);
            
            if (shmid_name == -1)
            {
                perror("shmget failed : ");
                exit(1);
            }
            
            shared_name = shmat(shmid_name, (void *)0, SHM_RDONLY);
            
            if (shared_name == (void*)-1)
            {
                perror("shmat failed : ");
                exit(1);
            }
            
            // 공유 메모리에 저장된 디렉토리 경로를 nextDirPath 에 저장
            strcpy(nextDirPath, (char*)shared_name);
            
            if (shmdt(shared_name) == -1) {
                perror("shmdt failed : ");
                exit(1);
            }
            
            // 자식 프로세스의 재귀적 탐색
            test(nextDirPath);
            
            exit(1);
        }
        
        // 부모 프로세스 수행
        else {
            // 모든 자식 프로세스의 종료를 대기
            while ( wait(NULL)>0 ) {}
        }
    }
    
    // 프로세스의 첨부한 공유 메모리를 제거
    if (shmdt((void*)shared_name) == -1) {
        perror("shmdt : ");
        exit(1);
    }

    closedir(pDir);
    free(filePath);

	return;
}
