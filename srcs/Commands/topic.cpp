/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/25 09:54:50 by arudy             #+#    #+#             */
/*   Updated: 2022/11/25 11:46:12 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

std::pair<std::string, std::string> Server::_strToPair(std::string buff) {
	size_t i = buff.find(':');
	if (i == buff.npos)
		return std::make_pair(buff, std::string(""));
	// std::string first_tmp(buff.begin(), buff.begin() + i);
	// std::string second_tmp(buff.begin() + i + 1, buff.end());
	// std::cout << "first tmp|" << first_tmp << "|\n";;
	// std::cout << "second tmp|" << second_tmp << "|\n";;

	// std::string first(first_tmp.begin(), first_tmp.begin() + first_tmp.find_last_not_of(" ") + 1);
	// std::string second(second_tmp.begin() + second_tmp.find_first_not_of(" "), second_tmp.begin() + second_tmp.find_last_not_of(" ") + 1);

	std::string first(buff.begin(), buff.begin() + i + buff.find_first_not_of(" ") - 1);
	std::string second(buff.begin() + i + 1, buff.end());

	return std::make_pair(std::string(first), std::string(second));
}

int Server::_topic(User *user, std::string args) {
	std::cout << "TOPIC :" << args << "|\n";
	if (args.empty())
		return _sendError(user, ERR_NEEDMOREPARAMS(user->getClient(), user->getNick(), "TOPIC"));

	std::pair<std::string, std::string> pair = _strToPair(args);

	std::cout << "PAIR FIRST |" << pair.first << "|\n";
	std::cout << "PAIR SECOND |" << pair.second << "|\n";

	// if (pair.first.empty() && pair.second.empty())
		// return _sendError(user, ERR_NEEDMOREPARAMS(user->getClient(), user->getNick(), "TOPIC"));

	return 0;
}
