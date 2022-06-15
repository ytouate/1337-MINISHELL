/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/12 10:57:34 by ilefhail          #+#    #+#             */
/*   Updated: 2022/06/15 22:05:53 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

t_lexer	*ft_init_lexer(char *content)
{
	t_lexer	*lexer;

	lexer = ft_calloc(1, sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->content = content;
	lexer->i = 0;
	lexer->c = content[0];
	return (lexer);
}

t_token	*ft_get_next_token(t_lexer *lexer, t_list *env_list)
{
	while (lexer->i < ft_strlen(lexer->content))
	{
		if (lexer->c == ' ')
			ft_skip_spaces(lexer);
		else if (lexer->c == '|')
		{
			ft_advance(lexer);
			ft_skip_spaces(lexer);
			return (ft_init_token(T_PIPE, ""));
		}
		else if (lexer->c == '"' || lexer->c == '\'')
			return (ft_init_token(T_WORD, ft_get_value(lexer, env_list)));
		else if (ft_strncmp(&lexer->content[lexer->i], "<<", 2) == 0 || \
			ft_strncmp(&lexer->content[lexer->i], ">>", 2) == 0)
			return (ft_her_app(lexer, env_list));
		else if (lexer->c == '<' || lexer->c == '>')
			return (ft_red(lexer, env_list));
		else
			return (ft_init_token(T_WORD, ft_get_value(lexer, env_list)));
	}
	return (NULL);
}

char	*ft_str_for_join(t_lexer *lexer, t_list *env_list)
{
	char	*s;

	if (lexer->c == '\'' || lexer->c == '"')
	{
		s = ft_collect_string(lexer, lexer->c, env_list);
		ft_advance(lexer);
		return (s);
	}
	else
		return (ft_get_str_without_quote(lexer, env_list));
}

char	*ft_join_and_clean(char *str, char *s)
{
	char	*temp;

	temp = str;
	str = ft_strjoin(str, s);
	free(s);
	free(temp);
	return (str);
}

char	*ft_get_value(t_lexer *lexer, t_list *env_list)
{
	char	*str;
	char	*s;

	str = ft_strdup("");
	ft_skip_spaces(lexer);
	while (lexer->content[lexer->i] && lexer->c != ' ' && \
		lexer->c != '>' && lexer->c != '<' && lexer->c != '|')
	{
		if (lexer->i < ft_strlen(lexer->content) - 1 && lexer->c == '$' && \
				(lexer->content[lexer->i + 1] == '\'' || \
			lexer->content[lexer->i + 1] == '"'))
			ft_advance(lexer);
		else
		{
			s = ft_str_for_join(lexer, env_list);
			if (s == NULL)
			{
				free(str);
				return (NULL);
			}
			str = ft_join_and_clean(str, s);
		}
	}
	return (str);
}
