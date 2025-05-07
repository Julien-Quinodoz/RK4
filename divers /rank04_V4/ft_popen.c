#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

int ft_popen(const char *file, const char *argv[], char type)
{
	int fd[2];
	pid_t pid;
	
	
	pipe(fd);
	if (type == 'r')
	{
		pid = fork();
		if (pid == 0)
		{
			dup2(fd[1], 1);
			close(fd[0]);
			close(fd[1]);
			execvp(file, (char *const *)argv);
			exit(-1);
		}
		close(fd[1]);
		return (fd[0]);
	}
	if (type == 'w')
	{
		pid = fork();
		if (pid == 0)
		{
			dup2(fd[0], 0);
			close(fd[0]);
			close(fd[1]);
			execvp(file, (char *const *)argv);
			exit(-1);
		}
		close(fd[0]);
		return (fd[1]);
	}
	return -1;
}



#include <fcntl.h>
#include <stdio.h>
#include <string.h>

//test type 'r'
int main()
{
	//int fd = open("texte", O_RDONLY);
	int fd = ft_popen("ls", (const char *[]) {"ls", NULL}, 'r');
	char buf[1];
	while(read(fd, buf, 1))
		write(1, buf, 1);

	close(fd);
	return (0);
}
