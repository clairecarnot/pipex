/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/08 12:26:21 by ccarnot           #+#    #+#             */
/*   Updated: 2023/09/15 10:28:54 by ccarnot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

char	*ft_strjoin_slash(char const *s1, char const *s2)
{
	char	*dest;
	int		i;
	int		j;

	if (!s1 || !s2)
		return (0x0);
	dest = malloc(sizeof(char) * ft_strlen(s1) + ft_strlen(s2) + 2);
	if (!dest)
		return (0x0);
	i = 0;
	while (((char *)s1)[i])
	{
		dest[i] = s1[i];
		i++;
	}
	dest[i] = '/';
	i++;
	j = -1;
	while (((char *)s2)[++j])
		dest[i + j] = s2[j];
	dest[i + j] = '\0';
	return (dest);
}

void	end_msg(char **argv, t_pipex *pipex)
{
	if (ft_strchr(argv[0], '/') || !pipex->binaries)
	{
		(write(2, argv[0], ft_strlen(argv[0])),
			write(2, ": No such file or directory\n", 28));
	}
	else
	{
		(write(2, argv[0], ft_strlen(argv[0])),
			write(2, ": command not found\n", 20));
	}
}

char	**parse_cmd(char *cmd, t_pipex *pipex)
{
	char	**argv;

	if (cmd && (ft_strchr(cmd, '\'') != NULL))
		argv = parse_with_quotes(cmd);
	else
		argv = ft_split(cmd, ' ');
	if (!argv || !argv[0])
	{
		if (argv && pipex->binaries)
			(clear_tabdouble(argv), write(2, " : command not found\n", 21));
		else if (argv && !pipex->binaries)
		{
			if (ft_strlen(cmd))
				(clear_tabdouble(argv),
					write(2, " : No such file or directory\n", 29));
			else
				(clear_tabdouble(argv),
					write(2, "  : No such file or directory\n", 30));
		}
		else
			perror("pip9- bad malloc args");
		(clear_all(pipex), exit(1));
	}
	return (argv);
}

char	**parse_with_quotes(char *cmd)
{
	char	**tmp1;
	char	**tmp2;
	char	**argv;
	int		size;
	int		i;

	tmp1 = ft_split(cmd, '\'');
	if (!tmp1)
		return (NULL);
	tmp2 = ft_split(tmp1[0], ' ');
	if (!tmp2)
		return (clear_tabdouble(tmp1), NULL);
	size = 0;
	i = 0;
	while (tmp1[i++])
		size++;
	i = 0;
	while (tmp2[i++])
		size++;
	argv = final_argv(tmp1, tmp2, size);
	(clear_tabdouble(tmp1), clear_tabdouble(tmp2));
	return (argv);
}

char	**final_argv(char **tmp1, char **tmp2, int size)
{
	char	**argv;
	int		i;
	int		j;

	argv = malloc(sizeof(char *) * (size + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (tmp2[i])
	{
		argv[i] = ft_strdup(tmp2[i]);
		if (!argv[i])
			return (clear_tabdouble(argv), NULL);
		i++;
	}
	i--;
	j = 0;
	while (tmp1[++j])
	{
		argv[i + j] = ft_strdup(tmp1[j]);
		if (!argv[i + j])
			return (clear_tabdouble(argv), NULL);
	}
	argv[i + j] = NULL;
	return (argv);
}
