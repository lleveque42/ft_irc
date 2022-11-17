/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:08:12 by arudy             #+#    #+#             */
/*   Updated: 2022/11/17 12:06:35 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(char *port, char *password) : _password(password), _port(port)
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
	struct addrinfo *servinfo;
	struct addrinfo hints;
	int optval = 1; // Allows other sockets to bind to this port, unless there is an active listening socket bound to the port already.

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, _port, &hints, &servinfo)) // remplie servinfo
		throw Exception::getaddrinfo();
	if ((_sd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) // retourne un socket descriptor pour les appels systemes
		throw Exception::socket();
	setsockopt(_sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
	fcntl(_sd, F_SETFL, O_NONBLOCK);
	if (bind(_sd, servinfo->ai_addr, servinfo->ai_addrlen)) // binds le socket sur le host
		throw Exception::bind();
	freeaddrinfo(servinfo);
}

void test(int sig) {
	if (sig == SIGINT)
		std::cout << "SIGINT pressed!\n";
}

void Server::launch() {
	int new_sd; // peut etre a mettre dans user ? chaque nouvelle connexion accepté crée un nouveau fd donc 1 par user
	sockaddr_storage newAddr; // ou toutes les infos de la nouvelle connexion vont aller
	socklen_t newAddrSize; // sizeof sockaddr_storage
	int user_id = 0; // A virer

	std::cout << "======== LAUNCH ========\n";
	if (listen(_sd, 10)) // queue toutes les connections entrantes, 10 max (arbitraire ca pourrait etre 20 au max)
		throw Exception::listen();
	std::cout << "ircserv: waiting for connections..." << std::endl;

	std::cout << "main sd : " << _sd << std::endl;
	// while (user_id != 1) {
	while (1) {
		newAddrSize = sizeof newAddr;
		new_sd = accept(_sd, (sockaddr *)&newAddr, &newAddrSize); // accepte les connections entrantes, le nouveau fd sera pour recevoir et envoyer des appels
		if (new_sd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
			continue;
		else if (new_sd == -1) {
			std::cerr << "accept error\n";
			continue;
		}
		else {
			_users.insert(std::pair<int, User*>(user_id, new User(new_sd))); // pair first garder user_id ? Ou mettre le sd
			user_id++;
			std::cerr << "connection ok\n";
		}

		// fcntl(new_sd, F_SETFL, O_NONBLOCK);
		// if (poll()) {
			// manageEntry();
		// }
		for (std::map<int, User*>::iterator it = _users.begin(); it != _users.end(); it++)
			std::cout << "Int " << it->first << " | User sd " << it->second->getUserSd() << std::endl;
	}

}
