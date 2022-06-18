/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_files.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/15 21:59:00 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 14:09:00 by ytouate          ###   ########.fr       */
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

void	check_files(t_token_head redi)
{
	int	out_file;

	while (redi.first_token != NULL)
	{
		if (redi.first_token->token == T_IN)
		{
			out_file = open(redi.first_token->value, O_RDONLY);
			if (out_file == -1)
				set_exit_code(1);
		}
		redi.first_token = redi.first_token->next;
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
				ft_exit(command->flags[1], 'e');
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
		check_files(*command->redi);
		command = command->next_command;
	}
}
