/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 13:17:21 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/13 11:40:20 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniShell.h"

void	sig_handler(int sig)
{
	// printf("%d \n", get_signal_flag());
	if (sig == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		// rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	main(int ac, char **av, char **env)
{
	(void)ac;
	(void)av;
	char *cmd;
	char *temp;

	t_vars	*vars;

	vars = malloc(sizeof(t_vars));
	vars->env = env;
	vars->env_list = get_env_list(vars->env);
	vars->export_list = get_env_list(vars->env);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sig_handler);
	while (true)
	{
		cmd = get_promt();
		if (cmd == NULL)
			exit(130);
		temp = cmd;
		cmd = ft_strtrim(cmd, " ");
		free(temp);
		if (*cmd)
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
		// system("leaks minishell");
		free(cmd);
	}
}
