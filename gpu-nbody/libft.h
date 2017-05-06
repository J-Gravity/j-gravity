/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmclaugh <pmclaugh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/29 16:49:08 by pmclaugh          #+#    #+#             */
/*   Updated: 2017/04/21 21:25:34 by pmclaugh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <fcntl.h>
# define BUFF_SIZE 32

typedef struct	s_ftlist
{
	void			*content;
	size_t			content_size;
	struct s_ftlist	*next;
}				t_ftlist;

char			*ft_strtrim(char const *s);
char			*ft_strsub(char const *s, unsigned int start, size_t len);
char			*ft_strstr(const char *big, const char *little);
char			**ft_strsplit(const char *s, char c);
char			*ft_strrchr(const char *s, int c);
char			*ft_strnstr(const char *big, const char *little, size_t len);
char			*ft_strnew(size_t size);
int				ft_strnequ(char const *s1, char const *s2, size_t n);
char			*ft_strncpy(char *dest, const char *src, unsigned int n);
int				ft_strncmp(const char *s1, const char *s2, unsigned int n);
char			*ft_strncat(char *s1, const char *s2, size_t n);
char			*ft_strmapi(const char *s, char(*f)(unsigned int, char));
char			*ft_strmap(const char *s, char(*f)(char));
size_t			ft_strlcat(char *dest, const char *src, size_t size);
char			*ft_strjoin(const char *s1, const char *s2);
void			ft_striteri(char *s, void (*f)(unsigned int, char *));
void			ft_striter(char *s, void (*f)(char *));
int				ft_strequ(char const *s1, char const *s2);
size_t			ft_strlen(const char *str);
char			*ft_strdup(const char *s1);
void			ft_strdel(char **as);
char			*ft_strcpy(char *dst, const char *src);
int				ft_strcmp(const char *s1, const char *s2);
void			ft_strclr(char *s);
char			*ft_strchr(const char *s, int c);
char			*ft_strcat(char *s1, const char *s2);
void			ft_putnbr_fd(int n, int fd);
void			ft_putnbr(int n);
void			ft_putstr_fd(const char *s, int fd);
void			ft_putendl_fd(const char *s, int fd);
void			ft_putstr(const char *str);
void			ft_putendl(const char *s);
void			ft_putchar_fd(char c, int fd);
void			ft_putchar(char c);
void			*ft_memmove(void *dest, const void *src, size_t n);
void			ft_memdel(void **ap);
void			*ft_memcpy(void *dest, const void *src, size_t n);
int				ft_memcmp(const void *s1, const void *s2, size_t n);
void			*ft_memchr(const void *s, int c, size_t n);
void			*ft_memccpy(void *dest, const void *src, int c, size_t n);
void			*ft_memalloc(size_t size);
int				ft_isprint(int c);
int				ft_isascii(int c);
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
void			*ft_memset(void *b, int c, size_t len);
void			ft_bzero(void *s, size_t n);
int				ft_atoi(const char *str);
long			ft_atol(const char *str);
int				ft_toupper(int c);
int				ft_tolower(int c);
int				ft_abs(int n);
int				ft_exp(int x, int n);
int				ft_iswspace(char c);
int				get_next_line(int fd, char **line);
char			*ft_itoa_base(int value, int base);
char			*ft_ltoa_base(long value, int base);
char			*ft_lltoa_base(long long value, int base);
char			*ft_ulltoa_base(unsigned long long value, int base);
char			*ft_ultoa_base(unsigned long value, int base);
char			*ft_utoa_base(unsigned int value, int base);
char			*ft_itoa(int n);
char			*ft_ltoa(long n);
char			*ft_lltoa(long long n);

t_ftlist		*ft_lstnew(const void *content, size_t content_size);
t_ftlist		*ft_lstmap(t_ftlist *lst, t_ftlist *(*f)(t_ftlist *elem));
void			ft_lstiter(t_ftlist *lst, void(*f)(t_ftlist *elem));
void			ft_lstdelone(t_ftlist **alst, void(*del)(void *, size_t));
void			ft_lstdel(t_ftlist **alst, void(*del)(void *, size_t));
void			ft_lstadd(t_ftlist **alst, t_ftlist *new);
t_ftlist		*ft_lstat(t_ftlist *begin_list, unsigned int i);
int				ft_lstlen(t_ftlist *begin_list);

int				ft_rand(int max);
int				*random_sequence(int max);

typedef	struct	s_heap
{
	int	capacity;
	int size;
	int *items;
}				t_heap;

int				h_parent_ind(int ind);
int				h_left_ind(int ind);
int				h_right_ind(int ind);
int				h_left_val(t_heap *heap, int ind);
int				h_right_val(t_heap *heap, int ind);
int				h_parent_val(t_heap *heap, int ind);
int				h_peek(t_heap *heap);
void			h_swap(t_heap *heap, int a, int b);
t_heap			*new_heap(int *items, int size);
void			heapify_down(t_heap *heap);
void			heapify_up(t_heap *heap);
void			h_add(t_heap *heap, int elem);
int				h_pop(t_heap *heap);
void			ensure_capacity(t_heap *heap);

void			heap_sort(int *data, int n);

typedef struct	s_rem
{
	int				fd;
	char			rem[BUFF_SIZE];
	int				len;
	struct s_rem	*next;
}				t_rem;

int				get_next_line(const int fd, char **line);

#endif
