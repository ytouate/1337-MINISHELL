/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:17:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/05/31 13:17:39 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniShell.h"

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;

	t_head_c	*command;
	t_vars		vars;
	vars.env_list = get_env_list(env);
	vars.export_list = get_env_list(env);
	sort_list(&vars.export_list);
	vars.env = env;
	char *cmd;
	signal(SIGQUIT, sig_handler);
	signal(SIGINT, sig_handler);
	while (true)
	{
		cmd = get_promt();
		if (cmd == NULL)
			exit(130);
		cmd = ft_strtrim(cmd, " ");
		if (*cmd)
		{
			command = ft_get_for_exec(cmd, vars.env_list);
			vars.command = command->first_c;
			if (command != NULL)
			{
				replace_symbol_by_val(command->first_c->flags, vars.env_list);
				ft_pipe(&vars);
			}
		}
	}
}