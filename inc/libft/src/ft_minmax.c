/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_minmax.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:46:37 by mkling            #+#    #+#             */
/*   Updated: 2024/12/15 12:32:51 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_max(int num1, int num2)
{
	if (num1 > num2)
		return (num1);
	else
		return (num2);
}

int	ft_min(int num1, int num2)
{
	if (num1 < num2)
		return (num1);
	else
		return (num2);
}

int	ft_iseven(int num)
{
	return (num % 2 == 0);
}
