#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>


int ft_popen(const char *file, char *const argv[], char type)
{
	if (!file || !argv)
		return -1;
	int fd[2];
	pipe(fd);
	int pid = fork();
	if (pid == -1)
		return -1;
	if (pid == 0)
	{
		if (type == 'r')
			dup2(fd[1], STDOUT_FILENO);
		if (type == 'w')
			dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(1);
	}
	if (type == 'r')
	{
		close(fd[1]);
		return (fd[0]);
	}
	if (type == 'w')
	{
		close(fd[0]);
		return (fd[1]);
	}
	return -1;
}

/*
int main()
{
	int fd = ft_popen("/bin/ls", (char *const[]){"ls", "-la", NULL}, 'r');
	char buf[1];
	while (read(fd, buf, 1))
		write(1, buf, 1);
	return 0;
}
*/

int main(void)
{
    int fd = ft_popen("/bin/wc", (char *const[]){"wc", NULL}, 'w');
    char *input = "Hello world\nThis is a test\nthird line mofo\n";
    write(fd, input, strlen(input));
    close(fd);  // Close to signal EOF to wc

    // Wait for the child process to finish
    int status;
    wait(&status);

    return 0;
}