#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int picoshell(char ***cmds)
{
    int num_cmds = 0;
    while (cmds[num_cmds] != NULL)
        num_cmds++;
    if (num_cmds == 0)
        return 0;
    int prev_read = -1;
    int ret = 0;
    int i = 0;
    while (i < num_cmds)
    {
        int fd[2];
        int last = 0;
        if (num_cmds - i == 1)
            last = 1;
        if (!last)
        {
            if (pipe(fd) == -1)
                return 1;
        }
        pid_t pid = fork();
        if (pid == -1)
        {
            if (!last)
            {
                close(fd[0]);
                close(fd[1]);
            }
            return 1;
        }
        if (pid == 0)
        {
            if (!last)
            {
                if (dup2(fd[1], 1) == -1)
                    exit(1);
            }
            if (i > 0)
            {
                if (dup2(prev_read, 0) == -1)
                    exit(1);
            }
            if (!last)
            {
                close(fd[0]);
                close(fd[1]);
            }
            if (prev_read != -1)
                close(prev_read);
            execvp(cmds[i][0], cmds[i]);
            exit(1);
        }
        if (prev_read != -1)
            close(prev_read);
        if (!last)
        {
            prev_read = fd[0];
            close(fd[1]);
        }
        i++;
    }
    int status;
    i = 0;
    while (i < num_cmds)
    {
        wait(&status);
        if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
            ret = 1;
        i++;
    }
    return ret;
}

int main(void)
{
    // Test commands:
    // This example pipes the output of "ls -l" to "wc -l".
    char *cmd1[] = {"ls", "-l", NULL};
    char *cmd2[] = {"wc", "-l", NULL};

    // The cmds array is terminated with a NULL pointer.
    char **cmds[] = {cmd1, cmd2, NULL};

    int result = picoshell(cmds);
    printf("picoshell returned %d\n", result);
    return result;
}

