/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop-2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquinodo <jquinodo@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 16:45:21 by jquinodo          #+#    #+#             */
/*   Updated: 2025/05/05 17:00:24 by jquinodo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>


int	ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	size_t pid;

	pipe(fd);

	if (type == 'r')
	{
		pid = fork();

		if (pid == 0)
		{
			dup2(fd[1], 1);
			close(fd[1]);
			close(fd[0]);
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
			close(fd[1]);
			close(fd[0]);
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
	int fd = ft_popen("ls", ( char *[]) {"ls", NULL}, 'r');
	char buf[1];
	while(read(fd, buf, 1))
		write(1, buf, 1);

	close(fd);
	return (0);
}