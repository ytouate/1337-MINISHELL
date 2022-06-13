/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:54:46 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/12 23:20:32 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

void	ft_env(t_vars vars, t_command *command)
{
	int	fd;

	printf("am her \n");
	fd = open_files(*command->redi).fd_out;
	if (fd == -1)
		return ;
	while (vars.env_list)
	{
		if (vars.env_list->content)
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
	char	**l;
	int		i;

	while (env_list)
	{
		l = ft_split(env_list->content, '=');
		temp = ft_strdup(l[0]);
		i = 0;
		while (l[i])
		{
			free(l[i]);
			i++;
		}
		free(l);
		if (!temp || !*temp)
			return (NULL);
		if (ft_strcmp(temp, var_name) == 0)
		{
			free(temp);
			return (env_list);
		}
		env_list = env_list ->next;
		free(temp);
	}
	return (NULL);
}

void	ft_setenv(t_list **env_list, char *var_name, char *var_val)
{
	char	*var;
	t_list	*temp;
	char	*t;

	temp = ft_getenv(*env_list, var_name);
	if (temp == NULL)
	{
		var = ft_strjoin(var_name, "=");
		t = var;
		var = ft_strjoin(var, var_val);
		free(t);
		ft_lstadd_back(env_list, ft_lstnew(var));
	}
	else
	{
		ft_unset(env_list, var_name);
		ft_setenv(env_list, var_name, var_val);
	}

}
