/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellparse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:52:46 by ilefhail          #+#    #+#             */
/*   Updated: 2022/05/29 16:08:57 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	ft_free_all(t_head_c *head)
{
	int			i;
	t_token		*temp;
	t_command	*t;

	
	while (head->first_c)
	{
		i = 0;
		while (head->first_c->flags[i])
		{
			free(head->first_c->flags[i]);
			i++;
		}
		free(head->first_c->flags);
		while (head->first_c->herdoc->first_token)
		{
			temp = head->first_c->herdoc->first_token;
			head->first_c->herdoc->first_token = head->first_c->herdoc->first_token->next;
			free(temp->value);
			free(temp);
		}
		free(head->first_c->herdoc);
		while (head->first_c->redi->first_token)
		{
			temp = head->first_c->redi->first_token;
			head->first_c->redi->first_token = head->first_c->redi->first_token->next;
			free(temp->value);
			free(temp);
		}
		free(head->first_c->redi);
		t = head->first_c;
		head->first_c = head->first_c->next_command;
		free(t);
	}
	free(head);
}
void	ft_init_head(t_head_c *head)
{
	head->first_c = NULL;
	head->taille = 0;
}

void	ft_add_red(t_token_head *head, t_token *t)
{
	t_token	*temp;

	temp = head->first_token;
	if (temp == NULL)
	{
		head->first_token = t;
		return;
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
		return;
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

int		ft_rederictions(t_command *re, t_token *token, t_head_c *head)
{
	if (ft_syntax(token->value, token, head) == 1)
		return (1);
	else if (token->token == T_HERDOC)
		ft_add_red(re->herdoc, token);
	else
		ft_add_red(re->redi, token);
	return (0);
}

int		ft_check_pipe(t_lexer *lexer, t_token *token, int k, t_head_c *head)
{
	free(token);
	ft_skip_spaces(lexer);
	if (lexer->content[lexer->i] == '\0' || k == 0)
	{
		ft_free_all(head);
		printf("minishell:syntax error\n");
		return (1);
	}
	return (0);
}

int	ft_check_token(t_token *token, t_command *re, int *i, t_head_c *head)
{
	
	if (token->token == 0)
	{
		if (ft_syntax(token->value, token, head) == 1)
			return (1);	
		re->flags = ft_replace(re->flags, *i, token->value);
		*i += 1;
		free(token->value);
		free(token);
	}
	else if (token->token >= 1 && token->token <= 4)
	{
		if (ft_rederictions(re, token, head) == 1)
			return (1);
	}
	return (0);
}

int		ft_fill_node(t_command *re, t_lexer *lexer, t_list *env_list, t_head_c *head)
{
	int			k;
	int			i;
	t_token		*token;

	token = ft_get_next_token(lexer, env_list);
	k = 0;
	i = 1;
	while (token)
	{
		if (token->token < 5)
		{
			if (ft_check_token(token, re, &i, head) == 1)
			{	
				
				return (1);
			}
		}
		else if (token->token == 5)
		{
			if (ft_check_pipe(lexer, token, k, head) == 1)
				return (1);
			break ;
		}
		k++;
		token = ft_get_next_token(lexer, env_list);	
	}
	return (0);
}

int	ft_add_commande(t_head_c *head, t_lexer *lexer, t_list *env_list)
{
	t_command	*re;
	int			i;

	re = malloc(sizeof(t_command));
	re->redi = malloc(sizeof(t_token_head));
	re->herdoc = malloc(sizeof(t_token_head));
	re->herdoc->first_token = NULL;
	re->redi->first_token = NULL;
	re->flags = malloc(sizeof(char *));
	re->flags[0] = NULL;
	if (ft_fill_node(re, lexer, env_list, head) == 1)
	{
		free(re->redi);
		free(re->herdoc);
		i = -1;
		while (re->flags[++i])
			free(re->flags[i]);
		free(re->flags);
		free(re);
		return (1);
	}
	ft_add_node(head, re);
	return (0);
}

t_head_c	*ft_get_for_exec(char *content, t_list *env_list)
{
	t_head_c	*head_of_command;
	t_lexer		*lexer;
	int			s;

	head_of_command = malloc(sizeof(t_head_c));
    ft_init_head(head_of_command);
	lexer = ft_init_lexer(content);
	while (lexer->content[lexer->i])
	{
		s = ft_add_commande(head_of_command, lexer, env_list);
		if (s == 1)
		{
			free(lexer);
			return (NULL);
		}
	}
	free(lexer);
	return (head_of_command);
}
