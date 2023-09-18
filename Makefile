# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ccarnot <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/29 16:45:43 by ccarnot           #+#    #+#              #
#    Updated: 2023/09/15 10:31:48 by ccarnot          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = pipex

BONUS = pipex_bonus

CFLAGS = -g3 -Wall -Wextra -Werror

INCL = -I -I./libft

SRCS = ./srcs/main.c ./srcs/clear.c ./srcs/tubeline.c ./srcs/child_process.c ./srcs/utils.c \
	./srcs/here_doc.c ./srcs/single_process.c

SRCS_BONUS = ./srcs_bonus/main_bonus.c ./srcs_bonus/clear_bonus.c ./srcs_bonus/tubeline_bonus.c ./srcs_bonus/child_process_bonus.c \
	./srcs_bonus/utils_bonus.c ./srcs_bonus/here_doc_bonus.c ./srcs_bonus/single_process_bonus.c

OBJS = $(SRCS:.c=.o)
OBJS_BONUS = $(SRCS_BONUS:.c=.o)

LIBFT = ./libft/libft.a

$(NAME): $(OBJS)
	make lib
	cc -o $(NAME) $(OBJS) $(LIBFT)

$(BONUS): $(OBJS_BONUS)
	make lib
	cc -o $(BONUS) $(OBJS_BONUS) $(LIBFT)

all: $(NAME)

%.o:%.c
	cc $(INCL) $(CFLAGS) -o $@ -c $<

lib:
	make -C ./libft/ bonus

clean:
	rm -f $(OBJS) $(OBJS_BONUS)

fclean: clean
	rm -f $(NAME) $(BONUS)

bonus: $(BONUS)

re: fclean
	make all

rebonus: fclean
	make bonus

.PHONY: all clean fclean bonus re rebonus
