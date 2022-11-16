# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 08:42:57 by arudy             #+#    #+#              #
#    Updated: 2022/11/16 13:53:55 by lleveque         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserv

SRCS	= $(addprefix srcs/, main.cpp Server.cpp Client.cpp)

OBJS	= $(SRCS:.cpp=.o)
# DEPS	= $(SRCS:.cpp=.d)

CC		= c++

RM		= rm -f

CFLAGS	= -Wall -Werror -Wextra -pedantic -MMD -MP -std=c++98

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
	${RM} ${OBJS} && clear

# -include $(DEPS)

.PHONY: all clean fclean re rc
