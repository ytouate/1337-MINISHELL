/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:46 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 12:58:39 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_env(t_vars vars)
{
	int	fd;

	fd = STDOUT_FILENO;
	if (vars.command->output->first_token != NULL)
	{
		if (vars.command->output->first_token->token == T_OUT)
			fd = open(vars.command->output->first_token->value, O_CREAT | O_WRONLY \
				| O_TRUNC, 0644);
		else
			fd = open(vars.command->output->first_token->value, O_CREAT | O_WRONLY \
				| O_APPEND, 0644);
	}
	while (vars.env_list)
	{
		ft_putendl_fd(vars.env_list->content, fd);
		vars.env_list = vars.env_list->next;
	}
}
