/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:08:12 by arudy             #+#    #+#             */
/*   Updated: 2022/11/17 100:00:224 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(char *port, char *password) : _password(password), _port(port), _pfds(), _fd_count(0) {
	_initCmd();
}

void Server::clear() {
	_recvs.clear();
	_cmds.clear();
	for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); it++)
		delete (*it).second;
	_users.clear();
	for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); it++)
		close(it->fd);
	_pfds.clear();
}

Server::~Server() {
	clear();
}

//////////////// INIT ///////////////////

void Server::_initCmd() {
	_cmds["PASS"] = &Server::_pass;
	_cmds["USER"] = &Server::_user;
	_cmds["NICK"] = &Server::_nick;
	_cmds["PING"] = &Server::_pong;
}

void Server::setup()
{
	struct addrinfo hints;
	struct addrinfo *servinfo = NULL;
	struct addrinfo *tmp = NULL;
	int optval = 1; // Allows other sockets to bind to this port, unless there is an active listening socket bound to the port already.

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, _port, &hints, &servinfo)) // remplie servinfo
		throw Exception::getaddrinfo();
	for (tmp = servinfo; tmp; tmp = tmp->ai_next) {
		if ((_sd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) // retourne un socket descriptor pour les appels systemes
			continue;
		setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)); // est ce qu on est sur qu on veut reallouer l'addresse ??
		fcntl(_sd, F_SETFL, O_NONBLOCK);
		if (bind(_sd, servinfo->ai_addr, servinfo->ai_addrlen)) { // binds le socket sur le host
			close(_sd);
			continue;
		}
		break;
	}
	if (tmp == NULL) // aucune adresse de bind
		throw Exception::bind();
	freeaddrinfo(servinfo);
	if (listen(_sd, 10)) // queue toutes les connections entrantes, 10 max (arbitraire ca pourrait etre 20 au max)
		throw Exception::listen();
	_pfds.push_back(pollfd());
	_pfds.back().fd = _sd;
	_pfds.back().events = POLLIN;
	_fd_count = 1;
	std::cout << ORANGE BOLD "[ircserv]" GREEN " setted up on port " << _port << " \033[32m\xE2\x9C\x93\033[0m" << RESET << std::endl;
}

void Server::launch() {
	if (_fd_count == 1)
		std::cout << ORANGE BOLD "[ircserv]" RESET BOLD " waiting for incoming connections... 😴" << RESET << std::endl;
	if (poll(&_pfds[0], _fd_count, -1) == -1) {
		if (errno != EINTR)
			throw Exception::poll();
		return;
	}
	if (_pfds[0].revents == POLLIN)
		_acceptUser();
	std::vector<pollfd>::iterator ite = _pfds.end();
	for (std::vector<pollfd>::iterator it = _pfds.begin() + 1; it != ite; it++)
		if ((*it).revents == POLLIN)
			_manageRequest(*it);
}

//////////////// MANAGE USERS ///////////////////

void Server::_acceptUser() {
	int new_sd;
	sockaddr_storage new_addr; // ou toutes les infos de la nouvelle connexion vont aller
	socklen_t new_addr_size; // sizeof sockaddr_storage

	std::cout << ORANGE BOLD "=========" GREEN "=========================" << RESET << std::endl;
	std::cout << ORANGE BOLD "[ircserv]" GREEN " new incoming connection!" << RESET << std::endl;
	std::cout << ORANGE BOLD "=========" GREEN "=========================" << RESET << std::endl;
	new_addr_size = sizeof new_addr;
	new_sd = accept(_sd, (sockaddr *)&new_addr, &new_addr_size); // accepte les connections entrantes, le nouveau fd sera pour recevoir et envoyer des appels
	_users.insert(std::pair<int, User*>(new_sd, new User(new_sd))); // pair first garder user_id ? Ou mettre le sd
	_pfds.push_back(pollfd());
	_pfds.back().fd = new_sd;
	_pfds.back().events = POLLIN;
	_fd_count++;
}

int Server::_disconnectUser(pollfd pfd, int ret) {
	if (_users[pfd.fd]->getNick() == "")
		_sendError(pfd, ":431  \033[91mNick: No nickname provided\033[00m\r\n");
	else if (_users[pfd.fd]->getAuth()) {
		std::cout << ORANGE BOLD "=========" RED "==============================" << RESET << std::endl;
		std::cout << ORANGE BOLD "[ircserv]" RED " Client " << pfd.fd << " has left the server!" << std::endl;
		std::cout << ORANGE BOLD "=========" RED "==============================" << RESET << std::endl;
	}
	else if (!_users[pfd.fd]->getTriedToAuth())
		_sendError(pfd, ":461 \033[91mConnection refused: No password provided\033[00m");
	close(pfd.fd);
	delete _users[pfd.fd];
	std::vector<pollfd>::iterator it;
	for (it = _pfds.begin() + 1; it->fd != pfd.fd; it++)
		;
	_pfds.erase(it);
	_users.erase(pfd.fd);
	_recvs.clear();
	_fd_count--;
	return ret;
}

///////////// MANAGE REQUESTS /////////////////

int Server::_fillRecvs(std::string buff) {
	size_t space_pos;
	size_t backr_pos;
	std::string::iterator begin;
	std::string::iterator space;
	std::string::iterator backr;
	int lines = std::count(buff.begin(), buff.end(), '\n');

	for (int i = 0; i < lines; i++) {
		begin = buff.begin();
		space_pos = buff.find(' ');
		backr_pos = buff.find('\r');
		space = begin + space_pos;
		backr = begin + backr_pos;
		if (space_pos == buff.npos)
			_recvs.push_back(std::make_pair(std::string(begin, buff.end() - 1), std::string()));
		else {
			if (backr_pos == buff.npos)
				_recvs.push_back(std::make_pair(std::string(begin, space), std::string(space + 1, buff.end() - 1)));
			else
				_recvs.push_back(std::make_pair(std::string(begin, space), std::string(space + 1, backr)));
		}
		buff.erase(begin, backr + 2);
	}
	return lines;
}

size_t Server::_recvAll(pollfd pfd) {
	char buffer[BUFFER_SIZE + 1];
	int size;

	while (1) {
		memset(buffer, 0, BUFFER_SIZE + 1);
		if ((size = recv(pfd.fd, buffer, BUFFER_SIZE, 0)) == -1)
			return -1;
		if (size == 0)
			return _disconnectUser(pfd, 0);
		buffer[size] = 0;
		_buff += buffer;
		if (_buff.find('\n') != _buff.npos)
			break;
	}
	return 0;
}

int Server::_manageRequest(pollfd pfd) {
	int ret;
	int lines;
	int status;
	// int size;

	if ((ret = _recvAll(pfd)))
		return ret;
	_recvs.clear();
	lines = _fillRecvs(std::string(_buff));
	_buff.clear();
	for (int i = 0; i < lines; i++) {
		std::cout << BLUE BOLD "[ircserv]" RESET BLUE " Recv    <--    " BLUE BOLD "[Client " << pfd.fd << "]" RESET BLUE ":    " << _recvs[i].first << " " << _recvs[i].second << RESET << std::endl;
		if ((status = _manageCmd(pfd, _recvs[i])))
		{
			if (status == 2)
				break;
			else if (status == 3)
				std::cerr << "Cmd not found!\n";
		}
	}
	return 0;
}

int Server::_manageCmd(pollfd pfd, std::pair<std::string, std::string> cmd) {
	if (_users[pfd.fd]->getFirstTry())
	{
		if (!_users[pfd.fd]->getCap() && cmd.first == "CAP" && cmd.second == "LS")
		{
			_users[pfd.fd]->setCap(true);
			return 0;
		}
		else if (!_users[pfd.fd]->getTriedToAuth() && cmd.first == "PASS")
		{
			if (!_users[pfd.fd]->getCap())
				return _disconnectUser(pfd, 2);
		}
		else if (_users[pfd.fd]->getNick() == "" && cmd.first == "NICK")
		{
			if (!_users[pfd.fd]->getAuth())
				return _disconnectUser(pfd, 2);
		}
		else if (!_users[pfd.fd]->getFirstTry() && cmd.first == "USER")
		{
			if (_users[pfd.fd]->getNick() == "")
				return _disconnectUser(pfd, 2);
		}
		else if (!_users[pfd.fd]->getCap())
			return _disconnectUser(pfd, 2);
		else if (!_users[pfd.fd]->getTriedToAuth())
			return _disconnectUser(pfd, 2);
		else if (!_users[pfd.fd]->getAuth())
			return _disconnectUser(pfd, 2);
		else if (_users[pfd.fd]->getNick() == "")
			return _disconnectUser(pfd, 2);
	}
	if (_cmds.find(cmd.first) != _cmds.end())
		return (this->*_cmds[cmd.first])(pfd, cmd.second);
	return 1;
}

int Server::_sendAll(int fd, const char *buf, size_t len, int flags) {
	size_t sent = 0;
	size_t toSend = len;
	int ret = 0;

	while (sent < len) {
		ret = send(fd, (buf + sent), toSend, flags);
		if (ret == -1)
			return ret;
		sent += ret;
		toSend -= ret;
	}
	return 0;
}

/////////////////  COMMANDS  ////////////////////

int	Server::_pass(pollfd pfd, std::string args) {
	_users[pfd.fd]->setTriedToAuth(true);
	if (_users[pfd.fd]->getAuth())
		return 0; //voir quoi faire quand deja authentifié
	if (args != _password) {
		std::string err(":464 \033[91mConnection refused: Password incorrect\033[00m\r\n");
		_sendAll(pfd.fd, err.c_str(), err.length(), 0);
		std::cout << RED BOLD "[ircserv]" RESET RED " Send    -->    " RED BOLD "[Client " << pfd.fd << "]" RESET RED ":    Connection refused: Password incorrect" << RESET << std::endl;
			return _disconnectUser(pfd, 2);
	}
	if (!_users[pfd.fd]->getAuth()) {
		_users[pfd.fd]->setAuth(true);
		std::string ok("\033[92mConnection accepted !\n\033[093mWelcome to our IRC server !\033[00m\r\n");
		_sendAll(pfd.fd, ok.c_str(), ok.length(), 0);
		std::cout << GREEN BOLD "[ircserv]" RESET GREEN " Send    -->    " GREEN BOLD "[Client " << pfd.fd << "]" RESET GREEN ":    Connection accepted: Password correct" << RESET << std::endl;
	}
	return 0;
}

int	Server::_user(pollfd pfd, std::string args) {
	std::vector<std::string> argsVec;
	std::string::iterator begin;
	std::string::iterator end;
	size_t end_pos;

	if (!_users[pfd.fd]->getFirstTry()) {
		std::string err(":462 \033[91mUser: You may not reregister\033[00m\r\n");
		return _sendError(pfd, err);
	}
	for (int i = 0; i < 4; i++) {
		begin = args.begin();
		if (i < 3) {
			end_pos = args.find(' ');
			end = begin + end_pos;
			if (end_pos == args.npos) {
				std::string err(":461 \033[91mUser: Not enough parameters\033[00m\r\n");
				_sendAll(pfd.fd, err.c_str(), err.length(), 0);
				return 0;
			}
			argsVec.push_back(std::string(begin, end));
			args.erase(begin, end + 1);
		}
		else {
			begin++;
			argsVec.push_back(std::string(begin, args.end()));
			args.erase(begin, args.end());
		}
	}
	_users[pfd.fd]->setUserName(argsVec[0]);
	// _users[pfd.fd]->setMode(argsVec[1]);
	_users[pfd.fd]->setHostName(argsVec[2]);
	_users[pfd.fd]->setRealName(argsVec[3]);
	std::string user_str = ":irc.server 001 " + _users[pfd.fd]->getNick() + " :\r\n";
	_users[pfd.fd]->setFirstTry(false);
	return _sendExecuted(pfd, user_str);
}

int	Server::_nick(pollfd pfd, std::string buff) {
	if (buff.empty())
		return _sendError(pfd, ":431  \033[91mNick: No nickname provided\033[00m\r\n");
	else if (!_validChars(buff))
		return _sendError(pfd, ":432  \033[91mNick: Erroneus nickname\033[00m\r\n");
	else if (_nickAlreadyUsed(_users[pfd.fd], buff)) {
		if (_users[pfd.fd]->getFirstTry()) {
			_users[pfd.fd]->setNick(buff);
			buff.insert(buff.end(), '_');
		}
		else
			return _sendError(pfd, ":433  \033[91mNick: Nickname is already in use\033[00m\r\n");
	}
	std::string old_nick;
	if (_users[pfd.fd]->getNick().empty())
		old_nick = buff;
	else
		old_nick = _users[pfd.fd]->getNick();
	_users[pfd.fd]->setNick(buff);
	std::string msg = ":" + old_nick + " NICK " + _users[pfd.fd]->getNick() + "\r\n";
	return _sendExecuted(pfd, msg);
}

int Server::_pong(pollfd pfd, std::string buff) {
	(void)buff;
	std::string msg = "PONG irc.server\r\n";
	return _sendExecuted(pfd, msg);
}

/////////////////  HELPERS  ////////////////////

int Server::_sendError(pollfd pfd, std::string err) {
	_sendAll(pfd.fd, err.c_str(), err.length(), 0);
	std::cout << RED BOLD "[ircserv]" RESET RED " Send    -->    " RED BOLD "[Client " << pfd.fd << "]" RESET RED << ":    " << err << RESET;
	return 1;
}

int Server::_sendExecuted(pollfd pfd, std::string ret) {
	_sendAll(pfd.fd, ret.c_str(), ret.length(), 0);
	std::cout << GREEN BOLD "[ircserv]" RESET GREEN " Send    -->    " GREEN BOLD "[Client " << pfd.fd << "]" RESET GREEN << ":    " << ret << RESET;
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
