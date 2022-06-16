/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomUtils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:25:22 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/16 11:34:21 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

int	ft_strcmp(char *s, char *str)
{
	int	i;

	i = 0;
	while (s[i] || str[i])
	{
		if (s[i] != str[i])
			return (s[i] - str[i]);
		i++;
	}
	return (0);
}

char	*get_promt(void)
{
	char	*cmd;

	cmd = readline("\033[0;34mMinishell$> :\033[0;37m");
	if (cmd && *cmd)
		add_history(cmd);
	return (cmd);
}

void	init_contex(t_contex *contex)
{
	contex->fd_in = STDIN_FILENO;
	contex->fd_out = STDOUT_FILENO;
}

void	walk_to_heredoc(t_command **command)
{
	while (*command && (*command)->herdoc->first_token == NULL)
		*command = (*command)->next_command;
}
