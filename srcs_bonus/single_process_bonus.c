/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 11:51:59 by ccarnot           #+#    #+#             */
/*   Updated: 2023/09/15 10:28:30 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	single_process(t_pipex *pipex, char **argv)
{
	int	pid;
	int	status;
	int	err;

	err = 0;
	pid = fork();
	if (pid == -1)
		perror("pip20 - fork failed");
	else if (pid == 0)
	{
		if (dup2(pipex->in_fd, STDIN_FILENO) == -1)
			(perror("pip16a- dup2"), clear_all(pipex), exit(1));
		if (dup2(pipex->out_fd, STDOUT_FILENO) == -1)
			(perror("pip16b- dup2"), clear_all(pipex), exit(1));
		exec_child(argv[2 + pipex->hdoc], pipex);
	}
	else
	{
		close_if(&pipex->in_fd);
		close_if(&pipex->out_fd);
		err = waitpid(pid, &status, 0);
		if (err == -1)
			perror("pip20 - waitpid failed");
		clear_all(pipex);
	}
}
