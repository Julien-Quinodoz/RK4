/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pop-16.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jquinodo <jquinodo@student.42lausanne.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 15:50:21 by jquinodo          #+#    #+#             */
/*   Updated: 2025/05/14 15:54:23 by jquinodo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>


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

//======== TESTE DE POPEN =======================================

# include <stdio.h>
# define Y "\033[38;5;226m"
# define S "\033[0m"

void test_command(const char *cmd, char *const args[], char type)
{
    printf(Y"\n🔍 Test de commande :\033[0m  %s\n", cmd);
    int fd = ft_popen(cmd, args, type);
    if (fd == -1) {
        perror("Erreur lors de l'exécution de ft_popen");
        return;
    }

    char buf[1024];  // Lecture optimisée
    ssize_t bytes;
    while ((bytes = read(fd, buf, sizeof(buf))) > 0)
        write(1, buf, bytes);

    close(fd);
}

int main()
{
    printf(Y"\n 🚀 Démarrage du test de ft_popen\n"S);

    test_command("ls", (char *[]) {"ls", NULL}, 'r');   // Liste détaillée du dossier
    test_command("pwd", (char *[]) {"pwd", NULL}, 'r');        // Affiche le chemin courant
    test_command("echo", (char *[]) {"echo", "Hello teste 42 -- > OK !", NULL}, 'r'); // Affiche un message
    test_command("date", (char *[]) {"date", NULL}, 'r');      // Affiche la date actuelle

    printf(Y"\n✅ Tous les tests terminés !\n"S);
    return 0;
}