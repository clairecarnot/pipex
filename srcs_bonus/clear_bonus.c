/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 16:00:15 by ccarnot           #+#    #+#             */
/*   Updated: 2023/09/15 10:27:32 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

int	close_if(int *fd)
{
	if (*fd > 0)
	{
		if (close(*fd) < 0)
		{
			ft_printf("close_if failed with fd = %d : %s\n",
				*fd, strerror(errno));
			return (-1);
		}
		*fd = -1;
	}
	return (0);
}

void	clear_pipes(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_nb - 1)
	{
		free(pipex->tab[i]);
		i++;
	}
	free(pipex->tab);
}

void	clear_open_fd(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_nb - 1)
	{
		if (pipex->tab[i])
		{
			close_if(&pipex->tab[i][0]);
			close_if(&pipex->tab[i][1]);
		}
		i++;
	}
}

void	clear_tabdouble(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

void	clear_all(t_pipex *pipex)
{
	if (pipex->tab)
		clear_open_fd(pipex);
	if (pipex->tab)
		clear_pipes(pipex);
	if (pipex->binaries)
		clear_tabdouble(pipex->binaries);
	if (pipex->pidtab)
		free(pipex->pidtab);
	close_if(&pipex->in_fd);
	if (pipex->hdoc)
		unlink("/tmp/here_doc");
	if (pipex->in_tmp)
		unlink("/tmp/infile_tmp");
	if (pipex->out_tmp)
		unlink("/tmp/outfile_tmp");
	close_if(&pipex->out_fd);
	free(pipex);
}
