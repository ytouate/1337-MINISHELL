/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:46 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 17:11:05 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_env(t_vars vars, t_commande *command)
{
	int	fd;

	fd = STDOUT_FILENO;
	if (command->output != NULL)
		fd = open_output_files(command);
	while (vars.env_list)
	{
		ft_putendl_fd(vars.env_list->content, fd);
		vars.env_list = vars.env_list->next;
	}
}
