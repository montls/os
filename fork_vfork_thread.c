#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/wait.h>

int total = 0;
pthread_mutex_t mutex;

void *count_txt(void *t){
	long thread_id = (long)t;
    char c;
	printf("Here is %lu\n", pthread_self());
	while((c=fgetc(t))!=EOF)
	{
			pthread_mutex_lock(&mutex);
			total++;
			pthread_mutex_unlock(&mutex);
	}		
	pthread_exit(NULL);
}

int main(int arc, char *argv[])
{
    int pid,pid_t,i;
	FILE *fp[2];
	char ch;
	char *para[] = {"./a1","./a2"};
	pthread_t threads[2];
	int rc;
	
	// open two files before fork()
	for(i=0;i<2;i++)
  	{
		if((fp[i]=fopen(para[i],"w+"))==NULL)
		{
			printf("cannot open this file.\n");
			exit(0);
		}
	}

	// fork() BEGIN:
	while((pid=fork())==-1);
	
	// children process
	if(pid==0)	
	{
		for(i=0;i<2;i++)
		{
			printf("请输入a%d文件的内容，#结束：\n",i+1);
			while((ch=getchar())!='#')
				fputc(ch,fp[i]);
            getchar();
		}
		fclose(fp[0]);
		fclose(fp[1]);
		exit(1);
	}

	// parent process
	wait(0);		
	pthread_mutex_init(&mutex, NULL);
	fseek(fp[0],0,SEEK_SET);
	fseek(fp[1],0,SEEK_SET);
	rc = pthread_create(&threads[0],NULL,count_txt,(void *)fp[0]);
	rc = pthread_create(&threads[1],NULL,count_txt,(void *)fp[1]);	
	if (rc) {
		perror("error");
		exit(1);
	}  

	pthread_join(threads[0],NULL);
	pthread_join(threads[1],NULL);
	pthread_mutex_destroy(&mutex);
	printf("a1、a2文件中共有: %d个字符\n",total);
	fclose(fp[0]);
	fclose(fp[1]);

	// wait(0);
	while((pid_t=vfork())==-1);
	if(pid_t==0)
	{
		execl("./change",para[0],para[1],NULL);
		exit(0);
	}
	wait(0);

}
