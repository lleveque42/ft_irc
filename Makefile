# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 08:42:57 by arudy             #+#    #+#              #
#    Updated: 2022/11/21 17:56:25 by lleveque         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserv

DIR_SRCS = srcs

DIR_OBJS = objects

SRCS	= main.cpp Server/Server.cpp User/User.cpp Utils/helper.cpp \
		  $(addprefix Commands/, nick.cpp pass.cpp pong.cpp user.cpp \
		  quit.cpp command.cpp )

OBJS	= $(SRCS:%.cpp=$(DIR_OBJS)/%.o)

CC		= c++

RM		= rm -rf

MKDIR	= mkdir -p

CFLAGS	= -Wall -Werror -Wextra -pedantic -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(DIR_OBJS)/%.o : $(DIR_SRCS)/%.cpp
			${MKDIR} ${dir $@}
		${CC} ${CFLAGS} -c $< -o $@

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re: fclean all

rc: re
	${RM} ${OBJS} && clear && ./ircserv 6667 lol

.PHONY: all clean fclean re rc
