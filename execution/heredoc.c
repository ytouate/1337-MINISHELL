/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 21:17:42 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/14 15:57:54 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

int	fill_temp_stdin(t_command *command)
{
	char	*line;
	int		temp_stdin;

	open_heredoc(&command);
	temp_stdin = open("/tmp/temp", O_RDWR | O_TRUNC | O_CREAT, 0777);
	while (true)
	{
		line = readline(">");
		if (line == NULL
			|| !ft_strcmp(line, command->herdoc->first_token->value))
			break ;
		ft_putendl_fd(line, temp_stdin);
	}
	return (temp_stdin);
}

void	check_out_files(int *out_file, int *fd_out)
{
	if (*fd_out == STDOUT_FILENO)
	{
		*fd_out = open("/tmp/temp_out_file",
				O_RDWR | O_TRUNC | O_CREAT, 0777);
	}
	else
		*out_file = -1;
}

void	check_in_files(int *fd_in, int *temp_stdin)
{
	if (*fd_in == STDIN_FILENO)
	{
		close(*temp_stdin);
		*temp_stdin = open("/tmp/temp", O_RDONLY);
		*fd_in = dup(*temp_stdin);
	}
	else
		*fd_in = dup(*fd_in);
}

int	ft_heredoc(t_vars *vars, t_command *command, t_contex contex)
{
	int		temp_stdin;
	int		out_file;

	out_file = 1337;
	temp_stdin = fill_temp_stdin(command);
	contex = open_files(*command->redi);
	check_out_files(&out_file, &contex.fd_out);
	check_in_files(&contex.fd_in, &temp_stdin);
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
