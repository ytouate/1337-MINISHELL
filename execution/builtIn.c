/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtIn.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:35:37 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 16:04:15 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_pwd(t_vars vars, t_commande *command)
{
	int fd;
	fd = STDOUT_FILENO;
	fd = open_output_files(command);
	char	working_directory[PATH_MAX];
	getcwd(working_directory, sizeof(working_directory));
	ft_setenv(&vars.env_list, "PWD", working_directory);
	ft_putendl_fd(working_directory, fd);
}

