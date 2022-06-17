/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/17 23:06:37 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	ft_echo(t_command *command, char *s, char flag, t_contex contex)
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
	{
		if (fd != STDOUT_FILENO)
			ft_putstr_fd(s, fd);
		else
			ft_putstr_fd(s, contex.fd_out);
	}
	else
	{
		if (fd != STDOUT_FILENO)
			ft_putendl_fd(s, fd);
		else
			ft_putendl_fd(s, contex.fd_out);
	}
	free(s);
	set_exit_code(EXIT_SUCCESS);
}

int	check_echo_flag(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (false);
	if (ft_strcmp(s, "-") == 0)
		return (false);
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

char	*check_for_space(char **s, char *result, int i)
{
	char	*temp;

	if (s[i + 1] && s[i][0] != '\0')
	{
		temp = result;
		result = ft_strjoin(result, " ");
		free(temp);
	}
	return (result);
}

char	*join_for_echo(char **s, char flag)
{
	int		i;
	char	*result;
	char	*temp;

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
		result = check_for_space(s, result, i);
		i++;
	}
	return (result);
}

bool	exec_echo(t_vars vars, t_command *command, t_contex contex)
{
	(void)vars;
	if (ft_strcmp(command->flags[0], "echo") == 0)
	{
		if (command->flags[1] == NULL)
			ft_echo(command, NULL, '0', contex);
		else if ((check_echo_flag(command->flags[1])))
		{
			ft_echo(command, join_for_echo(command->flags, 'n'), 'n', contex);
		}
		else
			ft_echo(command, join_for_echo(command->flags, '\0'), '\0', contex);
		return (true);
	}
	return (false);
}
