# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jbeall <jbeall@student.42.us.org>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/01 14:21:53 by jbeall            #+#    #+#              #
#    Updated: 2018/12/26 16:12:12 by jbeall           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ssl
FLAGS = -Wall -Werror -Wextra
SRC_DIR = ./src/
SRC = ft_ssl.c ft_ssl_md5.c ft_ssl_md5_helpers.c ft_ssl_md5_helpers2.c \
ft_ssl_md5_helpers3.c ft_ssl_sha256.c ft_ssl_sha256_helpers.c \
ft_ssl_sha256_helpers2.c ft_ssl_sha512.c ft_ssl_sha512_helpers.c \
ft_ssl_sha512_helpers2.c ft_ssl_sha512_helpers3.c ft_ssl_gnb.c
INC_DIR = ./includes/
LIB_INC_DIR = ./libft/includes/
LIB = ./libft/libft.a

$(NAME):
	@make -C ./libft --no-print-directory
	@echo "Creating $(NAME) executable..."
	@clang -o $(NAME) $(FLAGS) $(addprefix $(SRC_DIR), $(SRC)) $(LIB) -I$(LIB_INC_DIR) -I$(INC_DIR) -O3
	@echo "\033[92mdone!\033[0m"
all: $(NAME)
dev:
	@rm -f ./ft_ls
	@make -C ./libft --no-print-directory
	@echo "compiling dev build..."
	@gcc -o $(NAME) $(FLAGS) -g $(addprefix $(SRC_DIR), $(SRC)) $(LIB) -I$(LIB_INC_DIR) -I$(INC_DIR)
	@echo "\033[92mdone!\033[0m"
clean:
	@echo "cleaning..."
	@make clean -C ./libft --no-print-directory
fclean: clean
	@rm -f $(NAME)
	@echo "ft_ls executable removed!"
	@make fclean -C ./libft --no-print-directory
re: fclean all
