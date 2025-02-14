/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delimiters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mkling <mkling@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 11:29:43 by mkling            #+#    #+#             */
/*   Updated: 2025/02/14 11:29:57 by mkling           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* For use on char, not on tokens */
void	remove_delimiter(t_shell *shell, char **ptr_to_string)
{
	char	*original_string;
	char	*quotefree_string;
	size_t	len;

	if (shell->critical_er || !ptr_to_string || !*ptr_to_string)
		return ;
	original_string = *ptr_to_string;
	len = ft_strlen(original_string);
	if (ft_strchr(DELIMITERS, original_string[0]) == NULL
		|| ft_strchr(DELIMITERS, original_string[len]) == NULL)
		return ;
	quotefree_string = ft_calloc(sizeof(char), len - 1);
	ft_strlcat(quotefree_string, &original_string[1], len - 1);
	free(original_string);
	*ptr_to_string = quotefree_string;
}
