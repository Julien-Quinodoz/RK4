/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop-13.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquinodo <jquinodo@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 15:47:11 by jquinodo          #+#    #+#             */
/*   Updated: 2025/05/12 15:58:19 by jquinodo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <unistd.h>
# include <stdlib.h>

int	ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	pid_t pid;

	pipe(fd);             // pas intervertir
	pid = fork();			// ses deux fonction
						// pipe en premier !!


	if(type == 'r')
	{
		if(pid == 0)
		{
			dup2(fd[1], 1);
			close(fd[1]);
			close(fd[0]);
			execvp(file, (char *const*)argv);
			exit(-1);
		}
		close(fd[1]);
		return (fd[0]);
	}
	if(type == 'w')
	{
		if(pid == 0)
		{
			dup2(fd[0], 0);
			close(fd[1]);
			close(fd[0]);
			execvp(file, (char *const*)argv);
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
	int fd = ft_popen("ls", (char *[]) {"ls", NULL}, 'r');
	char buf[1];
	while(read(fd, buf, 1))
		write(1, buf, 1);

	close(fd);
	return (0);
}
