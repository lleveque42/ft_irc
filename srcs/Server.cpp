/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:08:12 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 18:14:32 by lleveque         ###   ########.fr       */
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
}

void Server::setup()
{
	std::cout << "======== SETUP ========\n";
	struct addrinfo *servinfo;
	struct addrinfo hints;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, _port, &hints, &servinfo)) // remplie servinfo
		throw Exception::getaddrinfo();
	// for (;;) {
	// if ((_sd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)))
	if ((_sd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) // retourne un socket descriptor pour les appels systemes
		throw Exception::socket();
	fcntl(_sd, F_SETFL, O_NONBLOCK);
	if (bind(_sd, servinfo->ai_addr, servinfo->ai_addrlen)) // binds le socket sur le host
		throw Exception::bind();
	// }
	freeaddrinfo(servinfo);
}

void test(int sig) {
	if (sig == SIGINT)
		std::cout << "SIGINT pressed!\n";
}

void Server::launch() {
	int newSd; // peut etre a mettre dans user ? chaque nouvelle connexion accepté crée un nouveau fd donc 1 par user
	sockaddr_storage newAddr; // ou toutes les infos de la nouvelle connexion vont aller
	socklen_t newAddrSize; // sizeof sockaddr_storage

	std::cout << "======== LAUNCH ========\n";
	if (listen(_sd, 10)) // queue toutes les connections entrantes, 10 max (arbitraire ca pourrait etre 20 au max)
		throw Exception::listen();
	std::cout << "ircserv: waiting for connections..." << std::endl;
	// std::signal(SIGINT, test);
	// std::signal(SIGQUIT, test);
	while (1) {
		newAddrSize = sizeof newAddr;
		newSd = accept(_sd, (sockaddr *)&newAddr, &newAddrSize); // accepte les connections entrantes, le nouveau fd sera pour recevoir et envoyer des appels
		if (newSd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
			continue;
		else if (newSd == -1) {
			std::cerr << "accept error\n";
			continue;
		}
		else {
			std::cerr << "connection ok\n";
			std::cout << newAddr.__ss_padding << std::endl;
			std::cout << newAddr.__ss_align << std::endl;
			std::cout << newAddr.ss_family << std::endl;
		}
		fcntl(newSd, F_SETFL, O_NONBLOCK);
		// if (poll()) {
			// manageEntry();
		// }
	}
}
