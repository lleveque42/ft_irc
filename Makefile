# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 08:42:57 by arudy             #+#    #+#              #
#    Updated: 2022/11/21 11:54:05 by lleveque         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserv

SRCS	= $(addprefix srcs/, main.cpp Server/Server.cpp User/User.cpp Utils/helper.cpp \
		  $(addprefix Commands/, nick.cpp pass.cpp pong.cpp user.cpp\
		  command.cpp ) )

OBJS	= $(SRCS:.cpp=.o)
# DEPS	= $(SRCS:.cpp=.d)

CC		= c++

RM		= rm -f

CFLAGS	= -Wall -Werror -Wextra -pedantic -std=c++98
# -MMD -MP

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o : %.cpp
		${CC} ${CFLAGS} -c $< -o $@

clean:
	${RM} ${OBJS}
# ${DEPS}

fclean: clean
	${RM} ${NAME}

re: fclean all

rc: re
	${RM} ${OBJS} && clear && ./ircserv 6667 lol

# -include $(DEPS)

.PHONY: all clean fclean re rc
