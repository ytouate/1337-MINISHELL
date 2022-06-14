/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 22:04:35 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/14 12:36:18 by ytouate          ###   ########.fr       */
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
		exit(exit_code);
	}
}

void	set_exit_code_inside_pipe(t_vars *vars, t_command *command)
{
	int		out_file;
	char	*path;

	while (command)
	{
		if (command->flags[0])
		{
			if (command->flags[0][0] == '/')
			{
				if (access(command->flags[0], F_OK | X_OK) != 0)
					set_exit_code(PERMISSION_DENIED);
				else
					set_exit_code(0);
			}
			else if (command->flags[0][0] == '.')
			{
				if (access(command->flags[0], F_OK) != 0)
					set_exit_code(PERMISSION_DENIED);
				else
					set_exit_code(0);
			}
			else
			{
				path = get_path(vars->env_list, command->flags[0]);
				if (path == NULL)
					set_exit_code(COMMAND_NOT_FOUND);
				else
					set_exit_code(0);
				free(path);
			}
		}
		while (command->redi->first_token != NULL)
		{
			if (command->redi->first_token->token == T_IN)
			{
				out_file = open(command->redi->first_token->value, O_RDONLY);
				if (out_file == -1)
					set_exit_code(1);
			}
			command->redi->first_token = command->redi->first_token->next;
		}
		command = command->next_command;
	}
}
