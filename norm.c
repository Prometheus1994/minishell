/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   norm.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ytlidi <ytlidi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 18:51:57 by ytlidi            #+#    #+#             */
/*   Updated: 2025/07/05 18:54:57 by ytlidi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*remove_quote(t_token *token, t_env *env)
{
	char	*str;
	char	*new_str;
	char	q;
	t_env	*env_line;

	int (i), (j), (continue_flag), flag = 0;
	j = 0;
	remove_quote_func_init(&i, token, &new_str, env);
	while (str[i] != '\0')
	{
		continue_flag = skipping_if_quote_mark(str, &i, &flag, &q);
		if ((str[i] == '$' && ((flag % 2 == 1 && q == '"') || flag % 2 == 0))
			&& token->type != TOKEN_HEREDOC)
		{
			continue_flag = printing_dollar(new_str, &j, str, &i);
			env_line = find_env_exp(env, &str[i]);
			continue_flag = expanding_to_an_empty_string(str, &i, env_line);
			continue_flag = expanding_to_a_real_value(new_str, &j, &i,
					env_line);
		}
		if (continue_flag == 1)
			continue ;
		new_str[j++] = str[i++];
	}
	new_str[j] = '\0';
	if (flag % 2 == 1)
	{
		printf("quote not closed\n");
		return (free(new_str), NULL);
	}
	return (new_str);
}
