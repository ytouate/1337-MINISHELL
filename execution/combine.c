/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 12:09:52 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/05 20:48:52 by ytouate          ###   ########.fr       */
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
	if (check_built_in_commands(vars, command) == false)
	{
		if (command->redi->first_token != NULL)
		{
			if (command->redi->first_token->token == T_HERDOC)
				ft_heredoc(vars, command, contex);
			else if (command->redi->first_token->token == T_OUT)
				ft_redirect_output_trunc_mode(vars, command);
			else if (command->redi->first_token->token == T_APPEND)
				ft_redirect_output_append_mode(command, vars);
			else if (command->redi->first_token->token == T_IN)
				redirect_input(vars, command);
		}
		else
			ft_execute(command, vars, contex);
	}
}

int	check_built_in_commands(t_vars *vars, t_commande *command)
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
