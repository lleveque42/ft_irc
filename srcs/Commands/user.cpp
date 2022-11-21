/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:41:37 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/21 11:44:46 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

int	Server::_user(User *user, std::string args) {
	std::vector<std::string> argsVec;
	std::string::iterator begin;
	std::string::iterator end;
	size_t end_pos;

	if (!user->getFirstTry())
		return _sendError(user, ":462 \033[91mUser: You may not reregister\033[00m\r\n");
	for (int i = 0; i < 4; i++) {
		begin = args.begin();
		if (i < 3) {
			end_pos = args.find(' ');
			end = begin + end_pos;
			if (end_pos == args.npos)
				return _sendError(user, ":461 \033[91mUser: Not enough parameters\033[00m\r\n");
			argsVec.push_back(std::string(begin, end));
			args.erase(begin, end + 1);
		}
		else {
			if (*begin != ':')
				return _sendError(user, ":461 \033[91mUser: No prefix \033[00m\r\n");
			begin++;
			argsVec.push_back(std::string(begin, args.end()));
			args.erase(begin, args.end());
		}
	}
	user->setUserName(argsVec[0]);
	// _users[pfd.fd]->setMode(argsVec[1]);
	user->setHostName(argsVec[2]);
	user->setRealName(argsVec[3]);
	std::string user_str = ":irc.server 001 " + user->getNick() + " :\r\n";
	user->setFirstTry(false);
	return _sendExecuted(user, user_str);
}
