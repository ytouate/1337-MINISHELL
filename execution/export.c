/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:55:08 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 12:58:56 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int check_for_redirection(t_commande *command)
{
	int	fd;

	fd = 1;
	if (command->output->first_token != NULL)
	{
		if (command->output->first_token->token == T_OUT)
			fd = open (command->output->first_token->value, O_RDWR | O_CREAT \
				| O_TRUNC, 0644);
		else
			fd = open (command->output->first_token->value, O_RDWR | O_CREAT \
				| O_APPEND, 0644);
	}
	return (fd);
}

void	ft_export(t_vars vars, char *arg)
{
	int	fd;

	fd = check_for_redirection(vars.command);
	if (arg == NULL)
	{
		sort_list(&vars.env_list);
		while (vars.env_list)
		{
			ft_putstr_fd("declare -x\t", fd);
			ft_putendl_fd(vars.env_list->content, fd);
			vars.env_list = vars.env_list->next;
		}
	}
	else
	{
		ft_lstadd_front(&vars.env_list, ft_lstnew(arg));
		sort_list(&vars.env_list);
	}
}