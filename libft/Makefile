NAME = libft.a

CC = cc -g

FLAGS = -Wall -Wextra -Werror -I ./

SRC = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
	ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
	ft_atoi.c ft_calloc.c ft_strdup.c \
	ft_substr.c ft_strjoin.c ft_strjoin_free.c ft_strtrim.c ft_split.c \
	ft_itoa.c ft_strmapi.c ft_striteri.c ft_putchar_fd.c \
	ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	get_next_line_bonus.c get_next_line_utils_bonus.c \
	ft_swap_endianes.c

SRCBNS = ft_lstnew_bonus.c ft_lstadd_back_bonus.c ft_lstsize_bonus.c \
	ft_lstlast_bonus.c ft_lstadd_front_bonus.c \
	ft_lstdelone_bonus.c ft_lstclear_bonus.c ft_lstiter_bonus.c ft_lstmap_bonus.c

HEADER = libft.h

OBJ = $(SRC:.c=.o)

BOBJ = $(SRCBNS:.c=.o)

$(NAME): $(OBJ)
	ar -rsc $(NAME) $(OBJ)

bonus: $(OBJ) $(BOBJ)
	ar -rsc $(NAME) $(OBJ) $(BOBJ)

.c.o:
	$(CC) $(FLAGS) -c $< -o  $(<:.c=.o)

clean:
	rm -f $(OBJ) $(BOBJ)

fclean: clean
	rm -f $(NAME)

all: $(NAME)

re: fclean all

.PHONY: all fclean clean re
