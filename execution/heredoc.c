/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/10 21:17:42 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 15:45:58 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

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
		{
			if (line)
				free(line);
			break ;
		}
		ft_putendl_fd(line, temp_stdin);
		free(line);
	}
	return (temp_stdin);
}

void	check_out_files(int *out_file, int *fd_out)
{
	if (*fd_out == STDOUT_FILENO)
	{
		unlink("/tmp/temp_out_file");
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

int	heredoc_return(int outfile, t_contex contex)
{
	if (outfile == -1)
		return (0);
	else if (outfile == 0)
		return (contex.fd_in);
	else
		return (-1);
}

void	read_for_heredoc(t_command *command, int fd_in)
{
	char	*line;

	while (true)
	{
		line = readline(">");
		if (line == NULL
			||!ft_strcmp(line, command->herdoc->first_token->value))
		{
			if (line)
				free(line);
			break ;
		}
		ft_putendl_fd(line, fd_in);
		free(line);
	}
	close(fd_in);
	fd_in = open("/tmp/temp", O_RDONLY);
}
