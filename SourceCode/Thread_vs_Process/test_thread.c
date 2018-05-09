  #include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

#include <sys/times.h>
#include <limits.h>

#define NAME_MAX 1024

typedef struct {
    long            d_ino;                  // inode 넘버
   off_t           d_off;                  // offset
    unsigned short  d_reclen;               // d_name 길이
    char            d_name[NAME_MAX+1];     // 파일이름
}dirent;

void* test(void*);
void* count(long);
unsigned long totalFileSize = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(void) {
    char* startDir;
    struct tms mytms;
    clock_t startTime, finishTime;
    
    // 시작 시간 측정
    if((startTime  = times(&mytms)) == -1) {
        perror("startTime ");
        exit(1);
    }
    
    // 학교 유닉스 서버내 컴공학생 폴더의 상위 폴더
    startDir = "/u4/cestu";
    printf("Start Running.\n");
    test((void*)startDir);

    // 종료 시간 측정
    if((finishTime  = times(&mytms)) == -1) {
        perror("finishTime ");
        exit(1);
    }
    
    // 파일의 총 크키와 실행 시간 및 User, System 시간 출력

	printf("\n");
	printf("*************************************************\n");
	printf("*               THREAD COMPLETE                 *\n");
	printf("*            Running Time : %.1f sec            *\n", (double)(finishTime - startTime) / CLK_TCK);
	printf("*************************************************\n");
    
    return 0;
}
void* test(void* fdName) {
    DIR *pDir = NULL;
    pthread_t pthread;
    struct dirent *pFile = NULL;
    struct stat buf;
    char filePath[1024];
    int thread_id;
    
    // 디렉토리 'fdName'의 정보를 pDir에 저장
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
        
        // 현재 읽은 파일이 폴더일 경우 Thread를 생성해 재귀적 함수를 호출하여 폴더를 탐색
        if(S_ISDIR(buf.st_mode)) {
            
            // pFile->d_name == 현재 읽고 있는 폴더 및 파일의 이름 , buf.st_size == 현재 읽고 있는 폴더 및 파일의 크기
            printf("[DIR]%s (SIZE)%lld bytes\n", pFile->d_name, (long long)buf.st_size);
            
            // 현재 읽은 폴더의 경로를 시작으로 새 Thread 생성 및 작업 수행, thread_id 에 생성한 Thread 의 ID 반환
            thread_id = pthread_create(&pthread, NULL, test, (void*)filePath);

            // 생성된 Thread 종료 까지 그 Thread 를 생성한 Thread 는 대기
            pthread_join(pthread, NULL);
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
    closedir(pDir);
    return NULL;
}