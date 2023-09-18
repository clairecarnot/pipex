/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/31 16:53:21 by ccarnot           #+#    #+#             */
/*   Updated: 2023/09/13 18:29:26 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	child_process(t_pipex *pipex, int i)
{
	if (i == 0)
	{
		close_if(&pipex->out_fd);
		if (dup2(pipex->in_fd, STDIN_FILENO) == -1)
			(perror("pip14a- dup2"), clear_all(pipex), exit(1));
		(close_if(&pipex->in_fd), close_if(&pipex->tab[i][0]));
		if (dup2(pipex->tab[i][1], STDOUT_FILENO) == -1)
			(perror("pip14b- dup2"), clear_all(pipex), exit(1));
		close_if(&pipex->tab[i][1]);
	}
	else if (i == pipex->cmd_nb - 1)
	{
		(close_if(&pipex->in_fd), close_if(&pipex->tab[i - 1][1]));
		if (dup2(pipex->tab[i - 1][0], STDIN_FILENO) == 1)
			(perror("pip14c- dup2"), clear_all(pipex), exit(1));
		close_if(&pipex->tab[i - 1][0]);
		if (dup2(pipex->out_fd, STDOUT_FILENO) == -1)
			(perror("pip14d- dup2"), clear_all(pipex), exit(1));
		close_if(&pipex->out_fd);
	}
	else
		child_process_next(pipex, i);
}

void	child_process_next(t_pipex *pipex, int i)
{
	(close_if(&pipex->in_fd), close_if(&pipex->out_fd));
	close_if(&pipex->tab[i - 1][1]);
	if (dup2(pipex->tab[i - 1][0], STDIN_FILENO) == -1)
		(perror("pip14e- dup2"), clear_all(pipex), exit(1));
	close_if(&pipex->tab[i - 1][0]);
	if (dup2(pipex->tab[i][1], STDOUT_FILENO) == -1)
		(perror("pip14f- dup2"), clear_all(pipex), exit(1));
	close_if(&pipex->tab[i][1]);
}

void	exec_child(char *cmd, t_pipex *pipex)
{
	char	**argv;
	char	*path;

	close_if(&pipex->in_fd);
	close_if(&pipex->out_fd);
	argv = parse_cmd(cmd, pipex);
	if (argv[0][0] == '/')
		exec_absolute_path(argv, pipex);
	else
	{
		path = get_path(argv, pipex);
		free(argv[0]);
		argv[0] = path;
		execve(argv[0], argv, pipex->env);
		(clear_tabdouble(argv), clear_all(pipex));
		exit(1);
	}
}

char	*get_path(char **argv, t_pipex *pipex)
{
	char	*path;
	int		i;

	i = 0;
	while (pipex->binaries && pipex->binaries[i])
	{
		path = ft_strjoin_slash(pipex->binaries[i], argv[0]);
		if (!path)
		{
			(clear_tabdouble(argv), clear_all(pipex));
			(perror("pip10- strjoin /binaries failed"), exit(1));
		}
		if (access(path, F_OK | X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	end_msg(argv, pipex);
	(clear_tabdouble(argv), clear_all(pipex));
	exit(1);
}

void	exec_absolute_path(char **argv, t_pipex *pipex)
{
	if (access(argv[0], F_OK | X_OK) == 0)
	{
		execve(argv[0], argv, pipex->env);
		(clear_tabdouble(argv), clear_all(pipex));
		exit(1);
	}
	write(2, argv[0], ft_strlen(argv[0]));
	write(2, ": No such file or directory\n", 28);
	(clear_tabdouble(argv), clear_all(pipex));
	exit(1);
}
