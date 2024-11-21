/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_max.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 15:46:37 by mkling            #+#    #+#             */
/*   Updated: 2024/10/04 15:57:51 by mkling           ###   ########.fr       */
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
