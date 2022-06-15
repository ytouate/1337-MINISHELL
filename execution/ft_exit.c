/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/01 22:04:35 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/15 21:56:54 by ytouate          ###   ########.fr       */
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

void	heck_export_error(t_vars *vars, t_command *command)
{
	while (command->flags[++i])
		if (!is_properly_named(command->flags[i]))
			set_exit_code(1);
}

void	check_cd_errors(t_vars *vars, t_command *command)
{
	if (command->flags[1] == NULL
		|| ft_strcmp("~", command->flags[1]) == 0)
		if (ft_getenv(vars->env_list, "HOME") == NULL)
			set_exit_code(1);
}

void	check_command_error(t_vars *vars, t_command *command)
{
	if (access(command->flags[0], F_OK | X_OK) != 0)
		set_exit_code(PERMISSION_DENIED);
	else
		set_exit_code(0);
}

void	check_command_error_2(t_vars *vars, t_command *command)
{
	if (access(command->flags[0], F_OK | X_OK) != 0)
		set_exit_code(PERMISSION_DENIED);
	else
		set_exit_code(0);
}

char	*check_path(t_vars *vars, t_command *command)
{
	char	*path;

	path = get_path(vars->env_list, command->flags[0]);
	if (path == NULL)
		set_exit_code(COMMAND_NOT_FOUND);
	else
		set_exit_code(0);
	free(path);
}

void	check_files(t_vars *vars, t_command *command)
{
	int	out_file;

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
}

void	set_exit_code_inside_pipe(t_vars *vars, t_command *command)
{
	int		i;

	i = 0;
	while (command)
	{
		if (command->flags[0])
		{
			if (ft_strcmp(command->flags[0], "export") == 0)
				check_export_error(vars, command);
			else if (ft_strcmp(command->flags[0], "cd") == 0)
				check_cd_errors(vars, command);
			else if (command->flags[0][0] == '/')
				check_command_error(vars, command);
			else if (command->flags[0][0] == '.')
				check_command_error_2(vars, command);
			else
				check_path(vars, command);
		}
		check_files(vars, command);
		command = command->next_command;
	}
}
