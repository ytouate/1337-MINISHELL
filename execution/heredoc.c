/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 21:17:42 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/14 12:43:53 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	ft_heredoc(t_vars *vars, t_command *command, t_contex contex)
{
	int		temp_stdin;
	char	*line;
	int		out_file;

	open_heredoc(&command);
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
	{
		contex.fd_out = open("/tmp/temp_out_file",
				O_RDWR | O_TRUNC | O_CREAT, 0777);
	}
	else
		out_file = -1;
	if (contex.fd_in == STDIN_FILENO)
	{
		close(temp_stdin);
		temp_stdin = open("/tmp/temp", O_RDONLY);
		contex.fd_in = dup(temp_stdin);
	}
	else
		contex.fd_in = dup(contex.fd_in);
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

void	read_for_heredoc(char *line, t_command *command, int fd_in)
{
	while (true)
	{
		line = readline(">");
		if (line == NULL
			||!ft_strcmp(line, command->herdoc->first_token->value))
		{
			break ;
		}
		ft_putendl_fd(line, fd_in);
		free(line);
	}
	close(fd_in);
	fd_in = open("/tmp/temp", O_RDONLY);
}

bool	heredoc_outside_pipe(t_vars *vars, t_command *command)
{
	char		*line;
	t_contex	contex;
	t_contex	temp_contex;

	line = NULL;
	open_heredoc(&command);
	unlink("/tmp/temp");
	contex.fd_in = open("/tmp/temp", O_RDWR | O_TRUNC | O_CREAT, 0777);
	if (command->herdoc->first_token == NULL)
		return (false);
	read_for_heredoc(line, command, contex.fd_in);
	temp_contex = open_files(*vars->command->redi);
	if (temp_contex.fd_in == -1 || temp_contex.fd_out == -1)
		return (true);
	contex.fd_out = dup(temp_contex.fd_out);
	if (temp_contex.fd_in != STDIN_FILENO)
		contex.fd_in = dup(temp_contex.fd_in);
	else
		contex.fd_in = dup(contex.fd_in);
	if (!check_built_in_commands(vars, command))
		ft_execute(command, vars, contex);
	wait(NULL);
	unlink("/tmp/temp");
	return (true);
}
