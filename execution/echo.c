/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/12 12:23:22 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_echo(t_command *command, char *s, char flag)
{
	int		fd;

	fd = open_files(*command->redi).fd_out;
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
	free(s);
	set_exit_code(EXIT_SUCCESS);
}

int	check_echo_flag(char *s)
{
	int	i;

	i = 0;
	if (s[i++] != '-')
		return (false);
	while (s[i])
	{
		if (s[i] != 'n')
			return (false);
		i += 1;
	}
	return (true);
}

char	*join_for_echo(t_list *env_list, char **s, char flag)
{
	int		i;
	char	*result;
	char	*temp;

	(void)env_list;
	if (flag == 'n')
		i = 2;
	else
		i = 1;
	result = ft_strdup("");
	while (check_echo_flag(s[i]))
		i++;
	while (s[i])
	{
		temp = result;
		result = ft_strjoin(result, s[i]);
		free(temp);
		if (s[i + 1] && s[i][0] != '\0')
		{
			temp = result;
			result = ft_strjoin(result, " ");
			free(temp);
		}
		i++;
	}
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
