/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:35:37 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/02 16:14:01 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_pwd(t_vars vars, t_commande *command)
{
	int		fd;
	char	working_directory[PATH_MAX];

	fd = open_files(command).fd_in;
	if (fd == -1)
		return ;
	getcwd(working_directory, sizeof(working_directory));
	ft_setenv(&vars.env_list, "PWD", working_directory);
	ft_putendl_fd(working_directory, fd);
}

bool	run_pwd(t_vars vars, t_commande *command)
{
	if (!ft_strcmp(command->flags[0], "pwd"))
	{
		ft_pwd(vars, command);
		return (true);
	}
	return (false);
}
