#include "minishell.h"

void filling_type_s_or_d_quote(t_token *list)
{
	if (list->token[0] == '\'')
		list->type = TOKEN_S_QUOTE;
	if (list->token[0] == '"')
		list->type = TOKEN_D_QUOTE;
}

int quote_tokens(char *str, t_token **list, int *i)
{
	char	q;
	int		j;
	char	*s;
	t_token	*token;

	while (str[*i] == ' ')
		(*i)++;
	if (str[*i] == '\'' || str[*i] == '"')
	{
		q = str[*i];
		j = (*i)++;
		while (str[*i] != q && str[*i] != '\0')
			(*i)++;
		if (str[*i] == '\0')
			printf("quote not closed\n");
		if (*i > j)
		{
			s = ft_substr(str, j, *i - j + 1); //free
			if (s == NULL)
				return (1);
			token = ft_lstnew_token(s); //free
			filling_type_s_or_d_quote(token);
			ft_lstadd_back_token(list, token);
		}
		(*i)++;
	}
	return (0);
}
void filling_type_pipe_or_rd(t_token *list)
{
	if (list->token[1] == '\0')
	{
		if (list->token[0] == '|')
			list->type = TOKEN_PIPE;
		if (list->token[0] == '>')
			list->type = TOKEN_RD_IN;
		if (list->token[0] == '<')
			list->type = TOKEN_RD_OUT;
	}
	else
	{
		if (list->token[1] == '>')
			list->type = TOKEN_APPEND;
		if (list->token[1] == '<')
			list->type = TOKEN_HEREDOC;
	}
}

int inner_pipes_and_rds_tokens(char *str, t_token **list, int **i, int s_or_d)
{
	char 	*s;
	int		j;
	t_token	*token;

	s = malloc(s_or_d + 1); //free
	if (s == NULL)
		return (1);
	j = 0;
	while (j < s_or_d)
	{
		s[j] = str[**i];
		j++;
	}
	s[j] = '\0';
	token = ft_lstnew_token(s); //free
	filling_type_pipe_or_rd(token);
	ft_lstadd_back_token(list, token);
	**i += s_or_d;
	return (0);
}

int pipes_and_rds_tokens(char *str, t_token **list, int *i)
{
	char	rd;

	if (str[*i] == '|' || str[*i] == '>' || str[*i] == '<')
	{
		rd = str[*i];
		if (str[*i + 1] == rd && rd != '|')
			return (inner_pipes_and_rds_tokens(str, list, &i, 2));
		else
			return (inner_pipes_and_rds_tokens(str, list, &i, 1));
	}
	return (0);
}

int word_tokens(char *str, t_token **list, int *i)
{
	int		j;
	char	*s;
	t_token *token;

	j = *i;
	while (str[*i] != ' ' && str[*i] != '|'
		&& str[*i] != '>' && str[*i] != '<' && str[*i] != '\0')
		(*i)++;
	if (*i > j)
	{
		s = ft_substr(str, j, *i - j); //free
		if (s == NULL)
			return (1);
		token = ft_lstnew_token(s); //free
		token->type = TOKEN_WORD; //not necessary (it's 0 by default)
		ft_lstadd_back_token(list, token);
	}
	return (0);
}

int words_count(t_token *beginning)
{
	int		i;
	t_token	*current;

	current = beginning;
	i = 0;
	while (current != NULL && current->type == TOKEN_WORD)
	{
		i++;
		current = current->next;
	}
	return i;
}

t_token *parse_input(char *str)
{
	t_token			*token_list;
	t_token			*current_token;
	t_command		*command_list;
	t_command		*current_command;
	t_command		*command;
	t_redirection	*redirection_list = NULL;
	t_redirection	*current_redirection;
	t_redirection	*redirection;
	char			**args;
	int				i;

	token_list = NULL;
	i = 0;
	while (str[i] != '\0')
	{
		if (quote_tokens(str, &token_list, &i))
			return (NULL);
		if (pipes_and_rds_tokens(str, &token_list, &i))
			return (NULL);
		if (word_tokens(str, &token_list, &i))
			return (NULL);
	}
	command_list = NULL;
	current_token = token_list;
	args = malloc(sizeof(char *) * (words_count(token_list) + 1));
	i = 0;
	while (current_token != NULL && current_token->type == TOKEN_WORD)
	{
		args[i] = ft_strdup(current_token->token);
		current_token = current_token->next;
		i++;
	}
	args[i] = NULL;
	command = ft_lstnew_command(args);
	i = 0;
	while (command->args[i])
	{
		printf("%s\n",command->args[i]);
		i++;
	}
	// ft_lstadd_back_command(command_list, command);
		printf("Here\n");
	while (current_token->type != TOKEN_PIPE && current_token != NULL)
	{
		redirection = NULL;
		if (current_token && current_token->type >= 4 && current_token->type <= 7)
			redirection = ft_lstnew_redirection(current_token->type, current_token->next->token);
		ft_lstadd_back_redirection(&redirection_list, redirection);
		current_token = current_token->next->next;
	}
	/////////////////////////////////////////////
/*	current_token = token_list;
	current_command->command = current_token->token;
	i = 0;
	printf("test\n");
	while (current_token && current_token->type == TOKEN_WORD)
	{
		current_command->args[i] = current_token->token;
		current_token = current_token->next;
		i++;
	}
	current_command->args[i] = NULL;*/
	// if (current_token->type == TOKEN_PIPE)
	// {
	// 	current_command = current_command->next;
	// }
	//	if 
	return (token_list);
}