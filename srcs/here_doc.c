/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 10:45:35 by ccarnot           #+#    #+#             */
/*   Updated: 2023/09/18 15:48:12 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	here_doc_main(t_pipex *pipex, int argc, char *argv[], char **environ)
{
	if (does_heredoc_file_exist(argv))
		return ;
	if (argc >= 5)
	{
		pipex = pipex_init(argc - 1, environ);
		define_heredoc(argv, pipex);
		pipex->hdoc = 1;
		heredoc_files(pipex, argv, argc);
		set_pipes_tabs(pipex);
		pipex->binaries = get_binary_paths(environ);
		if (argc == 5)
			single_process(pipex, argv);
		else
		{
			create_pipeline(pipex, argv);
			clear_all(pipex);
		}
	}
	exit(0);
}

int	does_heredoc_file_exist(char *argv[])
{
	int	fd;

	fd = open(argv[1], O_RDONLY);
	if (fd > 0)
	{
		close_if(&fd);
		return (1);
	}
	else
		return (0);
}

void	define_heredoc(char **argv, t_pipex *pipex)
{
	char	*line;
	int		len_line;
	int		len_argv;

	len_argv = ft_strlen(argv[2]);
	pipex->in_fd = open("/tmp/here_doc", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	if (pipex->in_fd < 0)
		(perror("pip11- failed to create here_doc"), free(pipex), exit(1));
	while (1)
	{
		(ft_putstr_fd("here_doc>", 1), line = get_next_line(0, 0));
		if (!line)
		{
			(perror("pip12- pb with gnl"), close_if(&pipex->in_fd));
			(unlink("/tmp/here_doc"), free(pipex), exit(1));
		}
		len_line = ft_strlen(line);
		if ((ft_strncmp(line, argv[2], ft_strlen(argv[2])) == 0)
			&& (len_line == len_argv + 1))
		{
			(get_next_line(0, 1), free(line), close_if(&pipex->in_fd));
			break ;
		}
		(ft_putstr_fd(line, pipex->in_fd), free(line));
	}
}

void	heredoc_files(t_pipex *pipex, char **argv, int argc)
{
	pipex->in_fd = open("/tmp/here_doc", O_RDONLY);
	if (pipex->in_fd < 0)
	{
		perror("pip17- failed to open here_doc");
		pipex->in_fd = open("/tmp/infile_tmp", O_RDONLY | O_CREAT, S_IRUSR);
		if (pipex->in_fd < 0)
			(perror("pip18- failed to create infile"), free(pipex), exit(1));
		pipex->in_tmp = 1;
	}
	pipex->out_fd = open(argv[argc - 1], O_WRONLY | O_TRUNC | O_CREAT
			| O_APPEND, S_IRUSR | S_IWUSR);
	if (pipex->out_fd < 0)
	{
		perror("pip13- failed to open outfile");
		pipex->out_fd = open("/tmp/outfile_tmp", O_WRONLY | O_TRUNC | O_CREAT
				| O_APPEND, S_IRUSR | S_IWUSR);
		if (pipex->out_fd < 0)
		{
			perror("pip22- failed to create outfile");
			close_if(&pipex->in_fd);
			unlink_if(pipex);
			(free(pipex), exit(1));
		}
		pipex->out_tmp = 1;
	}
}

void	unlink_if(t_pipex *pipex)
{
	if (pipex->hdoc)
		unlink("/tmp/here_doc");
	if (pipex->in_tmp)
		unlink("/tmp/infile_tmp");
}
