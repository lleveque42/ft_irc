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

Server::Server(char *port, char *password) : _password(password), _port(port), _pfds(), _fdCount(0)
{
	std::cout << "Server port : " << _port << std::endl;
	std::cout << "Server password : " << _password << std::endl;
	_initCmd();
}

Server::~Server()
{
	std::cout << "Destructor server\n";
	// Delete pair.second in _users
}

void Server::_initCmd() {
	_cmds["PASS"] = &Server::_pass;
}

void Server::setup()
{
	std::cout << "======== SETUP ========\n";
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
	_fdCount = 1;
	std::cout << "setup completed!" << std::endl;
}

void test(int sig) {
	if (sig == SIGINT)
		std::cout << "SIGINT pressed!\n";
}

void Server::launch() {
	std::cout << "======== LAUNCH ========\n";
	std::cout << "ircserv: waiting for connections..." << std::endl;
	while (1) {
		if (poll(&_pfds[0], _fdCount, -1) == -1)
			throw Exception::poll();
		if (_pfds[0].revents == POLLIN)
			_acceptUser();
		std::vector<pollfd>::iterator ite = _pfds.end();
		for (std::vector<pollfd>::iterator it = _pfds.begin() + 1; it != ite; it++)
			if ((*it).revents == POLLIN)
				_manageRequest(*it);
	}
}

void Server::_acceptUser() {
	int new_sd; // peut etre a mettre dans user ? chaque nouvelle connexion accepté crée un nouveau fd donc 1 par user
	sockaddr_storage new_addr; // ou toutes les infos de la nouvelle connexion vont aller
	socklen_t new_addr_size; // sizeof sockaddr_storage

	new_addr_size = sizeof new_addr;
	new_sd = accept(_sd, (sockaddr *)&new_addr, &new_addr_size); // accepte les connections entrantes, le nouveau fd sera pour recevoir et envoyer des appels
	_users.insert(std::pair<int, User*>(new_sd, new User(new_sd))); // pair first garder user_id ? Ou mettre le sd
	_pfds.push_back(pollfd());
	_pfds.back().fd = new_sd;
	_pfds.back().events = POLLIN;
	_fdCount++;
}

int Server::_fillRecvs(std::string buff) {
	std::string::iterator begin;
	std::string::iterator space;
	std::string::iterator backr;
	int lines = std::count(buff.begin(), buff.end(), '\n');

	for (int i = 0; i < lines; i++) {
		begin = buff.begin();
		space = begin + buff.find(' ');
		backr = begin + buff.find('\r');
		_recvs.push_back(std::make_pair(std::string(begin, space), std::string(space + 1, backr)));
		buff.erase(begin, backr + 2);
	}
	return lines;
}

int Server::_manageRequest(pollfd pfd) {
	int size;
	int lines;

	size = recv(pfd.fd, _buff, BUFFER_SIZE, 0);
	// if (size = 0)
		// delete client
	lines = _fillRecvs(std::string(_buff));
	// for (size_t i = 0; i < _recvs.size(); i++)
		// std::cout << "first: " << _recvs[i].first << " || second: " << _recvs[i].second << std::endl;
	for (int i = 0; i < lines; i++) {
		std::cout << GREEN BOLD "[Server]" RESET GREEN " Recv    <--    " GREEN BOLD "[Client " << pfd.fd << "]" RESET GREEN ":    " << _recvs[i].first << " " << _recvs[i].second << RESET << std::endl;
		_manageCmd(pfd, _recvs[i]);
		if (_recvs[i].first == "CAP" && _recvs[i].second == "LS") // checker cap
			continue;
		// if (_users[pfd.fd]->getAuth())
			// _disconnectUser(pfd);
	}
	(void)size;
	_recvs.clear();
	return 0;
}

int Server::_manageCmd(pollfd pfd, std::pair<std::string, std::string> cmd) {
	if (_cmds.find(cmd.first) != _cmds.end())
		(this->*_cmds[cmd.first])(pfd, cmd.second);
	return 0;
}

/////////////////////////////////////

int	Server::_pass(pollfd pfd, std::string arg) {
	if (arg == _password)
		send(pfd.fd, _buff, sizeof _buff, MSG_CONFIRM);
	(void)arg;
	(void)pfd;
	return 0;
}
