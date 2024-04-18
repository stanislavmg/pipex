#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main (int argc, char * argv[])
{ 
	int pipedes[2];
	pid_t pid;

	pipe(pipedes);
	pid = fork();
	if ( pid == 0 ) 
	{
		close(pipedes[0]);
		dup2(pipedes[1], 1);
		execve("/usr/sbin/ifconfig", NULL, NULL);
		close(pipedes[1]);
	}
	else
	{
		char buf[1024];
		int len;
		close(pipedes[1]);
		while ((len = read(pipedes[0], buf, 1024)) != 0)
			write(2, buf, len);
		close(pipedes[0]);
	}
	return 0;
}