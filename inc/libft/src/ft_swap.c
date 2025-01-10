/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_swap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:08:43 by mkling            #+#    #+#             */
/*   Updated: 2024/12/30 11:12:30 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	ft_swap_int(int *a, int *b)
{
	int	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	ft_swap_ptr(void **a, void **b, size_t size)
{
	void *tmp = malloc(sizeof(size));
	if (!tmp)
		return ;
	ft_memcpy(tmp, a, size);
	ft_memcpy(a, b, size);
	ft_memcpy(a, tmp, size);
	free(tmp);
}
