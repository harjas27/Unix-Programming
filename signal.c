#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int pid,rpid,f;
void sig_handler1(int signo)
{	
	if(getpid()==rpid)
		printf("%d\n",getpid());
	else
	{
		printf("%d\t",getpid());
		fflush(stdout);
		kill(pid,SIGUSR1);
	}	
	//pause();
}

void sig_handler2(int signo)
{
	exit(0);
}

void main(int argc, char *argv[])
{
	//setbuf(stdout, NULL);
	int n;
	int i,j,k;
	f=0;
	if(argc == 2)
	{
		int l=0;
		int p=1;
		for(i=0;;i++)
		{
			if(argv[1][i] != '\0')
			{
				p*=10;
				l+=1;
			}
			else
				break;
		}
		for(i=0;i<l;i++)
		{
			p/=10;
			n+=(p*(argv[1][i]-48));
		}
	}
	else
		scanf("%d",&n);	
		
	signal(SIGUSR1,sig_handler1);	
	rpid=-1;
	pid_t parr[n];
	for(i=0;i<n;i++)
	{
		parr[i]=-1;
	}
	signal(SIGUSR2,sig_handler2);
	pid=getpid();
	parr[0]=pid;
	//printf("initial %d \n",getpid());
	if(n==1)
	{
		for(i=0;i<1000;i++)
		{
			printf("%d\t\t",i+1);
			printf("%d\n",getpid());
		}
	}
	else if(n==2)
	{
		pid=fork();
		//printf("last %d\n",getpid());
		if(pid > 0)
		{
			while(1)
				pause();
		}
		parr[n-1]=getpid();
		rpid=getpid();
		/*for(i=0;i<n;i++)
		{
			printf("%d\t",parr[i]);
		}*/
		for(i=0;i<1000;i++)
		{
			printf("%d\t\t",i+1);
			fflush(stdout);
			//printf("%d\t",parr[j]);
			kill(parr[0],SIGUSR1);
			//while(1);
			pause();
			//sleep(1);
			//printf("%d\n",getpid());
		}
		for(j=0;j<n-1;j++)
		{	
			//printf("%d\t",parr[j]);
			kill(parr[j],SIGUSR2);
		}
	}
	else
	{
		pid=fork();
		for(i=0;i<n-2;i++)
		{
			if(pid == 0)
			{
				parr[i+1]=getpid();
				pid=fork();
			}
			if(pid > 0)
			{
				//printf("pause: %d\n",getpid());
				while(1)
					pause();
			}
		}
		//printf("last %d\n",getpid());
		parr[n-1]=getpid();
		rpid=getpid();
		/*for(i=0;i<n;i++)
		{
			printf("%d\t",parr[i]);
		}*/
		for(i=0;i<1000;i++)
		{
			printf("%d\t\t",i+1);
			fflush(stdout);
			//printf("%d\t",parr[j]);
			kill(parr[0],SIGUSR1);
			//while(1);
			pause();
			//sleep(1);
			//printf("%d\n",getpid());
		}
		for(j=0;j<n-1;j++)
		{	
			//printf("%d\t",parr[j]);
			kill(parr[j],SIGUSR2);
		}
	}
}
