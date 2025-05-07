/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop-version-optimisée.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquinodo <jquinodo@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 18:18:20 by jquinodo          #+#    #+#             */
/*   Updated: 2025/05/05 18:35:54 by jquinodo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <stdlib.h>


// FULL CONTROLES ; MAIS PAS FORCEMENT OBLIGATOIRE !

int	ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	pid_t pid;
	pipe(fd);

	if(!file || !argv || (type != 'r' && type != 'w'))
		return -1;

	if (pipe(fd) == -1)
		return -1;


	pid = fork();

	if (pid == -1)
	{
		close(fd[0]);
		close(fd[1]);
		return -1;
	}

	if(type == 'r')
	{
		if(pid == 0)
		{
			dup2(fd[1], 1);
			close(fd[1]);
			close(fd[0]);
			execvp(file, (char *const*)argv);
			exit (-1);
		}
		close(fd[1]);
		return fd[0];
	}

	if(type == 'w')
	{
		if(pid == 0)
		{
			dup2(fd[0], 0);
			close(fd[1]);
			close(fd[0]);
			execvp(file, (char *const*)argv);
			exit (-1);
		}
		close(fd[0]);
		return fd[1];
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
	int fd = ft_popen("pwd", ( char *[]) {"ls", NULL}, 'r');
	char buf[1];
	while(read(fd, buf, 1))
		write(1, buf, 1);

	close(fd);
	return (0);
}