/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   after_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilefhail <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 08:53:48 by ilefhail          #+#    #+#             */
/*   Updated: 2022/06/14 08:53:49 by ilefhail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "/Users/ilefhail/Desktop/MiniShell/MiniShell.h"

int	ft_check_while(t_lexer *lexer)
{
	if (lexer->content[lexer->i] && \
		lexer->c != ' ' && lexer->c != '$' && \
		lexer->c != '\'' && lexer->c != '"' && \
		lexer->c != '?' && lexer->c != '!' && \
		(ft_isalnum(lexer->c) != 0 || \
		ft_isalpha(lexer->c) != 0 || lexer->c == '_'))
		return (1);
	else
		return (0);
}

char	*ft_get_content_of_variable(t_lexer *lexer, t_list *env_list)
{
	char	*s;
	char	*str;

	s = ft_strdup("");
	while (ft_check_while(lexer) == 1)
	{
		str = s;
		s = ft_strjoin(s, &lexer->c);
		free(str);
		ft_advance(lexer);
	}
	str = ft_get_env_val(env_list, s);
	if (str)
	{
		free(s);
		return (str);
	}
	else
	{
		free(s);
		free(str);
		str = ft_strdup("");
	}
	return (str);
}

char	*ft_after_dollar(t_lexer *lexer, t_list *env_list)
{
	char	*str;

	ft_advance(lexer);
	if (lexer->c == '?')
	{
		str = ft_itoa(get_exit_code());
		ft_advance(lexer);
	}
	else if (ft_isdigit(lexer->c) != 0 || lexer->c == '@' || lexer->c == '*')
	{
		ft_advance(lexer);
		return (ft_strdup(""));
	}
	else
		str = ft_get_content_of_variable(lexer, env_list);
	return (str);
}

int	ft_check_after_dollar(t_lexer *lexer)
{
	if (ft_isalnum(lexer->content[lexer->i + 1]) != 0 \
		|| lexer->content[lexer->i + 1] == '?' )
		return (1);
	else if (lexer->content[lexer->i + 1] == '@' || \
		lexer->content[lexer->i + 1] == '*')
		return (1);
	else if (lexer->content[lexer->i + 1] == '_')
		return (1);
	return (0);
}
