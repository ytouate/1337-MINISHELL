/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 17:01:01 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_echo(t_commande *command, char *s, char flag)
{
	int fd;
	fd = -1;
	if (command->output != NULL)
		fd = open_output_files(command);
	if (fd == -1)
		fd = STDOUT_FILENO;
	if (s == NULL)
	{
		write(fd, "\n", 2);
		return ;
	}
	if (flag == 'n')
		write(fd, s, ft_strlen(s));
	else
	{
		write(fd, s, ft_strlen(s));
		write(fd, "\n", 1);
	}
}

char	*join_for_echo(t_list *env_list, char **s, char flag)
{
	int		i;
	char	*result;
	char	**temp;

	if (flag == 'n')
		i = 2;
	else
		i = 1;
	result = "";
	while (s[i])
	{
		if (ft_strcmp(s[i], "~") == 0)
		{
			if (ft_getenv(env_list, "HOME") != NULL)
			{
				temp = ft_split(ft_getenv(env_list, "HOME")->content, '=');
				if (temp[1] != NULL)
					result = ft_strjoin(result, temp[1]);
				free_2d_array(temp);	
			}
		}
		else
			result = ft_strjoin(result, s[i]);
		result = ft_strjoin(result, " ");
		i++;
	}
	result = ft_strtrim(result, " ");
	return (result);
}
