/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tubeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 17:00:25 by ccarnot           #+#    #+#             */
/*   Updated: 2023/09/18 18:15:27 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	create_pipeline(t_pipex *pipex, char **argv)
{
	int		i;
	pid_t	pid;

	tabpid_init(pipex);
	create_tubes(pipex);
	i = -1;
	while (++i < pipex->cmd_nb)
	{
		pid = fork();
		if (pid == -1)
			ft_printf("pip8- fork %d failed: %s\n", i, strerror);
		else if (pid == 0)
		{
			if (pipex->in_tmp && (i == 0))
				(clear_all(pipex), exit(1));
			if (pipex->out_tmp && (i == pipex->cmd_nb - 1))
				(clear_all(pipex), exit(1));
			close_unneeded_pipes(pipex, i);
			child_process(pipex, i);
			exec_child(argv[i + 2 + pipex->hdoc], pipex);
		}
		else
			pipex->pidtab[i] = pid;
	}
	parent_process(pipex);
}

void	create_tubes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_nb - 1)
	{
		if (pipe(pipex->tab[i]) == -1)
		{
			ft_printf("pip7- bad pipe(tab[%d]) creation: %s\n", i, strerror);
			clear_all(pipex);
			exit(1);
		}
		i++;
	}
}

void	close_unneeded_pipes(t_pipex *pipex, int i)
{
	int	j;

	j = 0;
	while (j < pipex->cmd_nb - 1)
	{
		if (j != i -1)
		{
			close_if(&pipex->tab[j][0]);
		}
		if (j != i)
		{
			close_if(&pipex->tab[j][1]);
		}
		j++;
	}
}

void	parent_process(t_pipex *pipex)
{
	int	j;
	int	status;
	int	err;

	err = 0;
	j = 0;
	while (j < pipex->cmd_nb -1)
	{
		close_if(&pipex->tab[j][0]);
		close_if(&pipex->tab[j][1]);
		j++;
	}
	close_if(&pipex->in_fd);
	close_if(&pipex->out_fd);
	j = 0;
	while (j < pipex->cmd_nb)
	{
		err = wait(NULL);
		err = waitpid(pipex->pidtab[j], &status, WNOHANG);
		if (err == -1)
			ft_printf("pip19- no wait for process %d : %s\n", strerror(errno));
		j++;
	}
}

void	tabpid_init(t_pipex *pipex)
{
	int	i;

	pipex->pidtab = malloc(sizeof(int) * pipex->cmd_nb);
	if (!pipex->pidtab)
	{
		perror("pip6 - bad malloc pipex->tabpid");
		clear_all(pipex);
		exit(1);
	}
	i = -1;
	while (++i < pipex->cmd_nb)
		pipex->pidtab[i] = -1;
}
