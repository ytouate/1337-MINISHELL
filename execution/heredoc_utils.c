/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 20:47:37 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 20:48:13 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	exec_first_command_before_heredoc(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	dup2(data.fd[1], STDOUT_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	exec_last_command_before_heredoc(t_vars *vars, t_norm data)
{
	if (vars->command->redi->first_token == NULL && vars->command->next_command)
		dup2(data.fd[1], STDOUT_FILENO);
	else
		close(data.fd[1]);
	close(data.fd[0]);
	dup2(data.temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, data.contex);
}

void	exec_other_command_before_heredoc(t_vars *vars, t_norm data)
{
	close(data.fd[0]);
	dup2(data.fd[1], STDOUT_FILENO);
	dup2(data.temp_fd, STDIN_FILENO);
	exec_node(vars, vars->command, data.contex);
}
