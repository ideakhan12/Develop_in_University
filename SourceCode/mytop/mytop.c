#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/times.h>
#include <limits.h>
#include <procfs.h>
#include <pwd.h>
#include <kstat.h>
#include <time.h>

//���μ��� ����ü pInfo ���ȭ
#define Data pInfo

//username �迭 ������
#define strLen 15

//���� ó���� ���� �ܺ� ����
extern int errno;

//���μ��� ������ ��� �ִ� ����ü
typedef struct pInfo
{
	int pid;
	char username[255];
	double per_cpu;
	double per_mem;
	int time;
	int state;
	char command[255];
	struct pInfo* nextNode;
}pInfo;

//pInfo�� ���� ���� ����ü
typedef struct node {
	Data data;
	struct node* next;
}Node;

//���μ��� ����Ʈ�� ���� ����ü
typedef struct list {
	Node* header;
	int length;
	void(*Add)(struct list*, Data);
	int(*Remove)(struct list*, Data);
	Data(*Search)(struct list*, unsigned int);
	void(*Print)(struct list*);
	void(*Sort)(struct list*);
}List;

List newList(); //���μ��� ����Ʈ �ʱ�ȭ

void Add(List*, Data); // ����ü�� ���μ��� ����Ʈ�� �߰�

int Remove(List*, Data); // �ش� ����ü�� ���μ��� ����Ʈ���� ����

Data SearchByPID(List*, unsigned int); // head ���� �ش��ϴ� pid�� ���� ����ü Ž��

void Print(List*); // ���μ��� ����Ʈ ���

void Data_Print(Data data); //���μ��� ����ü�� ���� ������ ���

void Data_Sort_By_PerCpu(List *this); // CPU �������� ���� ��Ʈ

int comp(const void * elem1, const void * elem2); // CPU �� ��

int getNumUsr(); // ������ ���

void getMemory(int *total, int *free); //�޸� �� ���

void help(); // ���� ���

void sortedInsert(pInfo** head_ref, pInfo* new_node); // ���������ϸ鼭 ��� �߰�

void insertionSort(pInfo **head_ref); // �ش� ����Ʈ�� ���� ����

int getProcessList(struct pInfo **head); // ���μ����� ���ؿ���, �ش� ���μ����� ����Ʈ�� �߰�

void viewProcess(pInfo **head, int line); // ���μ����� ���� ���� ���

void freeList(pInfo **head); // ���μ��� ����Ʈ ����


int main(int argc, char** argv)
{
	pInfo *head;
	int n, line = 20, tim = 7;
	extern char *optarg;
	extern int optind;

	head = (pInfo *)calloc(1, sizeof(pInfo));

	while ((n = getopt(argc, argv, "l:t:h")) != -1) // �ɼ� ���� l t h
	{
		switch (n)
		{
		case 'l': // ��� ���μ� ����
			line = atoi(optarg);
			break;

		case 't': // ��� �ֱ� ����
			tim = atoi(optarg);
			break;

		case 'h': // ���� ���
			help();
			break;
		}
	}

	while (1) { //���α׷� ���� �� ���� ����
		system("clear"); //ȭ�� ����

		getProcessList(&head); //���μ����� ���ϰ�, �� ���μ����� ����Ʈ�� ����

		viewProcess(&head, line); //����Ʈ�� ����Ǿ� �ִ� ���μ��� ���� ���
		
		sleep(tim); // �����ֱ⸸ŭ sleep
		freeList(&head); //����Ʈ�� ��带 �޸� �Ҵ� ����
	}

	return 0;
}

void sortedInsert(pInfo** head_ref, pInfo* new_node)
{
	pInfo* current;

	if (*head_ref == NULL || (*head_ref)->per_cpu <= new_node->per_cpu)
	{
		new_node->nextNode = *head_ref;
		*head_ref = new_node;
	}
	else
	{
		current = *head_ref;
		while (current->nextNode != NULL &&
			current->nextNode->per_cpu > new_node->per_cpu)
		{
			current = current->nextNode;
		}
		new_node->nextNode = current->nextNode;
		current->nextNode = new_node;
	}
}



void insertionSort(pInfo **head_ref)
{
	pInfo *sorted = NULL;

	pInfo *current = *head_ref;

	while (current != NULL)
	{
		pInfo *next = current->nextNode;

		sortedInsert(&sorted, current);

		current = next;
	}

	*head_ref = sorted;
}



void help() {
	printf("help\n");

	printf("option -ln : printf n lines\n");

	printf("option -tn : renew printf every nsec\n");

	printf("option -h : help\n");


	return;
}



