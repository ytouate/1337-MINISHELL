/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 20:55:52 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 13:09:55 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

int	get_len(t_command *command)
{
	int	count;

	count = 0;
	while (command)
	{
		count += 1;
		command = command->next_command;
	}
	return (count);
}

void	exec_node(t_vars *vars, t_command *command, t_contex contex)
{
	int		i;

	i = 0;
	if (check_built_in_commands(vars, command, contex) == false)
	{
		if (!check_redirection(vars, command))
		{
			if (command->herdoc->first_token == NULL)
			{
				ft_execute(command, vars, contex);
				wait(NULL);
			}
		}
	}
}

int	check_built_in_commands(t_vars *vars, t_command *command, t_contex contex)
{
	if (command->flags[0] != NULL)
	{
		if (run_pwd(*vars, command, contex))
			return (true);
		else if (run_env(*vars, command, contex))
			return (true);
		else if (run_exit(*vars, command))
			return (true);
		else if (run_cd(*vars, command))
			return (true);
		else if (run_unset(vars, command))
			return (true);
		else if (run_export(command, vars, contex))
			return (true);
		else if (exec_echo(*vars, command, contex))
			return (true);
		return (false);
	}
	return (false);
}

void	ft_execute(t_command *command, t_vars *vars, t_contex contex)
{
	char	*command_path;

	if (command->flags[0] == NULL)
		return ;
	command_path = get_path(vars->env_list, command->flags[0]);
	if (command->flags[0][0] == '/' || command->flags[0][0] == '.')
		check_cmd(command, vars, contex);
	else if (command_path == NULL)
		ft_error(command->flags[0], " :command not found", COMMAND_NOT_FOUND);
	else
	{
		if (command->flags[0][0])
			exec_command(command, vars, contex, command_path);
		else
			ft_error(command->flags[0], " :command not found",
				COMMAND_NOT_FOUND);
	}
	free(command_path);
	g_global_vars.pid = -1;
}

void	close_pipe(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}
