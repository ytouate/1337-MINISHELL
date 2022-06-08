/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/08 20:38:18 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_echo(t_command *command, char *s, char flag)
{
	int		fd;

	fd = open_files(*command).fd_out;
	if (fd == -1)
		return ;
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
	set_exit_code(0);
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

bool	exec_echo(t_vars vars, t_command *command)
{
	if (ft_strcmp(command->flags[0], "echo") == 0 || \
		ft_strcmp(command->flags[0], "ECHO") == 0)
	{
		if (command->flags[1] == NULL)
			ft_echo(command, NULL, '0');
		else if ((check_echo_flag(command->flags[1])))
			ft_echo(command, join_for_echo(vars.env_list, \
				command->flags, 'n'), 'n');
		else
			ft_echo(command, join_for_echo(vars.env_list, \
				command->flags, '\0'), '\0');
		return (true);
	}
	return (false);
}
