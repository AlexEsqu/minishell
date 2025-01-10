/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_iter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 19:32:09 by mkling            #+#    #+#             */
/*   Updated: 2024/12/21 20:53:54 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../inc/libft.h"

void	ft_lstiter(t_list *lst, void (*f)(void *))
{
	t_list	*iteri;

	iteri = lst;
	while (iteri)
	{
		(*f)(iteri->content);
		iteri = iteri->next;
	}
}

int	ft_lstsize(t_list *lst)
{
	int		i;
	t_list	*iteri;

	i = 0;
	if (lst == NULL)
		return (0);
	iteri = lst;
	while (iteri != NULL)
	{
		iteri = iteri->next;
		i++;
	}
	return (i);
}

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*current;

	current = lst;
	if (current == NULL)
		return (NULL);
	while (current->next != NULL)
		current = current->next;
	return (current);
}

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;

	if (!lst || !f || !del)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		new_node = ft_lstnew(NULL);
		if (!new_node)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		new_node->content = (*f)(lst->content);
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}

void	ft_lstpop(t_list **head, t_list *node, void (*del)(void *))
{
	if (!head || !node || !del)
		return ;
	if (node->prev)
		node->prev->next = node->next;
	else
		*head = node->next;
	if (node->next)
		node->next->prev = node->prev;
	ft_lstdelone(node, del);
	node = NULL;
}
