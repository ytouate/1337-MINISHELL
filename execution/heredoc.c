/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 21:17:42 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 21:17:50 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_heredoc(t_vars *vars, t_command *command, t_contex contex)
{
	int		temp_stdin;
	char	*line;
	int		out_file;

	out_file = 1337;
	temp_stdin = open("/tmp/temp", O_RDWR | O_TRUNC | O_CREAT, 0777);
	while (true)
	{
		line = readline(">");
		if (line == NULL
			|| !ft_strcmp(line, command->herdoc->first_token->value))
			break ;
		ft_putendl_fd(line, temp_stdin);
	}
	contex = open_files(*command->redi);
	if (contex.fd_out == STDOUT_FILENO)
		contex.fd_out = open("/tmp/temp_out_file",
				O_RDWR | O_TRUNC | O_CREAT, 0777);
	else
		out_file = -1;
	close(temp_stdin);
	temp_stdin = open("/tmp/temp", O_RDONLY);
	contex.fd_in = dup(temp_stdin);
	contex.fd_out = dup(contex.fd_out);
	if (!check_built_in_commands(vars, command))
	{
		ft_execute(command, vars, contex);
		wait(NULL);
		close(contex.fd_out);
	}
	unlink("tmp/temp");
	if (out_file == -1)
		return (0);
	else if (out_file == 0)
		return (contex.fd_in);
	else
		return (-1);
}

bool	heredoc_outside_pipe(t_vars *vars, t_command *command)
{
	char		*line;
	t_contex	contex;

	contex.fd_in = open("/tmp/temp", O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (command->herdoc->first_token == NULL)
		return (false);
	while (true)
	{
		line = readline(">");
		if (line == NULL
			||!ft_strcmp(line, command->herdoc->first_token->value))
			break ;
		ft_putendl_fd(line, contex.fd_in);
	}
	close(contex.fd_in);
	contex.fd_in = open("/tmp/temp", O_RDONLY);
	contex.fd_out = (STDOUT_FILENO);
	contex.fd_in = dup(contex.fd_in);
	if (!check_built_in_commands(vars, command))
	{
		ft_execute(command, vars, contex);
		wait(NULL);
	}
	unlink("/tmp/temp");
	return (true);
}