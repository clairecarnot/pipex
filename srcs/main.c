/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 17:07:28 by ccarnot           #+#    #+#             */
/*   Updated: 2023/09/18 15:47:16 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char *argv[], char **environ)
{
	t_pipex	*pipex;

	pipex = NULL;
	if (argc > 4)
	{
		if (ft_strncmp(argv[1], "here_doc", 8) == 0)
			here_doc_main(pipex, argc, argv, environ);
		pipex = pipex_init(argc, environ);
		check_files(pipex, argv, argc);
		set_pipes_tabs(pipex);
		pipex->binaries = get_binary_paths(environ);
		if (argc == 4)
			single_process(pipex, argv);
		else
		{
			create_pipeline(pipex, argv);
			clear_all(pipex);
		}
	}
	else
		write(1, "Min 4 arguments required\n", 25);
	return (0);
}

t_pipex	*pipex_init(int argc, char **environ)
{
	t_pipex	*pipex;

	pipex = malloc(sizeof(t_pipex));
	if (!pipex)
	{
		perror("pip1- bad malloc on pipex struct");
		exit(1);
	}
	pipex->tab = NULL;
	pipex->cmd_nb = argc - 3;
	pipex->in_fd = -1;
	pipex->out_fd = -1;
	if (environ)
		pipex->env = environ;
	pipex->pidtab = NULL;
	pipex->binaries = NULL;
	pipex->hdoc = 0;
	pipex->in_tmp = 0;
	pipex->out_tmp = 0;
	return (pipex);
}

void	check_files(t_pipex *pipex, char **argv, int argc)
{
	pipex->in_fd = open(argv[1], O_RDONLY);
	if (pipex->in_fd < 0)
	{
		perror("pip2- failed to open infile");
		pipex->in_fd = open("/tmp/infile_tmp", O_RDONLY | O_CREAT, S_IRUSR);
		if (pipex->in_fd < 0)
			(perror("pip15- failed to create infile"), free(pipex), exit(1));
		pipex->in_tmp = 1;
	}
	pipex->out_fd = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT,
			S_IRUSR | S_IWUSR);
	if (pipex->out_fd < 0)
	{
		perror("pip3- failed to open outfile");
		pipex->out_fd = open("/tmp/outfile_tmp", O_WRONLY | O_TRUNC | O_CREAT,
				S_IRUSR | S_IWUSR);
		if (pipex->out_fd < 0)
		{
			(perror("pip21- failed creating outfile"), close_if(&pipex->in_fd));
			if (pipex->in_tmp)
				unlink("/tmp/infile_tmp");
			(free(pipex), exit(1));
		}
		pipex->out_tmp = 1;
	}
}

void	set_pipes_tabs(t_pipex *pipex)
{
	int	i;

	pipex->tab = malloc(sizeof(int *) * (pipex->cmd_nb - 1));
	if (!pipex->tab)
	{
		perror("pip4- bad malloc on pipex->tab");
		(clear_all(pipex), exit(1));
	}
	i = -1;
	while (++i < pipex->cmd_nb - 1)
		pipex->tab[i] = NULL;
	i = -1;
	while (++i < pipex->cmd_nb - 1)
	{
		pipex->tab[i] = malloc(sizeof(int) * 2);
		if (!pipex->tab[i])
		{
			ft_printf("pip5- bad malloc on pipex->tab[%d]: %s\n",
				i, strerror(errno));
			(clear_all(pipex), exit(1));
		}
		pipex->tab[i][0] = -1;
		pipex->tab[i][1] = -1;
	}
}

char	**get_binary_paths(char **env)
{
	char	*path;
	char	**binaries;
	char	**environ;

	if (env)
		environ = env;
	path = NULL;
	while (env && *environ)
	{
		if (ft_strncmp("PATH=", *environ, 5) == 0)
		{
			path = ft_strdup(*environ + 5);
			if (!path)
				return (NULL);
			break ;
		}
		environ++;
	}
	if (!path)
		return (NULL);
	binaries = ft_split(path, ':');
	if (!binaries)
		return (free(path), NULL);
	free(path);
	return (binaries);
}
