/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:54:26 by ilefhail          #+#    #+#             */
/*   Updated: 2022/06/15 22:14:55 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

void	ft_add_red(t_token_head *head, t_token *t)
{
	t_token	*temp;

	temp = head->first_token;
	if (temp == NULL)
	{
		head->first_token = t;
		return ;
	}
	while (temp->next)
		temp = temp->next;
	temp->next = t;
}

void	ft_add_node(t_head_c *head, t_command *command)
{
	t_command	*temp;

	temp = head->first_c;
	if (temp == NULL)
	{
		head->first_c = command;
		head->taille += 1;
		command->next_command = NULL;
		return ;
	}
	while (temp->next_command)
		temp = temp->next_command;
	temp->next_command = command;
	head->taille += 1;
	command->next_command = NULL;
}

char	**ft_replace(char **av, int i, char *value)
{
	int		e;
	char	**temp;

	temp = malloc(sizeof(char *) * (i + 1));
	e = 0;
	while (e < i - 1)
	{
		temp[e] = av[e];
		e++;
	}	
	temp[e] = ft_strdup(value);
	temp[e + 1] = NULL;
	free(av);
	return (temp);
}

int	ft_syntax(char *value, t_token *t, t_head_c *head)
{
	if (value == NULL)
	{
		free(t);
		free(t->value);
		ft_free_all(head);
		printf("minishell:syntax error\n");
		set_exit_code(SYNTAX_ERROR_EXIT);
		return (1);
	}
	return (0);
}

int	ft_rederictions(t_command *re, t_token *token, t_head_c *head)
{
	if (ft_syntax(token->value, token, head) == 1)
		return (1);
	else if (token->token == T_HERDOC)
		ft_add_red(re->herdoc, token);
	else
		ft_add_red(re->redi, token);
	return (0);
}
