/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 12:09:52 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/15 12:44:44 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./../MiniShell.h"

extern g_vars global_vars;
void	set_exit_code(int num)
{
	if (num == SYNTAX_ERROR_EXIT)
		global_vars.exit_code = num;
	else
	{
		while (num > 255)
			num -= 256;
		global_vars.exit_code = num;
	}
}

int	get_exit_code(void)
{
	return (global_vars.exit_code);
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
