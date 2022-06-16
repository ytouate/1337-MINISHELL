/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 21:59:00 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/16 11:04:54 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	check_command_error(t_vars *vars, t_command *command)
{
	(void)vars;
	if (access(command->flags[0], F_OK | X_OK) != 0)
		set_exit_code(PERMISSION_DENIED);
	else
		set_exit_code(0);
}

void	check_command_error_2(t_vars *vars, t_command *command)
{
	(void)vars;
	if (access(command->flags[0], F_OK | X_OK) != 0)
		set_exit_code(PERMISSION_DENIED);
	else
		set_exit_code(0);
}

void	check_path(t_vars *vars, t_command *command)
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

	(void)vars;
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
			if (ft_strcmp(command->flags[0], "exit") == 0)
				ft_exit(0, command->flags[1], 'p');
			else if (ft_strcmp(command->flags[0], "export") == 0)
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
