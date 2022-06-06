/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtInUtils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:41:23 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/06 12:40:17 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"

t_list	*get_env_list(char **env)
{
	t_list	*env_list;
	int		i;

	i = 0;
	env_list = ft_lstnew(ft_strdup(env[i++]));
	while (env[i])
		ft_lstadd_back(&env_list, ft_lstnew(ft_strdup(env[i++])));
	return (env_list);
}

void	sort_list(t_list **env_list)
{
	t_list	*p;
	t_list	*q;
	char	*val;

	p = *env_list;
	while (p)
	{
		q = *env_list;
		while (q->next)
		{
			if (ft_strcmp(q->content, q->next->content) > 0)
			{
				val = q->content;
				q->content = q->next->content;
				q->next->content = val;
			}
			q = q->next;
		}
		p = p ->next;
	}
}

char *join_var(char **temp)
{
	int i;
	char *result;
	i = 0;
	result = "";
	while (temp[++i])
	{
		result = ft_strjoin(result, temp[i]);
		if (temp[i + 1])
			result = ft_strjoin(result, "=");
	}
	return (result);
}

char	*ft_get_env_val(t_list *env_list, char *var_name)
{
	char	*temp;

	while (env_list)
	{
		temp = ft_split(env_list->content, '=')[0];
		if (!temp || !*temp)
			return (NULL);
		if (ft_strcmp(temp, var_name) == 0)
			return (join_var(ft_split(env_list->content, '=')));
		env_list = env_list ->next;
	}
	return (NULL);
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
