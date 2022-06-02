/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:55:08 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/02 21:02:55 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_export(t_commande *command, t_list *env, char *arg)
{
	int fd;
	fd = open_files(command).fd_out;
	if (fd == -1)
		return ;
	if (arg == NULL)
	{
		// sort_list(&env);
		while (env)
		{
			ft_putstr_fd("declare -x\t", fd);
			ft_putendl_fd(env->content, fd);
			env = env->next;
		}
	}
	else
	{
		ft_lstadd_front(&env, ft_lstnew(arg));
		// sort_list(&env);
	}
}

void show_export_list(t_commande *command, t_vars vars)
{
	int fd;
	fd = open_files(command).fd_out;
	
	while (vars.export_list)
	{
		ft_putstr_fd("declare -x  ", fd);
		ft_putendl_fd(vars.export_list->content, fd);
		vars.export_list = vars.export_list->next;
	}
}
bool run_export(t_commande *command, t_vars *vars)
{
	char **temp;
	// int i = 0;
	if (!ft_strcmp(command->flags[0], "export") || \
		!ft_strcmp(command->flags[0], "EXPORT"))
	{
		if (command->flags[1] == NULL)
			show_export_list(command, *vars);
		else
		{
			temp = ft_split(command->flags[1], '=');
			if (is_variable(command->flags[1]))
			{
				ft_lstadd_front(&(vars)->env_list, ft_lstnew(ft_strdup(command->flags[1])));
				ft_lstadd_front(&(vars)->export_list, ft_lstnew(ft_strdup(command->flags[1])));
			}
		}
		return (true);
	}
	return (false);
}