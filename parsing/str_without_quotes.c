/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_without_quotes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilefhail <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 08:52:34 by ilefhail          #+#    #+#             */
/*   Updated: 2022/06/14 08:52:35 by ilefhail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "/Users/ilefhail/Desktop/MiniShell/MiniShell.h"

char	*get_str_utils(t_lexer *lexer)
{
	char	*str;

	ft_advance(lexer);
	str = ft_strdup(&lexer->c);
	ft_advance(lexer);
	return (str);
}

char	*ft_get_str(t_lexer *lexer, t_list *env_list)
{
	char	*str;
	char	*s;

	if (lexer->c == '$' && lexer->i < ft_strlen(lexer->content) - 1 \
		&& ft_check_after_dollar(lexer) == 1)
		str = ft_after_dollar(lexer, env_list);
	else if (lexer->c == '~')
	{
		s = getenv("HOME");
		if (s == NULL)
			str = ft_strdup(&lexer->c);
		else
			str = ft_strdup(s);
		ft_advance(lexer);
	}
	else if (lexer->c == '\\' && lexer->i < ft_strlen(lexer->content) - 1)
		str = get_str_utils(lexer);
	else if (lexer->c == '\\' && lexer->i == ft_strlen(lexer->content) - 1)
		return (NULL);
	else
	{
		str = ft_strdup(&lexer->c);
		ft_advance(lexer);
	}
	return (str);
}

char	*ft_witout_quotes_util(char *str, char *s)
{
	free(s);
	free(str);
	return (NULL);
}

char	*ft_get_str_without_quote(t_lexer *lexer, t_list *env_list)
{
	char	*str;
	char	*s;

	str = ft_strdup("");
	while (lexer->content[lexer->i] && lexer->c != ' ' && \
		lexer->c != '\'' && lexer->c != '"' && lexer->c != '>' && \
		lexer->c != '<' && lexer->c != '|')
	{
		if (lexer->c == '&')
			return (ft_witout_quotes_util(str, s));
		s = ft_get_str(lexer, env_list);
		if (s == NULL)
		{
			free(str);
			return (NULL);
		}
		str = ft_join_and_clean(str, s);
	}
	return (str);
}
