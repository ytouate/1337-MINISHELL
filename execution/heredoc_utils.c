/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 20:47:37 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 15:46:53 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

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

void	open_heredoc(t_command **command)
{
	char	*line;

	if ((*command)->herdoc->first_token == NULL
		|| (*command)->herdoc->first_token->next == NULL)
		return ;
	while (*command)
	{
		if ((*command)->herdoc->first_token->next == NULL)
			return ;
		while (true)
		{
			line = readline(">");
			if (line == NULL
				|| ft_strcmp(line, (*command)->herdoc->first_token->value) == 0)
				break ;
			free(line);
		}
		(*command)->herdoc->first_token = (*command)->herdoc->first_token->next;
	}
}

bool	heredoc_outside_pipe(t_vars *vars, t_command *command)
{
	t_contex	contex;
	t_contex	temp_contex;

	open_heredoc(&command);
	unlink("/tmp/temp");
	contex.fd_in = open("/tmp/temp", O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (command->herdoc->first_token == NULL)
		return (false);
	read_for_heredoc(command, contex.fd_in);
	temp_contex = open_files(*vars->command->redi);
	if (temp_contex.fd_in == -1 || temp_contex.fd_out == -1)
	{
		set_exit_code(1);
		return (true);
	}
	contex.fd_out = dup(temp_contex.fd_out);
	if (temp_contex.fd_in != STDIN_FILENO)
		contex.fd_in = dup(temp_contex.fd_in);
	else
		contex.fd_in = dup(contex.fd_in);
	if (!check_built_in_commands(vars, command, contex))
		ft_execute(command, vars, contex);
	wait(NULL);
	unlink("/tmp/temp");
	return (true);
}
