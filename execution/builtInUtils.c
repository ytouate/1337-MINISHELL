/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtInUtils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 12:41:23 by ytouate           #+#    #+#             */
/*   Updated: 2022/06/15 22:05:53 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

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

char	*join_var(char **temp)
{
	int		i;
	char	*result;
	char	*t;

	i = 0;
	result = ft_strdup("");
	while (temp[++i])
	{
		t = result;
		result = ft_strjoin(result, temp[i]);
		free(t);
		if (temp[i + 1])
		{
			t = result;
			result = ft_strjoin(result, "=");
			free(t);
		}
	}
	free_2d_array(temp);
	return (result);
}

char	*ft_get_env_val(t_list *env_list, char *var_name)
{
	char	*temp;
	char	**l;
	int		i;
	char	*f;

	temp = ft_strdup("");
	while (env_list)
	{	
		l = ft_split(env_list->content, '=');
		f = temp;
		temp = ft_strdup(l[0]);
		free(f);
		i = 0;
		free_2d_array(l);
		if (!temp || !*temp)
			return (NULL);
		if (ft_strcmp(temp, var_name) == 0)
		{
			free(temp);
			return (join_var(ft_split(env_list->content, '=')));
		}
		env_list = env_list->next;
	}
	free(temp);
	return (NULL);
}
