/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 12:09:52 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 13:09:59 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../MiniShell.h"

void	set_exit_code(int num)
{
	if (num == SYNTAX_ERROR_EXIT)
		g_global_vars.exit_code = num;
	else
	{
		while (num > 255)
			num -= 256;
		g_global_vars.exit_code = num;
	}
}

int	get_exit_code(void)
{
	return (g_global_vars.exit_code);
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
