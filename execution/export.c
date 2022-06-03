/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:55:08 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/03 09:17:55 by ytouate          ###   ########.fr       */
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
	int i = 0;
	if (!ft_strcmp(command->flags[0], "export") || \
		!ft_strcmp(command->flags[0], "EXPORT"))
	{
		if (command->flags[1] == NULL)
			show_export_list(command, *vars);
		else
		{
			while (command->flags[++i])
			{	
				temp = ft_split(command->flags[i], '=');
				if (is_variable(command->flags[i]))
				{
					if (ft_getenv(vars->env_list, temp[0]) != NULL)
					{
						ft_unset(&vars->env_list, temp[0]);
						ft_unset(&vars->export_list, temp[0]);
						ft_lstadd_front(&(vars)->env_list, ft_lstnew(ft_strdup(command->flags[i])));
						ft_lstadd_front(&(vars)->export_list, ft_lstnew(ft_strdup(command->flags[i])));
						sort_list(&vars->export_list);
					}
					else
					{
						ft_lstadd_front(&(vars)->env_list, ft_lstnew(ft_strdup(command->flags[i])));
						ft_lstadd_front(&(vars)->export_list, ft_lstnew(ft_strdup(command->flags[i])));
						sort_list(&vars->export_list);
					}
				}
				else
				{
					if (ft_getenv(vars->export_list, temp[0]) == NULL)
					{
						ft_lstadd_front(&(vars)->export_list, ft_lstnew(ft_strdup(command->flags[i])));
						sort_list(&vars->export_list);
					}
				}
			}
		}
		return (true);
	}
	return (false);
}