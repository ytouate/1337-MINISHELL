/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:55:08 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/18 15:33:14 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	ft_export(t_command *command, t_list *env, char *arg)
{
	int	fd;

	fd = open_files(*command->redi).fd_out;
	if (fd == -1)
	{
		set_exit_code(1);
		return ;
	}
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

void	show_export_list(t_command *command, t_vars vars, t_contex contex)
{
	t_contex	ctx;

	ctx = open_files(*command->redi);
	if (ctx.fd_in == -1 || ctx.fd_out == -1)
	{
		set_exit_code(1);	
		return ;
	}
	while (vars.export_list && vars.export_list->content != NULL)
	{
		if (ctx.fd_out == STDOUT_FILENO)
		{
			ft_putstr_fd("declare -x  ", contex.fd_out);
			ft_putendl_fd(vars.export_list->content, contex.fd_out);
		}
		else
		{
			ft_putstr_fd("declare -x  ", ctx.fd_out);
			ft_putendl_fd(vars.export_list->content, ctx.fd_out);
		}
		vars.export_list = vars.export_list->next;
	}
}

void	add_unexisted_variable(t_command *command, t_vars *vars,
		char **temp, int i)
{
	ft_unset(&vars->env_list, temp[0]);
	ft_unset(&vars->export_list, temp[0]);
	ft_lstadd_front(&(vars)->env_list, ft_lstnew(ft_strdup(command->flags[i])));
	ft_lstadd_front(&(vars)->export_list,
		ft_lstnew(ft_strdup(command->flags[i])));
	sort_list(&vars->export_list);
}

void	add_existed_variable(t_command *command, t_vars *vars,
		int i, char **temp)
{
	ft_unset(&vars->env_list, temp[0]);
	ft_unset(&vars->export_list, temp[0]);
	ft_lstadd_front(&(vars)->env_list, ft_lstnew(ft_strdup(command->flags[i])));
	ft_lstadd_front(&(vars)->export_list,
		ft_lstnew(ft_strdup(command->flags[i])));
	sort_list(&vars->export_list);
}

bool	run_export(t_command *command, t_vars *vars, t_contex contex)
{
	int			i;
	int			flag;

	flag = 0;
	i = 0;
	if (ft_strcmp(command->flags[0], "export")
		&& ft_strcmp(command->flags[0], "EXPORT"))
		return (false);
	if (command->flags[1] == NULL)
		show_export_list(command, *vars, contex);
	else
	{
		while (command->flags[++i])
		{
			if (is_properly_named(command->flags[i]))
				add_properly_named_word(command, vars, i);
			else
				show_export_error(&flag, i, command);
		}
	}
	return (true);
}
