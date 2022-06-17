/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:35:37 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/17 17:05:21 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	ft_pwd(t_vars vars, t_command *command)
{
	t_contex		contex;
	char			working_directory[PATH_MAX];

	contex = open_files(*command->redi);
	if (contex.fd_out == -1 || contex.fd_in == -1)
	{
		set_exit_code(1);
		return ;
	}
	getcwd(working_directory, sizeof(working_directory));
	ft_setenv(&vars.env_list, "PWD", working_directory);
	ft_setenv(&vars.export_list, "PWD", working_directory);
	ft_putendl_fd(working_directory, contex.fd_out);
	set_exit_code(EXIT_SUCCESS);
}

bool	run_pwd(t_vars vars, t_command *command)
{
	if (!ft_strcmp(command->flags[0], "pwd"))
	{
		ft_pwd(vars, command);
		return (true);
	}
	return (false);
}
