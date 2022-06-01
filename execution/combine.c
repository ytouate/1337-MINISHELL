/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:06:57 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 22:52:13 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

extern int	exit_code;

int	exit_code = 0;

void	set_exit_code(int num)
{
	while (num > 255)
		num -= 256;
	exit_code = num;
}

int	get_exit_code(void)
{
	return (exit_code);
}

void	exec_node(t_vars *vars, t_commande *command, t_contex contex)
{
	int		i;

	i = 0;
	if (check_built_in_commands(*vars, command) == false)
	{
		if (command->input->first_token != NULL)
		{
			if (command->input->first_token->token == T_HERDOC)
				ft_heredoc(vars, command, contex);
			else
				redirect_input(vars, command);
		}
		else if (command->output->first_token != NULL)
		{
			if (command->output->first_token->token == T_OUT)
				ft_redirect_output_trunc_mode(vars, command);
			else
				ft_redirect_output_append_mode(command, vars);
		}
		else
			ft_execute(command, vars, contex);
	}
}

int	check_built_in_commands(t_vars vars, t_commande *command)
{
	if (command->flags[0] != NULL)
	{
		if (run_pwd(vars, command))
			return (true);
		else if (run_env(vars, command))
			return (true);
		else if (run_exit(vars, command))
			return (true);
		else if (run_cd(vars, command))
			return (true);
		else if (run_unset(vars, command))
			return (true);
		else if (run_export(vars, command))
			return (true);
		else if (exec_echo(vars, command))
			return (true);
		return (false);
	}
	return (false);
}
