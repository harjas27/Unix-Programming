#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <time.h>

void my_pause()
{
	while(1)
		pause();
}

void main(int argc, char *argv[])
{
	int n,i;
	n=atoi(argv[1]);
	int ind,pid;
	char s[4]="1001";
	long long int data=2*1024*8;//size for 1GB file
	pid_t parr[n];
	clock_t end,begin;
	int *fds;
	double total_tf,total_pipe;
	fds=(int*)malloc((n)*sizeof(int));
	total_tf=0;
	char d='\0';
	char null[1];
	null[0]=d;
	char *names[n];
	for(i=0;i<n;i++)
	{
		names[i]=(char*)malloc(11*sizeof(char));
		strcpy(names[i],"file-XXXXXX");
	}
	ind=0;
	parr[ind]=pid;
	fds[ind]=mkstemp(names[ind]);
	unlink(names[ind]);
	if(fds[ind]<1)
	{
		printf("error\n");
		exit(0);
	}
	printf("*****************TEMP FILES******************\n");
	begin=clock();
	for(long long int i=1;i<=data;i++)
	{
		if(write(fds[ind],s,sizeof(s))<1)
			printf("error\n");
	}
	end=clock();
	double t=(double)(end-begin)/CLOCKS_PER_SEC;
	total_tf+=t;
	printf("Time taken by %dth process= %lf\n",ind+1,t);	
	pid=fork();
	if(pid > 0)
		my_pause();
	for(i=0;i<n-2;i++)
	{
		if(pid == 0)
		{
			ind=i+1;
			parr[ind]=getpid();
			fds[ind]=mkstemp(names[ind]);
			unlink(names[ind]);
			int fd_r=fds[ind-1];
			lseek(fd_r,0,SEEK_SET);
			char c[1];
			begin=clock();
			while(read(fd_r,c,1) != 0)
			{
				if(write(fds[ind],c,sizeof(c))<1)
					printf("error\n");
			}
			end=clock();
			double t=(double)(end-begin)/CLOCKS_PER_SEC;
			total_tf+=t;
			printf("Time taken by %dth process= %lf\n",ind+1,t);
			close(fds[ind-1]);
			pid=fork();
		}
		if(pid > 0)
			my_pause();
	}
	ind=n-1;
	parr[ind]=getpid();
	int fd_r=fds[ind-1];
	lseek(fd_r,0,SEEK_SET);
	char c[1];
	begin=clock();
	while(read(fd_r,c,1) != 0);
	end=clock();
	t=(double)(end-begin)/CLOCKS_PER_SEC;
	total_tf+=t;
	printf("Time taken by %dth process= %lf\n",ind+1,t);
	close(fds[ind-1]);
	printf("Total time using temp files = %lf\n",total_tf);
	
	

	//pipe
	printf("*****************PIPE******************\n");
	total_pipe=0;
	ind=0;
	int p[2];
	pipe(p);
	begin=clock();
	for(long long int i=1;i<=data;i++)
	{
		if(write(p[1],s,sizeof(s))<1)
			printf("error\n");
	}
	write(p[1],null,sizeof(null));
	end=clock();
	t=(double)(end-begin)/CLOCKS_PER_SEC;
	total_pipe+=t;
	printf("Time taken by %dth process= %lf\n",ind+1,t);
	pid=fork();
	if(pid > 0)
		my_pause();
	for(i=0;i<n-2;i++)
	{
		if(pid == 0)
		{
			ind=i+1;
			char c[1];
			begin=clock();
			while(read(p[0],c,1) != 0)
			{
				if(strcmp(c,null) == 0)
					break;
				if(write(p[1],c,sizeof(c))<1)
					printf("error\n");
			}
			write(p[1],null,sizeof(null));
			end=clock();
			double t=(double)(end-begin)/CLOCKS_PER_SEC;
			total_pipe+=t;
			printf("Time taken by %dth process= %lf\n",ind+1,t);
			pid=fork();
		}
		if(pid > 0)
		{
			exit(0);
		}
	}
	ind=n-1;
	begin=clock();
	while(read(p[0],c,1) != 0)
	{
		if(strcmp(c,null) == 0)
			break;
	}
	end=clock();
	t=(double)(end-begin)/CLOCKS_PER_SEC;
	total_pipe+=t;
	printf("Time taken by %dth process= %lf\n",ind+1,t);	
	printf("Total time using pipes = %lf\n",total_pipe);
	
	//exit
	for(i=0;i<n-1;i++)
		kill(parr[i],SIGKILL);
}
