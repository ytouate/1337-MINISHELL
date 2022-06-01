/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:55:08 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 15:47:11 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int check_for_redirection(t_commande *command)
{
	int	fd;

	fd = STDOUT_FILENO;
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

void	ft_export(t_vars vars, t_commande *command,  char *arg)
{
	int	fd;

	fd = STDOUT_FILENO;
	fd = open_output_files(command);
	if (arg == NULL)
	{
		sort_list(&vars.env_list);
		while (vars.env_list)
		{
			ft_putstr_fd("declare -x  ", fd);
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