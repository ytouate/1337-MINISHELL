/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytouate <ytouate@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/09 12:33:56 by ilefhail          #+#    #+#             */
/*   Updated: 2022/06/06 14:15:28 by ytouate          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

typedef enum{
	T_WORD,
	T_IN,
	T_OUT,
	T_HERDOC,
	T_APPEND,
	T_PIPE
}t_type;

typedef struct TOKEN{
	t_type token;
	char *value;
	struct TOKEN *next;
}t_token;

typedef struct t_token_head{
	t_token	*first_token;
}t_token_head;

typedef struct minishellpars{
	char					**flags;
	struct minishellpars	*next_command;
	t_token_head			*redi;
	t_token_head			*herdoc;
}t_command;

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "../MiniShell.h"

typedef struct lexer
{
	char	*content;
	char	c;
	size_t	i;
}t_lexer;
	
typedef struct head
{
	int		taille;
	t_command	*first_c;
}t_head_c;

t_token		*ft_init_token(int type, char *value);
void		ft_advance(t_lexer	*lexer);
void		ft_skip_spaces(t_lexer	*lexer);
t_lexer		*ft_init_lexer(char *content);
t_token		*ft_get_next_token(t_lexer *lexer, t_list *env_list);
char		*ft_collect_string(t_lexer *lexer, char c, t_list *env_list);
char		*ft_get_value(t_lexer *lexer, t_list *env_list);
t_head_c	*ft_get_for_exec(char *content, t_list *env_list);
void		ft_init_head(t_head_c *head);
void		ft_add_node(t_head_c *head, t_command *commande);
int			ft_add_commande(t_head_c *head, t_lexer *lexer, t_list *env_list);
char		*ft_get_str_without_quote(t_lexer *lexer, t_list *env_list);
void		ft_add_red(t_token_head *head, t_token *t);
char		**ft_replace(char **av, int i, char *value);
int			ft_syntax(char *value, t_token *t);
int			ft_rederictions(t_command *re, t_token *token);
int			ft_check_pipe(t_lexer *lexer, t_token *token, int k);
int			ft_check_token(t_token *token, t_command *re, int *i);
int			ft_fill_node(t_command *re, t_lexer *lexer, t_list *env_list);
t_token		*ft_red(t_lexer *lexer, t_list *env_list);
t_token		*ft_her_app(t_lexer *lexer, t_list *env_list);
char		*ft_str_for_join(t_lexer *lexer, t_list *env_list);
char		*ft_after_dollar(t_lexer *lexer, t_list *env_list);
int			ft_check_after_dollar(t_lexer *lexer);
char		*ft_get_str(t_lexer *lexer, t_list *env_list);
char		*ft_help_collect_str(t_lexer *lexer, t_list *env_list, char c);

#endif