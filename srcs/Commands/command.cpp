/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 09:32:47 by arudy             #+#    #+#             */
/*   Updated: 2022/11/28 10:31:47 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

void Server::_initCmd() {
	_cmds["PASS"] = &Server::_pass;
	_cmds["USER"] = &Server::_user;
	_cmds["NICK"] = &Server::_nick;
	_cmds["PING"] = &Server::_pong;
	_cmds["QUIT"] = &Server::_quit;
	_cmds["MODE"] = &Server::_mode;
	_cmds["JOIN"] = &Server::_join;
	_cmds["PART"] = &Server::_part;
	_cmds["WHOIS"] = &Server::_whois;
	_cmds["TOPIC"] = &Server::_topic;
	_cmds["PRIVMSG"] = &Server::_privmsg;
	_cmds["OPER"] = &Server::_oper;
	_cmds["WHO"] = &Server::_who;
}

int Server::_manageCmd(pollfd pfd, std::pair<std::string, std::string> cmd) {
	int ret;

	if (_users[pfd.fd]->getFirstTry())
		if ((ret = _acceptConnection(_users[pfd.fd], cmd)))
			return ret;
	if (cmd.first == "CAP")
		return 0;
	if (_cmds.find(cmd.first) != _cmds.end())
		return (this->*_cmds[cmd.first])(_users[pfd.fd], cmd.second);
	return 3;
}
