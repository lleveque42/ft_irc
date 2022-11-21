/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 09:52:20 by arudy             #+#    #+#             */
/*   Updated: 2022/11/21 09:53:30 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/User.hpp"

int Server::_sendError(User *user, std::string err) {
	_sendAll(user->getUserSd(), err.c_str(), err.length(), 0);
	std::cout << RED BOLD "[ircserv]" RESET RED " Send    -->    " RED BOLD "[Client " << user->getUserSd() << "]" RESET RED << ":    " << err << RESET;
	return 1;
}

int Server::_sendExecuted(User *user, std::string ret) {
	_sendAll(user->getUserSd(), ret.c_str(), ret.length(), 0);
	std::cout << GREEN BOLD "[ircserv]" RESET GREEN " Send    -->    " GREEN BOLD "[Client " << user->getUserSd() << "]" RESET GREEN << ":    " << ret << RESET;
	return 0;
}

bool	Server::_validChars(std::string s) {
	for(size_t i = 0; i < s.length(); i++)
		if (s[i] < 33 && s[i] > 126)
			return false;
	return true;
}

bool	Server::_nickAlreadyUsed(User *current, std::string s) {
	for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); it++)
		if (it->second->getNick() == s && it->second->getUserSd() != current->getUserSd())
			return true;
	return false;
}

int Server::_acceptConnection(User *user, std::pair<std::string, std::string> cmd) {
	if (!user->getCap() && cmd.first == "CAP" && cmd.second == "LS")
		return user->setCap(true), 0;
	else if (!user->getTriedToAuth() && cmd.first == "PASS") {
		if (!user->getCap())
			return _disconnectUser(user, 2);
	}
	else if (user->getNick() == "" && cmd.first == "NICK") {
		if (!user->getAuth())
			return _disconnectUser(user, 2);
	}
	else if (cmd.first == "USER") {
		if (user->getNick() == "")
			return _disconnectUser(user, 2);
	}
	else
		return _disconnectUser(user, 2);
	return 0;
}
