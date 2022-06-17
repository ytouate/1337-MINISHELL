/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:35:37 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/17 23:01:00 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	ft_pwd(t_vars vars, t_command *command, t_contex contex)
{
	t_contex		ctx;
	char			working_directory[PATH_MAX];

	ctx = open_files(*command->redi);
	if (ctx.fd_out == -1 || ctx.fd_in == -1)
	{
		set_exit_code(1);
		return ;
	}
	getcwd(working_directory, sizeof(working_directory));
	ft_setenv(&vars.env_list, "PWD", working_directory);
	ft_setenv(&vars.export_list, "PWD", working_directory);
	if (ctx.fd_out == STDOUT_FILENO)
		ft_putendl_fd(working_directory, contex.fd_out);
	else
		ft_putendl_fd(working_directory, ctx.fd_out);
	set_exit_code(EXIT_SUCCESS);
}

bool	run_pwd(t_vars vars, t_command *command, t_contex contex)
{
	if (!ft_strcmp(command->flags[0], "pwd"))
	{
		ft_pwd(vars, command, contex);
		return (true);
	}
	return (false);
}
