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
}

Server::~Server()
{
	std::cout << "Destructor server\n";
	// Delete pair.second in _users
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
			acceptUser();
		std::vector<pollfd>::iterator ite = _pfds.end();
		for (std::vector<pollfd>::iterator it = _pfds.begin() + 1; it != ite; it++)
			if ((*it).revents == POLLIN)
				manageRequest(*it);
	}
}

void Server::acceptUser() {
	int newSd; // peut etre a mettre dans user ? chaque nouvelle connexion accepté crée un nouveau fd donc 1 par user
	sockaddr_storage newAddr; // ou toutes les infos de la nouvelle connexion vont aller
	socklen_t newAddrSize; // sizeof sockaddr_storage

	newAddrSize = sizeof newAddr;
	newSd = accept(_sd, (sockaddr *)&newAddr, &newAddrSize); // accepte les connections entrantes, le nouveau fd sera pour recevoir et envoyer des appels
	_users.insert(std::pair<int, User*>(newSd, new User(newSd))); // pair first garder user_id ? Ou mettre le sd
	_pfds.push_back(pollfd());
	_pfds.back().fd = newSd;
	_pfds.back().events = POLLIN;
	_fdCount++;
}

int Server::manageRequest(pollfd pfds) {
	int size;

	size = recv(pfds.fd, _buff, BUFFER_SIZE, 0);
	// if (size = 0)
		// delete client
	// if (_users[pfds.fd]->getAuth())
	std::cout << "1 buffer: " <<  _buff << std::endl;
}
