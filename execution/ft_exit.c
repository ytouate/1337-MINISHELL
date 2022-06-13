/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 22:04:35 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 20:39:04 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

bool	is_number(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	run_exit(t_vars vars, t_command *command)
{
	if (!ft_strcmp(command->flags[0], "exit"))
	{
		if (vars.num_of_commands == 1)
			ft_exit(0, command->flags[1], '\0');
		else
			ft_exit(0, command->flags[1], 'p');
		return (true);
	}
	return (false);
}

void	ft_exit(int exit_code, char *arg, char flag)
{
	int	n;

	if (arg != NULL)
	{
		if (!is_number(arg))
		{
			printf("exit %s: numeric argument required\n", arg);
			exit_code = 255;
		}
		else
		{
			n = ft_atoi(arg);
			while (n > 256)
				n -= 256;
			exit_code = n;
		}
	}
	if (flag != 'p')
	{
		printf("exit\n");
		set_exit_code(exit_code);
		system("leaks minishell");
		exit(exit_code);
	}
}
