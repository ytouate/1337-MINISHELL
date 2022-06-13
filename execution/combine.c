/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 12:09:52 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/13 11:37:15 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	g_exit_code = 0;
int g_signal_flag = 0;

void	set_signal_flag(int num)
{
	g_signal_flag = num;
}

int		get_signal_flag(void)
{
	return (g_signal_flag);
}
void	set_exit_code(int num)
{
	if (num == SYNTAX_ERROR_EXIT)
		g_exit_code = num;
	else
	{
		while (num > 255)
			num -= 256;
		g_exit_code = num;
	}
}

int	get_exit_code(void)
{
	return (g_exit_code);
}

bool	check_redirection(t_vars *vars, t_command *command)
{
	if (command->redi->first_token != NULL)
	{
		if (command->redi->first_token->token == T_OUT)
			ft_redirect_output_trunc_mode(vars, command);
		else if (command->redi->first_token->token == T_APPEND)
			ft_redirect_output_append_mode(command, vars);
		else if (command->redi->first_token->token == T_IN)
			redirect_input(vars, command);
		return (true);
	}
	else
		return (false);
}

void	exec_node(t_vars *vars, t_command *command, t_contex contex)
{
	int		i;

	i = 0;
	if (check_built_in_commands(vars, command) == false)
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

int	check_built_in_commands(t_vars *vars, t_command *command)
{
	if (command->flags[0] != NULL)
	{
		if (run_pwd(*vars, command))
			return (true);
		else if (run_env(*vars, command))
			return (true);
		else if (run_exit(*vars, command))
			return (true);
		else if (run_cd(*vars, command))
			return (true);
		else if (run_unset(*vars, command))
			return (true);
		else if (run_export(command, vars))
			return (true);
		else if (exec_echo(*vars, command))
			return (true);
		return (false);
	}
	return (false);
}
