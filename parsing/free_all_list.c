/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ilefhail <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/14 10:37:38 by ilefhail          #+#    #+#             */
/*   Updated: 2022/06/14 10:37:40 by ilefhail         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parsing.h"

void	ft_init_head(t_head_c *head)
{
	head->first_c = NULL;
	head->taille = 0;
}

void	free_list_of_tokens(t_token_head *head)
{
	t_token	*temp;

	while (head->first_token)
	{
		temp = head->first_token;
		head->first_token = head->first_token->next;
		free(temp->value);
		free(temp);
	}
	free(head);
}

void	ft_free_all(t_head_c *head)
{
	int			i;
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
		free_list_of_tokens(head->first_c->herdoc);
		free_list_of_tokens(head->first_c->redi);
		t = head->first_c;
		head->first_c = head->first_c->next_command;
		free(t);
	}
	free(head);
}
