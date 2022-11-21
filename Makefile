# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: arudy <arudy@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/11/16 08:42:57 by arudy             #+#    #+#              #
#    Updated: 2022/11/21 09:55:04 by arudy            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= ircserv

SRCS	= $(addprefix srcs/, main.cpp Server.cpp User.cpp command.cpp helper.cpp)

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
