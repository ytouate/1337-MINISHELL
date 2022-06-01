/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:46 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 22:22:36 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_env(t_vars vars, t_commande *command)
{
	int	fd;

	fd = check_for_redirection(command);
	while (vars.env_list)
	{
		ft_putendl_fd(vars.env_list->content, fd);
		vars.env_list = vars.env_list->next;
	}
}

bool	run_env(t_vars vars, t_commande *command)
{
	if (!ft_strcmp(command->flags[0], "env"))
	{
		ft_env(vars, command);
		return (true);
	}
	return (false);
}
