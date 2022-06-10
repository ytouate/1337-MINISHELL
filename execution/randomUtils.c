/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   randomUtils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:25:22 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 20:59:50 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

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

	cmd = readline("Minishell$> :");
	if (cmd && *cmd)
		add_history(cmd);
	return (cmd);
}

void	replace_symbol_by_val(char **s, t_list *env_list)
{
	int	i;

	i = 0;
	(void)env_list;
	while (s[i])
	{
		if (ft_strcmp(s[i], "~") == 0)
		{
			free(s[i]);
			if (getenv("HOME") != NULL)
				s[i] = ft_strdup(getenv("HOME"));
		}
		i++;
	}
}

void	init_contex(t_contex *contex)
{
	contex->fd_in = STDIN_FILENO;
	contex->fd_out = STDOUT_FILENO;
}
