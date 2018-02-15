#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int lev;

void my_wait(int n)
{
	for(int i=0;i<n;i++)
		wait();
}

void recur(int n)
{
	if(n>0)
	{
		int pid=getpid();
		int i;
		for(i=0;i<n;i++)
		{
			if(pid > 0)
			{
				pid=fork();
				if(pid==0)
				{
					recur(n-1);
					my_wait(n-1);
					printf("%d          %d        %d        %d\n",lev-n+1,getpid(),getppid(),i+1);
					exit(0);
				}
			}
		}
	}
		
}

void main(int argc, char *argv[])
{
	int n=0;
	if(argc == 2)
	{
		int l=0;
		int p=1;
		int i;
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
	lev=n;
	printf("Level       pid          ppid       Position\n");
	recur(n);
	for(int i=0;i<n;i++)
		wait();
	printf("\nAll Children Exited\n");
	exit(0);
}
