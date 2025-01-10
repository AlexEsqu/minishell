/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bubble_sort.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 12:06:19 by mkling            #+#    #+#             */
/*   Updated: 2024/10/04 19:12:31 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/libft.h"

void	ft_bubble_sort(int *num_array, int size_array)
{
	int	index1;
	int	index2;

	index1 = 0;
	while (index1 < size_array)
	{
		index2 = 0;
		while (index2 < size_array - index1 - 1)
		{
			if (num_array[index2] > num_array[index2 + 1])
				ft_swap_int(&num_array[index2], &num_array[index2 + 1]);
			index2++;
		}
		index1++;
	}
}
