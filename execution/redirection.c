/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/04 15:12:03 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_redirect_output_append_mode(t_commande *command, t_vars *vars)
{
	t_contex	contex;
	contex.herdoc_fildes = -1;
	contex = open_files(command);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return ;
	if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

void	ft_redirect_output_trunc_mode(t_vars *vars, t_commande *command)
{
	t_contex	contex;
	contex.herdoc_fildes = -1;
	contex = open_files(command);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return ;
	if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

void	redirect_input(t_vars *vars, t_commande *command)
{
	t_contex	contex;
	contex.herdoc_fildes = -1;
	contex = open_files(command);
	if (contex.fd_out == -1 || contex.fd_in == -1)
		return ;
	else if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

void	exec_herdoc_command(t_commande *command, t_vars *vars, t_contex contex)
{
	char *path;
	if (command->flags[0] != NULL)
	{
		path = get_path(vars->env_list, command->flags[0]);
		if (path)
		{
			if (fork() == 0)
			{
				dup2(contex.herdoc_fildes, STDIN_FILENO);
				dup2(contex.fd_out, STDOUT_FILENO);
				execve(path, command->flags, vars->env);
				perror("execve");
				exit(127);
			}
		}
		else
		{
			set_exit_code(127);
			printf("%s: command, not found", command->flags[0]);
		}
	}
}
void	ft_heredoc(t_vars *vars, t_commande *command, t_contex contex)
{
	int heredoc[2];
	char *line;
	pipe(heredoc);
	(void)vars;
	while (true)
	{
		line = readline(">");
		if (line == NULL || !ft_strcmp(line, command->redi->first_token->value))
			break;
		ft_putendl_fd(line, heredoc[1]);
	}
	contex.fd_in = dup(heredoc[0]);
}