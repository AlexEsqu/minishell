/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_iter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/31 19:32:09 by mkling            #+#    #+#             */
/*   Updated: 2024/10/06 17:55:42 by mkling           ###   ########.fr       */
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

// void	ft_lstiterr(t_list *lst, void (*f)(void *))
// {
// 	if (!lst)
// 		return ;
// 	(*f)(lst->content);
// 		return (ft_lstiterr(lst->next, f));
// }

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

// int ft_lstsizer(t_list *lst)
// {
// 	if (!lst)
// 		return 0;
// 	return 1 + ft_lstsizer(lst->next);
// }
