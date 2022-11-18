/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:07:23 by arudy             #+#    #+#             */
/*   Updated: 2022/11/18 17:54:11 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <csignal>
#include <fcntl.h>
#include <cerrno>
#include <map>
#include <map>
#include <vector>
#include <utility>
#include <poll.h>
#include <algorithm>
#include "../includes/Exception.hpp"
#include "../includes/User.hpp"

#define BUFFER_SIZE 4096
#define BOLD "\033[1m"
#define RESET "\033[0m"
#define UL "\033[4m"
#define TAB "\t"
#define SPACE "  "
#define ORANGE "\033[93m"
#define RED "\033[91m"
#define BLUE "\033[96m"
#define GREEN "\033[92m"
#define PINK "\033[95m"
#define STAR "\033[92m * \033[0m"

class Server;

typedef int (Server::*cmds)(pollfd, std::string);

class Server
{
	private:
		std::string				_password;
		char					*_port;
		int						_sd;
		std::map<int, User*>	_users;
		std::vector<pollfd>		_pfds;
		int						_fd_count;
		char					_buff[BUFFER_SIZE + 1];
		std::vector<std::pair<std::string, std::string> > _recvs;
		std::map<const std::string, cmds> _cmds;


	public:
		Server(char *port, char *password);
		~Server();
		void	setup();
		void	launch();
		void	clear();

	private:
		void	_initCmd();
		void	_acceptUser();
		int		_manageRequest(pollfd pfds);
		int		_fillRecvs(std::string buff);
		int		_manageCmd(pollfd pfd, std::pair<std::string, std::string> cmd);
		int		_sendAll(int fd, const char *buf, size_t len, int flags);
		int		_pass(pollfd pfds, std::string args);
		int		_user(pollfd pfds, std::string args);
		int		_nick(pollfd pfds, std::string buff);
		int		_disconnectUser(pollfd pfd, int ret);
		bool	_validChars(std::string s);
		bool	_nickAlreadyUsed(User *current, std::string s);
		// void	_serverLogs(pollfd pfds);
		// void	_serverLogs(pollfd pfds);

};