int getNumUsr()
{
	int fd, count = 0, flag = 0, i;
	struct passwd *pw;
	DIR *dirp;
	char pathName[256];
	struct dirent *direntp;
	struct psinfo pinfo;
	char username[100][100];

	if ((dirp = opendir("/proc")) == NULL)
	{
		perror("dir open Error (proc) ");
		exit(1);
	}
	while ((direntp = readdir(dirp)) != NULL)
	{
		if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, ".."))
		{
			continue;
		}

		memset(pathName, 0x00, sizeof(pathName));
		sprintf(pathName, "%s/%s/psinfo", "/proc", direntp->d_name);

		if ((fd = open(pathName, O_RDONLY)) <0)
			continue;

		read(fd, (void *)&pinfo, sizeof(pinfo));

		if (strcmp(pinfo.pr_fname, "") != 0)
		{
			pw = getpwuid(pinfo.pr_uid);

			strcpy(username[count++], pw->pw_name);

			if (count > 1) {
				for (i = 0; i<count - 1; i++) {
					if (strcmp(username[i], username[count - 1]) == 0)
						flag = 1;
				}
				if (flag == 1) {
					count--;
					flag = 0;
				}
			}
		}

		close(fd);
	}

	closedir(dirp);

	return count;

}



void getMemory(int *total, int *free) {
	int totalMemory;

	int freeMemory;

	kstat_ctl_t *kc = NULL;

	static kstat_t *ks = NULL;

	int pageSize = sysconf(_SC_PAGE_SIZE);

	kc = kstat_open();

	if (!kc) {
		printf("fail to kstat_open : %s\n", strerror(errno));
		exit(1);
	}

	totalMemory = sysconf(_SC_PHYS_PAGES);

	kstat_t *lookup = kstat_lookup(kc, "unix", 0, "system_pages");

	if (!lookup) {
		printf("kstat_read error : %s\n", strerror(errno));
		exit(1);
	}

	if (kstat_read(kc, lookup, 0) == -1) {
		perror("kstat_read error\n");
		exit(1);
	}

	kstat_named_t *data_lookup = (kstat_named_t *)kstat_data_lookup(lookup, "freenmae");

	if (data_lookup) {
		freeMemory = data_lookup->value.ul;
	}

	*free = (int)(freeMemory *(float)(((float)pageSize / 1024.0) / 1024.0));

	*total = (int)(totalMemory * (float)(((float)pageSize / 1024.0) / 1024.0));

	kstat_close(kc);

	return;
}



int getProcessList(struct pInfo **head)
{
	int fd;
	struct passwd *pw;
	DIR *dirp;
	char pathName[256];
	struct dirent *direntp;
	struct psinfo pinfo;
	pInfo *info;


	if ((dirp = opendir("/proc")) == NULL)
	{
		perror("dir open Error (proc) ");
		exit(1);
	}

	while ((direntp = readdir(dirp)) != NULL)
	{

		if (!strcmp(direntp->d_name, ".") || !strcmp(direntp->d_name, ".."))
		{
			continue;
		}

		sprintf(pathName, "%s/%s/psinfo", "/proc", direntp->d_name);


		if ((fd = open(pathName, O_RDONLY)) <0)
		{
			continue;
		}


		read(fd, (void *)&pinfo, sizeof(pinfo));


		if (strcmp(pinfo.pr_fname, "") != 0)
		{
			pw = getpwuid(pinfo.pr_uid);

			info = (pInfo *)calloc(1, sizeof(pInfo));

			info->pid = pinfo.pr_pid;

			strcpy(info->username, pw->pw_name);

			info->per_cpu = pinfo.pr_pctcpu / (double)0x8000 * 100;

			info->per_mem = pinfo.pr_pctmem / (double)0x8000 * 100;

			strcpy(info->command, pinfo.pr_fname);

			info->state = pinfo.pr_wstat;

			sortedInsert(head, info);
		}

		close(fd);
	}

	closedir(dirp);

	return 1;
}

void viewProcess(pInfo **head, int line)
{
	int i;
	int freemem, totalmem;
	pInfo *cur = *head;
	time_t timer;
	struct tm *t;
	int totalMemory, freeMemory;

	timer = time(NULL);

	t = localtime(&timer);

	getMemory(&totalMemory, &freeMemory);

	printf(" %d users\t Total Memory : %d\t Free Memory : %d\t %d:%d:%d\n",
		getNumUsr(), totalMemory, freeMemory, t->tm_hour, t->tm_min, t->tm_sec);

	if (head == NULL)
	{
		perror("head not exsist");
		exit(1);
	}

	printf("%6s %8s %5s %7s %6s %6s %s\n", "PID", "USERNAME", "STATE", "TIME", "CPU", "MEMORY", "COMMAND");

	cur = cur->nextNode;

	for (i = 0; i < line; i++)
	{
		if (i == 0)
			printf("\033[91m%6d %8s %5d %7s %5.2lf%c %5.2lf%c %s\033[0m\n",
				cur->pid, cur->username, cur->state, "time", cur->per_cpu, '%', cur->per_mem, '%', cur->command);
		else if (i == 1 || i == 2 || i == 3)
			printf("\033[93m%6d %8s %5d %7s %5.2lf%c %5.2lf%c %s\033[0m\n",
				cur->pid, cur->username, cur->state, "time", cur->per_cpu, '%', cur->per_mem, '%', cur->command);
		else
			printf("%6d %8s %5d %7s %5.2lf%c %5.2lf%c %s\n",
				cur->pid, cur->username, cur->state, "time", cur->per_cpu, '%', cur->per_mem, '%', cur->command);

		cur = cur->nextNode;
	}
}

