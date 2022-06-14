/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:17:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/13 20:53:16 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniShell.h"

void	sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		if (get_signal_flag() != 1)
			rl_redisplay();
		else
			set_exit_code(130);
		set_signal_flag(0);
	}
	if (sig == SIGQUIT)
	{
		if (get_signal_flag() == 1)
		{
			ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
			set_exit_code(131);
			set_signal_flag(0);
		}
		else
		{
			rl_on_new_line();
			rl_redisplay();
			set_signal_flag(0);
		}
	}
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char *cmd;

	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	vars->env = env;
	vars->env_list = get_env_list(vars->env);
	vars->export_list = get_env_list(vars->env);
	signal(SIGINT, sig_handler);
	signal(SIGQUIT, sig_handler);
	while (true)
	{
		cmd = get_promt();
		if (cmd == NULL)
		{
			free(cmd);
			exit(EXIT_SUCCESS); 
		}
		else if (*cmd)
		{
			vars->head = ft_get_for_exec(cmd, vars->env_list);
			if (vars->head != NULL)
			{
				vars->command = vars->head->first_c;
				vars->num_of_commands = get_len(vars->command);
				if (vars->command != NULL)
					ft_pipe(vars);
				ft_free_all(vars->head);
			}
		}
		free(cmd);
	}
}
