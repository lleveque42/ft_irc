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

#include "../../includes/Server.hpp"

Server::Server(char *port, char *password) : _creation_time(currentTime()),
 _password(password), _port(port), _pfds(), _fd_count(0) {
	_initCmd();
}

void Server::clear() {
	_recvs.clear();
	_cmds.clear();
	for (std::map<std::string, Channel*>::iterator it = _channels.begin(); it != _channels.end(); it++)
		delete (*it).second;
	_channels.clear();
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

void Server::setup() {
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
	if (tmp == NULL) { // aucune adresse de bind
		freeaddrinfo(servinfo);
		throw Exception::bind();
	}
	freeaddrinfo(servinfo);
	if (listen(_sd, 10)) // queue toutes les connections entrantes, 10 max (arbitraire ca pourrait etre 20 au max)
		throw Exception::listen();
	_pfds.push_back(pollfd());
	_pfds.back().fd = _sd;
	_pfds.back().events = POLLIN;
	_fd_count = 1;
	std::cout << DIS_CREATED(_creation_time)  << std::endl;
	std::cout << DIS_SERVSETTEDUP << _port << DIS_EMOJICHECK << std::endl;
}

void Server::launch() {
	if (_fd_count == 1)
		std::cout << DIS_WAITCONNEC << std::endl;
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

	std::cout << DIS_CONNECTED << std::endl;
	new_addr_size = sizeof new_addr;
	new_sd = accept(_sd, (sockaddr *)&new_addr, &new_addr_size); // accepte les connections entrantes, le nouveau fd sera pour recevoir et envoyer des appels
	_users.insert(std::pair<int, User*>(new_sd, new User(new_sd))); // pair first garder user_id ? Ou mettre le sd
	_pfds.push_back(pollfd());
	_pfds.back().fd = new_sd;
	_pfds.back().events = POLLIN;
	_fd_count++;
}

int Server::_disconnectUser(User *user, int ret) {
	std::string disconnection(" has been disconnected!");
	std::string delimiter("================================");

	if (!user->getCap())
		_sendError(user, ERR_NOCAP);
	else if (user->getTriedToAuth() && !user->getAuth())
		_sendError(user, ERR_PASSWDMISMATCH(user->getClient()));
	else if (user->getTriedToAuth() && user->getNick() == "")
		_sendError(user, ERR_NONICK);
	else if (user->getTriedToAuth() && user->getUserName() == "")
		_sendError(user, ERR_NOUSER);
	else if (user->getAuth()) {
		disconnection = " has left the server!";
		delimiter = "==============================";
	}
	else if (!user->getTriedToAuth())
		_sendError(user, ERR_NOPASS);
	std::cout << ORANGE BOLD "=========" RED << delimiter << RESET << std::endl;
	std::cout << ORANGE BOLD "[ircserv]" RED " Client " << user->getUserSd() << disconnection << std::endl;
	std::cout << ORANGE BOLD "=========" RED << delimiter << RESET << std::endl;
	std::vector<pollfd>::iterator it;
	for (it = _pfds.begin() + 1; it->fd != user->getUserSd(); it++)
		;
	user->removeFromAll();
	_pfds.erase(it);
	_users.erase(user->getUserSd());
	_recvs.clear();
	_fd_count--;
	close(user->getUserSd());
	delete user;
	return ret;
}

///////////// MANAGE REQUESTS /////////////////

int Server::_manageRequest(pollfd pfd) {
	int ret;
	int lines;
	int status;

	if ((ret = _recvAll(pfd)))
		return ret;
	_recvs.clear();
	lines = _fillRecvs(std::string(_buff));
	_buff.clear();
	for (int i = 0; i < lines; i++) {
		std::cout << DIS_RECV << pfd.fd << DIS_RECVEND(_recvs[i].first, _recvs[i].second) << std::endl;
		if ((status = _manageCmd(pfd, _recvs[i]))) {
			if (status == 2)
				break;
			else if (status == 3)
				_sendError(_users[pfd.fd], ERR_UNKNOWNCOMMAND(_users[pfd.fd]->getClient(), _recvs[i].first));
		}
	}
	return 0;
}

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

size_t Server::_recvAll(pollfd pfd) {
	char buffer[BUFFER_SIZE + 1];
	int size;

	while (1) {
		memset(buffer, 0, BUFFER_SIZE + 1);
		if ((size = recv(pfd.fd, buffer, BUFFER_SIZE, 0)) == -1)
			return -1;
		if (size == 0)
			return _disconnectUser(_users[pfd.fd], 0);
		buffer[size] = 0;
		_buff += buffer;
		if (_buff.find('\n') != _buff.npos)
			break;
	}
	return 0;
}
