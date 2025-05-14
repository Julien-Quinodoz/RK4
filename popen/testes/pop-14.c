/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop-14.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquinodo <jquinodo@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:19:21 by jquinodo          #+#    #+#             */
/*   Updated: 2025/05/14 15:29:31 by jquinodo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <stdlib.h>
# include <unistd.h>


int	ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	pipe(fd);
	pid_t pid;
	pid = fork();

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
			exit (-1);
		}
		close(fd[0]);
		return (fd[1]);
	}
	return -1;
}


int main()
{
	//int fd = open("texte", O_RDONLY);
	int fd = ft_popen("pwd", (char *[]) {"pwd", NULL}, 'r');
	char buf[1];
	while(read(fd, buf, 1))
		write(1, buf, 1);

	close(fd);
	return (0);
}
