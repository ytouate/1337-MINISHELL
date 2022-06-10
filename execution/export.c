/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:55:08 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 10:02:53 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_export(t_command *command, t_list *env, char *arg)
{
	int fd;
	fd = open_files(*command->redi).fd_out;
	if (fd == -1)
		return ;
	if (arg == NULL)
	{
		sort_list(&env);
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
		sort_list(&env);
	}
}

void show_export_list(t_command *command, t_vars vars)
{
	t_contex contex;
	contex = open_files(*command->redi);
	if (contex.fd_in == -1 || contex.fd_out == -1)
		return ;
	while (vars.export_list && vars.export_list->content != NULL)
	{
		ft_putstr_fd("declare -x  ", contex.fd_out);
		ft_putendl_fd(vars.export_list->content, contex.fd_out);
		vars.export_list = vars.export_list->next;
	}
}

void	add_unexisted_variable(t_command *command, t_vars *vars, char **temp, int i)
{
	ft_unset(&vars->env_list, temp[0]);
	ft_unset(&vars->export_list, temp[0]);
	ft_lstadd_front(&(vars)->env_list, ft_lstnew(ft_strdup(command->flags[i])));
	ft_lstadd_front(&(vars)->export_list, ft_lstnew(ft_strdup(command->flags[i])));
	sort_list(&vars->export_list);
}

void	add_existed_variable(t_command *command, t_vars *vars, int i)
{
	ft_lstadd_front(&(vars)->env_list, ft_lstnew(ft_strdup(command->flags[i])));
	ft_lstadd_front(&(vars)->export_list, ft_lstnew(ft_strdup(command->flags[i])));
	sort_list(&vars->export_list);
}

bool add_variable(t_command *command, t_vars *vars, char **temp, int i)
{
	if (is_variable(command->flags[i]))
	{
		if (ft_getenv(vars->env_list, temp[0]) == NULL)
		{
			if (ft_getenv(vars->export_list, temp[0]) != NULL)
			{
				ft_unset(&vars->export_list, temp[0]);
				add_unexisted_variable(command, vars, temp, i);
			}
			else
				add_unexisted_variable(command, vars, temp, i);
		}
		else
			add_existed_variable(command, vars, i);
		return (true);
	}
	return (false);
}

void	add_non_variable(t_command *command, t_vars *vars, char **temp, int i)
{
	if (ft_getenv(vars->export_list, temp[0]) == NULL)
	{
		ft_lstadd_front(&(vars)->export_list, ft_lstnew(ft_strdup(command->flags[i])));
		sort_list(&vars->export_list);
	}
}

bool run_export(t_command *command, t_vars *vars)
{
	char	**temp;
	int		i;
	int flag = 0;
	i = 0;
	if (!ft_strcmp(command->flags[0], "export") || \
		!ft_strcmp(command->flags[0], "EXPORT"))
	{
		if (command->flags[1] == NULL)
			show_export_list(command, *vars);
		else
		{
			while (command->flags[++i])
			{
				if (is_properly_named(command->flags[i]))
				{
					temp = ft_split(command->flags[i], '=');
					if (!add_variable(command, vars, temp, i))
						add_non_variable(command, vars, temp, i);
				}
				else
				{
					if (flag == 0)
					{
						ft_putstr_fd(command->flags[i], STDERR_FILENO);
						ft_putendl_fd("export: not an identifier:", STDERR_FILENO);
						flag = 1;
					}
					set_exit_code(1);
				}
			}
		}
		return (true);
	}
	return (false);
}