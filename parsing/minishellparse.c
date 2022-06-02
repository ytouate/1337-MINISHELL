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

void	ft_add_node(t_head_c *head, t_commande *commande)
{
	t_commande	*temp;

	temp = head->first_c;
	if (temp == NULL)
	{
		head->first_c = commande;
		commande->next_comande = NULL;
		return;
	}
	while (temp->next_comande)
		temp = temp->next_comande;
	temp->next_comande = commande;
	commande->next_comande = NULL;
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
	temp[e] = value;
	temp[e + 1] = NULL;
	free(av);
	return (temp);
}

int	ft_syntax(char *value, t_token *t)
{
	if (value == NULL)
	{
		free(t);
		printf("minishell:syntax error\n");
		return (1);
	}
	return (0);
}

int		ft_rederictions(t_commande *re, t_token *token)
{
	if (ft_syntax(token->value, token) == 1)
		return (1);
	else
		ft_add_red(re->redi, token);
	return (0);
}

int		ft_check_pipe(t_lexer *lexer, t_token *token, int k)
{
	free(token);
	ft_skip_spaces(lexer);
	if (lexer->content[lexer->i] == '\0' || k == 0)
	{
		printf("minishell:syntax error\n");
		return (1);
	}
	return (0);
}

int	ft_check_token(t_token *token, t_commande *re, int *i)
{
	if (token->token == 0)
	{
		if (ft_syntax(token->value, token) == 1)
			return (1);
		re->flags = ft_replace(re->flags, *i, token->value);
		*i += 1;
		free(token);
	}
	else if (token->token >= 1 && token->token <= 4)
	{
		if (ft_rederictions(re, token) == 1)
			return (1);
	}
	return (0);
}

int		ft_fill_node(t_commande *re, t_lexer *lexer, t_list *env_list)
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
			if (ft_check_token(token, re, &i) == 1)
				return (1);
		}
		else if (token->token == 5)
		{
			if (ft_check_pipe(lexer, token, k) == 1)
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
	t_commande	*re;

	re = malloc(sizeof(t_commande));
	re->redi = malloc(sizeof(t_token_head));
	re->redi->first_token = NULL;
	re->flags = malloc(sizeof(char *));
	re->flags[0] = NULL;
	if (ft_fill_node(re, lexer, env_list) == 1)
		return (1);
	ft_add_node(head, re);
	return (0);
}

t_head_c	*ft_get_for_exec(char *content, t_list *env_list)
{
	t_head_c	*head_of_commande;
	t_lexer		*lexer;
	int			s;

	head_of_commande = malloc(sizeof(t_head_c));
    ft_init_head(head_of_commande);
	lexer = ft_init_lexer(content);
	while (lexer->content[lexer->i])
	{
		s = ft_add_commande(head_of_commande, lexer, env_list);
		if (s == 1)
			return (NULL);
	}
	return (head_of_commande);
}
