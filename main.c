/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:17:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/05 20:44:39 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniShell.h"
void	sig_handler(int sig)
{
	if (sig == SIGQUIT)
	{
		set_exit_code(131);
	}
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		set_exit_code(130);
	}
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char *cmd;

	t_head_c	*command;
	t_vars		*vars;

	vars = malloc(sizeof(t_vars));
	vars->env = env;
	vars->env_list = get_env_list(vars->env);
	vars->export_list = get_env_list(vars->env);
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
			command = ft_get_for_exec(cmd, vars->env_list);
			if (command != NULL)
			{
				vars->command = command->first_c;
				vars->num_of_commands = get_len(command->first_c);
				if (command != NULL)
				{
					replace_symbol_by_val(command->first_c->flags, vars->env_list);
					// exec_pipe(vars);
					ft_pipe(vars);
				}
			}
		}
	}
}