/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 17:04:14 by ccarnot           #+#    #+#             */
/*   Updated: 2023/09/15 10:25:23 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"

# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef struct s_pipex
{
	int		**tab;
	int		cmd_nb;
	int		in_fd;
	int		out_fd;
	char	**binaries;
	char	**env;
	int		*pidtab;
	int		hdoc;
	int		in_tmp;
	int		out_tmp;
}		t_pipex;

t_pipex	*pipex_init(int argc, char **environ);
void	check_files(t_pipex *pipex, char **argv, int argc);
void	set_pipes_tabs(t_pipex *pipex);
void	clear_pipes(t_pipex *pipex);
void	clear_all(t_pipex *pipex);
void	create_pipeline(t_pipex *pipex, char **argv);
void	create_tubes(t_pipex *pipex);
void	child_process(t_pipex *pipex, int i);
void	parent_process(t_pipex *pipex);
void	exec_child(char *cmd, t_pipex *pipex);
char	*get_path(char **argv, t_pipex *pipex);
void	clear_tabdouble(char **tab);
char	**get_binary_paths(char **env);
void	close_unneeded_pipes(t_pipex *pipex, int i);
int		close_if(int *fd);
void	tabpid_init(t_pipex *pipex);
void	clear_open_fd(t_pipex *pipex);
char	*ft_strjoin_slash(char const *s1, char const *s2);
void	exec_absolute_path(char **argv, t_pipex *pipex);
void	end_msg(char **argv, t_pipex *pipex);
char	**parse_cmd(char *cmd, t_pipex *pipex);
char	**parse_with_quotes(char *cmd);
char	**final_argv(char **tmp1, char **tmp2, int size);
void	here_doc_main(t_pipex *pipex, int argc, char *argv[], char **environ);
int		does_heredoc_file_exist(char *argv[]);
void	define_heredoc(char **argv, t_pipex *pipex);
void	heredoc_files(t_pipex *pipex, char **argv, int argc);
void	child_process_next(t_pipex *pipex, int i);
void	single_process(t_pipex *pipex, char **argv);
void	unlink_if(t_pipex *pipex);

#endif
