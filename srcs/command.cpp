/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 09:32:47 by arudy             #+#    #+#             */
/*   Updated: 2022/11/21 11:28:46 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/User.hpp"

void Server::_initCmd() {
	_cmds["PASS"] = &Server::_pass;
	_cmds["USER"] = &Server::_user;
	_cmds["NICK"] = &Server::_nick;
	_cmds["PING"] = &Server::_pong;
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

int	Server::_pass(User *user, std::string args) {
	user->setTriedToAuth(true);
	if (user->getAuth())
		return 0; //voir quoi faire quand deja authentifié
	if (args != _password)
		return _disconnectUser(user, 2);
	if (!user->getAuth()) {
		user->setAuth(true);
		std::string ok("\033[92mConnection accepted !\n\033[093mWelcome to our IRC server !\033[00m\r\n");
		_sendAll(user->getUserSd(), ok.c_str(), ok.length(), 0);
		std::cout << GREEN BOLD "[ircserv]" RESET GREEN " Send    -->    " GREEN BOLD "[Client " << user->getUserSd() << "]" RESET GREEN ":    Connection accepted: Password correct" << RESET << std::endl;
	}
	return 0;
}

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

int	Server::_nick(User *user, std::string buff) {
	if (buff.empty())
		return _sendError(user, ":431  \033[91mNick: No nickname provided\033[00m\r\n");
	else if (!_validChars(buff))
		return _sendError(user, ":432  \033[91mNick: Erroneus nickname\033[00m\r\n");
	else if (_nickAlreadyUsed(user, buff)) {
		if (user->getFirstTry()) {
			user->setNick(buff);
			buff.insert(buff.end(), '_');
		}
		else
			return _sendError(user, ":433  \033[91mNick: Nickname is already in use\033[00m\r\n");
	}
	std::string old_nick;
	if (user->getNick().empty())
		old_nick = buff;
	else
		old_nick = user->getNick();
	user->setNick(buff);
	std::string msg = ":" + old_nick + " NICK " + user->getNick() + "\r\n";
	return _sendExecuted(user, msg);
}

int Server::_pong(User *user, std::string buff) {
	(void)buff;
	std::string msg = "PONG irc.server\r\n";
	return _sendExecuted(user, msg);
}