List newList() {
	List list;
	memset(&list, 0, sizeof(list));
	list.Add = Add;
	list.Remove = Remove;
	list.Search = SearchByPID;
	list.Print = Print;
	list.Sort = Data_Sort_By_PerCpu;
	list.length = 0;
	list.header = NULL;
	return list;
}

Node* newNode() {
	Node* node = (Node*)malloc(sizeof(Node));
	memset(node, 0, sizeof(node));
	node->next = NULL;

	return node;
}

void Add(List* this, Data data) {
	Node* currentNode = this->header;

	if (this->header == NULL) {
		this->header = newNode();
		this->header->data = data;
		this->length++;
		return;
	}

	while (currentNode->next != NULL) {
		currentNode = currentNode->next;
	}

	currentNode->next = newNode();
	currentNode->next->data = data;
	this->length++;
}

int Remove(List* this, Data data) {
	Node* currentNode = this->header;
	Node* temp = currentNode;
	int i = 0;

	if (currentNode == NULL) {
		printf("There's no exist data\n");
		return 0;
	}
	else if (currentNode->data.pid == data.pid) 
	{
		temp = this->header;
		this->header = this->header->next;
		free(temp);
		this->length--;
		return 1;
	}
	else {
		for (i = 0; i < this->length - 1; i++) {
			if (currentNode->next->data.pid == data.pid) {

				temp = currentNode->next;
				currentNode->next = currentNode->next->next;
				free(temp);
				this->length--;
				return 1;
			}
		}

		return 0;
	}
}


Data SearchByPID(List* this, unsigned int pid) {
	Node * node = this->header;
	Data data;
	memset(&data, 0, sizeof(data));
	int i = 0;
	if (this->length <= 0) {
		printf("non exist matched data\n");
		return data;
	}
	else {
		for (i = 0; i < this->length; i++) {
			if (node->data.pid == pid)
			{
				return node->data;
			}
			node = node->next;
		}
		printf("non exist such data\n");
		return data;
	}

}

void Print(List *this) {
	int i = 0;
	Node *currentNode = this->header;
	if (this->length == 0) {
		printf("non existing Data\n");
		return;
	}

	for (i = 0; i < this->length; i++) {
		Data_Print(currentNode->data);
		currentNode = currentNode->next;
	}
}

void Data_Print(Data data) {
	printf("%5d\t", data.pid); //unsigned int pid;
	printf("%-10s\t", data.username); //char user[strlen];
	printf("%.2f\t", data.per_cpu);//double per_cpu;
	printf("%.2f\t", data.per_mem);//double per_mem;
	printf("%-15s\t", data.command);//char commend[strLen];
	printf("\n");

	/*
	//not yet
	printf("%s\t", data.pr); //char pr[strLen];
	printf("%c\t", data.ni); //	char ni;
	printf("%u\t", data.virt); //unsigned int virt;
	printf("%u\t", data.res); //unsigned int res;
	printf("%u\t", data.shr); //unsigned int shr;
	printf("%c\t", data.s); //char s;
	printf("%u\t", data.time);//unsigned long time;
	printf("\n");
	*/
}

void Data_Sort_By_PerCpu(List *this) { // percpu 

	int cnt = 0;
	Node* loop = this->header;
	Data* resList = malloc(sizeof(Data)*this->length);
	for (cnt = 0; cnt < this->length; cnt++) {
		memcpy(&resList[cnt], &loop->data, sizeof(loop->data));
		loop = loop->next;
	}

	qsort(resList, this->length, sizeof(*resList), comp);
	loop = this->header;
	for (cnt = 0; cnt < this->length; cnt++) {
		memcpy(&loop->data, &resList[cnt], sizeof(loop->data));
		loop = loop->next;
	}

	free(resList);
}


int comp(const void * elem1, const void * elem2) {	

	Data* f = ((Data*)elem1);
	Data* s = ((Data*)elem2);
	return (int)(s->per_cpu - f->per_cpu);
}

void freeList(pInfo **head)
{
	free(*head);
}