/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   combine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:06:57 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 14:10:10 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
extern int exit_code;
int exit_code = 0;
void set_exit_code(int num)
{
	while (num > 255)
		num -= 256;
	exit_code = num;
}

int get_exit_code()
{
	return (exit_code);
}
void	exec_node(t_vars *vars, int fd)
{
	int		i;

	i = 0;
    (void)fd;
	if (check_built_in_commands(*vars) == false)
	{
		if (vars->command->input->first_token != NULL)
		{
			if (vars->command->input->first_token->token == T_HERDOC)
            {
                ;
				// ft_herdoc(vars, fd);
            }
			else
				redirect_input(vars);
		}
		else if (vars->command->output->first_token != NULL)
		{
			if (vars->command->output->first_token->token == T_OUT)
				ft_redirect_output_trunc_mode(vars);
			else
				ft_redirect_output_append_mode(vars);
		}
		else
			ft_execute(vars);
	}
}