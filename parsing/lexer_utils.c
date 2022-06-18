/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 11:10:06 by ilefhail          #+#    #+#             */
/*   Updated: 2022/06/16 12:16:59 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

t_token	*ft_init_token(int type, char *value)
{
	t_token	*token;

	token = ft_calloc(1, sizeof(t_token));
	token->next = NULL;
	token->token = type;
	token->value = value;
	return (token);
}

void	ft_advance(t_lexer	*lexer)
{
	if (lexer->i < ft_strlen(lexer->content))
	{
		lexer->i += 1;
		lexer->c = lexer->content[lexer->i];
	}
}

void	ft_skip_spaces(t_lexer	*lexer)
{
	while (lexer->content[lexer->i] == ' ' && lexer->content[lexer->i] != '\0')
		ft_advance(lexer);
}

t_token	*ft_red(t_lexer *lexer, t_list *env_list)
{
	if (lexer->c == '<')
	{
		ft_advance(lexer);
		ft_skip_spaces(lexer);
		if (lexer->content[lexer->i] == '\0' || lexer->c == '<' || \
			lexer->c == '>')
			return (ft_init_token(T_IN, NULL));
		return (ft_init_token(T_IN, ft_get_value(lexer, env_list, 1)));
	}
	else
	{
		ft_advance(lexer);
		ft_skip_spaces(lexer);
		if (lexer->content[lexer->i] == '\0' || \
			lexer->c == '<' || lexer->c == '>')
			return (ft_init_token(T_OUT, NULL));
		return (ft_init_token(T_OUT, ft_get_value(lexer, env_list, 1)));
	}
}

t_token	*ft_her_app(t_lexer *lexer, t_list *env_list)
{
	if (ft_strncmp(&lexer->content[lexer->i], "<<", 2) == 0)
	{
		ft_advance(lexer);
		ft_advance(lexer);
		ft_skip_spaces(lexer);
		if (lexer->content[lexer->i] == '\0' || \
			lexer->c == '<' || lexer->c == '>'
			|| lexer->c == '|')
			return (ft_init_token(T_HERDOC, NULL));
		return (ft_init_token(T_HERDOC, ft_get_value(lexer, env_list, 0)));
	}
	else
	{
		ft_advance(lexer);
		ft_advance(lexer);
		ft_skip_spaces(lexer);
		if (lexer->content[lexer->i] == '\0' || \
			lexer->c == '<' || lexer->c == '>'
			|| lexer->c == '|')
			return (ft_init_token(T_APPEND, NULL));
		return (ft_init_token(T_APPEND, ft_get_value(lexer, env_list, 1)));
	}
}
