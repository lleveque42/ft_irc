/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 11:41:37 by lleveque          #+#    #+#             */
/*   Updated: 2022/11/24 13:08:03 by arudy            ###   ########.fr       */
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
		return _sendError(user, ERR_ALREADYREGISTERED(user->getClient(), user->getNick(), "USER"));
	for (int i = 0; i < 4; i++) {
		begin = args.begin();
		if (i < 3) {
			end_pos = args.find(' ');
			end = begin + end_pos;
			if (end_pos == args.npos)
				return _sendError(user, ERR_NEEDMOREPARAMS(user->getClient(), user->getNick(), "USER"));
			argsVec.push_back(std::string(begin, end));
			args.erase(begin, end + 1);
		}
		else {
			if (*begin != ':')
				return _sendError(user, ERR_NOPREFIX(user->getClient(), user->getNick(), "USER"));
			begin++;
			argsVec.push_back(std::string(begin, args.end()));
			args.erase(begin, args.end());
		}
	}
	user->setUserName(argsVec[0]);
	// _users[pfd.fd]->setMode(argsVec[1]);
	user->setHostName(argsVec[2]);
	user->setRealName(argsVec[3]);
	user->setFirstTry(false);
	return _registrationCompleted(user);
}
