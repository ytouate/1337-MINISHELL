/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/01 19:02:41 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void ft_redirect_output_append_mode(t_commande *command, t_vars *vars)
{
	t_contex contex;
	contex.fd_in = open_input_files(command);
	contex.fd_out = open_output_files(command);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return ;
	if (command->flags[0] != NULL)
        ft_execute(command, vars, contex);
}

void ft_redirect_output_trunc_mode(t_vars *vars, t_commande *command)
{
	t_contex contex;
	contex.fd_out = open_output_files(command);
	contex.fd_in = open_input_files(command);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return ;
	if (command->flags[0] != NULL)
        ft_execute(command, vars, contex);
}

void redirect_input(t_vars *vars, t_commande *command)
{
	t_contex contex;
	contex.fd_in = STDIN_FILENO;
	contex.fd_out = STDOUT_FILENO;
	contex.fd_in = open_input_files(command);
	if (contex.fd_in == -1)
		return ;
	contex.fd_out = open_output_files(command);
	if (contex.fd_out == -1)
		return ;
	else if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

void ft_heredoc(t_vars *vars, t_commande *command, t_contex contex)
{
	// TODO
	(void)command;
	(void)contex;
	(void)vars;
	return ;
}