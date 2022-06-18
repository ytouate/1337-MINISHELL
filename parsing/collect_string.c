/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collect_string.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 08:50:31 by ilefhail          #+#    #+#             */
/*   Updated: 2022/06/15 22:15:10 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MiniShell.h"

char	*ft_help_collect_str(t_lexer *lexer, t_list *env_list, char c, int h)
{
	char	*s;

	if (c == '"' && lexer->c == '$' && \
			lexer->i < ft_strlen(lexer->content) - 1 && \
			ft_check_after_dollar(lexer) == 1 && h == 1)
		return (ft_after_dollar(lexer, env_list));
	else if (lexer->c == '\\' && lexer->i < ft_strlen(lexer->content) - 1
		&& (lexer->content[lexer->i + 1] == '\\' \
		|| lexer->content[lexer->i + 1] == c \
		|| lexer->content[lexer->i + 1] == '$'))
	{
		ft_advance(lexer);
		s = ft_strdup(&lexer->c);
		ft_advance(lexer);
		return (s);
	}
	else
	{
		s = ft_strdup(&lexer->c);
		ft_advance(lexer);
		return (s);
	}
}

char	*ft_collect_string(t_lexer *lexer, char c, t_list *env_list, int h)
{
	char	*str;
	char	*temp;
	char	*s;

	str = ft_strdup("");
	ft_advance(lexer);
	while (lexer->content[lexer->i] && lexer->c != c)
	{
		temp = str;
		s = ft_help_collect_str(lexer, env_list, c, h);
		str = ft_strjoin(str, s);
		free(s);
		free(temp);
	}
	if (lexer->c != c)
	{
		free(str);
		return (NULL);
	}
	return (str);
}
