/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: grdalmas <grdalmas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/08 17:49:13 by grdalmas          #+#    #+#             */
/*   Updated: 2018/04/05 19:41:44 by grdalmas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static	char	*ft_strjoinfrees1(char *s1, char *s2, int n)
{
	char		*str;

	if (!(str = ft_strnew(ft_strlen(s1) + n)))
		return (NULL);
	ft_strcpy(str, s1);
	ft_strncat(str, s2, n);
	ft_strdel(&s1);
	return (str);
}

static	int		check_rest(t_list *file, char **line)
{
	char		*tmp;
	int			cnt;

	cnt = -1;
	*line = NULL;
	if (!file->content)
		return (0);
	while (((char*)(file->content))[++cnt])
	{
		if (((char*)(file->content))[cnt] == '\n')
		{
			*line = ft_strndup((char*)file->content, cnt);
			if (!(tmp = ft_strsub((char*)file->content, cnt + 1, 
				ft_strlen((char*)file->content) - cnt)))
				return (-1);
			ft_memdel(&(file->content));
			file->content = (void*)tmp;
			return (1);
		}
	}
	return (0);
}

static	int		get_line(t_list *file, char **line)
{
	char		buff[BUFF_SIZE + 1];
	int			ret;
	int			check;

	if ((check = check_rest(file, line)))
		return (check);
	if ((ret = read(file->content_size, buff, BUFF_SIZE)) <= 0)
		return (ret);
	buff[ret] = '\0';
	if (!file->content)
		file->content = (char*)ft_strndup(buff, ret);
	else
		file->content = (char*)ft_strjoinfrees1(file->content, buff, ret);
	if (!file->content)
		return (-1);
	if ((check = check_rest(file, line)))
		return (check);
	return (get_line(file, line));
}

static	t_list	*check_fd(t_list **begin_list, size_t fd)
{
	t_list		*tmp;

	tmp = *begin_list;
	while (tmp)
	{
		if (tmp->content_size == fd)
			return (tmp);
		tmp = tmp->next;
	}
	if (!(tmp = ft_lstnew(NULL, 0)))
		return (NULL);
	tmp->content_size = fd;
	if (!(*begin_list))
		*begin_list = tmp;
	else
		ft_lstadd_back(*begin_list, tmp);
	return (tmp);
}

int				get_next_line(const int fd, char **line)
{
	static t_list	*begin_list;
	t_list			*file;
	int				result;

	if ((fd < 0 || !line) || !(file = check_fd(&begin_list, (size_t)fd)))
		return (-1);
	result = get_line(file, line);
	if (!result && file->content && !((char*)file->content)[0])
		return (0);
	else if (!result && file->content)
	{
		*line = (char*)file->content;
		file->content = NULL;
		result = 1;
	}
	return (result);
}
