/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:50:01 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/03 10:20:40 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_redirect_output_append_mode(t_commande *command, t_vars *vars)
{
	t_contex	contex;

	contex = open_files(command);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return ;
	if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

void	ft_redirect_output_trunc_mode(t_vars *vars, t_commande *command)
{
	t_contex	contex;

	contex = open_files(command);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return ;
	if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

void	redirect_input(t_vars *vars, t_commande *command)
{
	t_contex	contex;

	contex = open_files(command);
	if (contex.fd_out == -1 || contex.fd_in == -1)
		return ;
	else if (command->flags[0] != NULL)
		ft_execute(command, vars, contex);
}

// TODO
int	ft_heredoc(t_vars *vars, t_commande *command, t_contex contex)
{
	(void)vars;
	(void)command;
	(void)contex;
	char *line;
	int temp_filedes = open("/tmp/temp", O_RDWR | O_CREAT | O_TRUNC, 0644);
	if (temp_filedes == -1)
		perror(NULL);
	else
	{
		while (true)
		{
			line = readline(">");
			if (line == NULL)
				break ;
			ft_putendl_fd(line, temp_filedes);
		}
	}
	return (temp_filedes);
}
