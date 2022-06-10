/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:46 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/10 21:02:12 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_env(t_vars vars, t_command *command)
{
	int	fd;

	fd = open_files(*command->redi).fd_out;
	if (fd == -1)
		return ;
	while (vars.env_list)
	{
		ft_putendl_fd(vars.env_list->content, fd);
		vars.env_list = vars.env_list->next;
	}
}

bool	run_env(t_vars vars, t_command *command)
{
	if (!ft_strcmp(command->flags[0], "env"))
	{
		ft_env(vars, command);
		return (true);
	}
	return (false);
}

int	is_properly_named(char *s)
{
	return (ft_isalpha(s[0]) || s[0] == '_');
}

t_list	*ft_getenv(t_list *env_list, char *var_name)
{
	char	*temp;

	while (env_list)
	{
		temp = ft_split(env_list->content, '=')[0];
		if (!temp || !*temp)
			return (NULL);
		if (ft_strcmp(temp, var_name) == 0)
			return (env_list);
		env_list = env_list ->next;
	}
	return (NULL);
}

void	ft_setenv(t_list **env_list, char *var_name, char *var_val)
{
	char	*var;
	t_list	*temp;

	temp = ft_getenv(*env_list, var_name);
	if (temp == NULL)
	{
		var = ft_strjoin(var_name, "=");
		var = ft_strjoin(var, var_val);
		ft_lstadd_back(env_list, ft_lstnew(var));
	}
	else
	{
		ft_unset(env_list, var_name);
		ft_setenv(env_list, var_name, var_val);
	}
}
