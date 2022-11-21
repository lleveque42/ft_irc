/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:07:23 by arudy             #+#    #+#             */
/*   Updated: 2022/11/21 19:36:32 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <csignal>
#include <fcntl.h>
#include <cerrno>
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

#define RPL_WELCOME(nickname) (":irc.server 001 " + std::string(nickname) + ":\r\n")
#define RPL_YOURHOST(nickname) (":irc.server 002 " + std::string(nickname) + " :Your host is irc.server, running version 1.0\r\n")
#define RPL_CREATED(nickname, date) (":irc.server 003 " + std::string(nickname) + " :This server was created " + std::string(date) + "\r\n")
#define RPL_MYINFO(nickname) (":irc.server 004 " + std::string(nickname) + " irc.server 1.0 io ikl\r\n")
#define RPL_PASS ": " GREEN "Connection accepted !\n" BOLD ORANGE "Welcome to our IRC server !\033[00m\r\n"
#define RPL_PASSSEND "Connection accepted: Password correct"
#define RPL_NICK(old_nick, nickname) (":" + std::string(old_nick) + " NICK " + std::string(nickname) + "\r\n")
#define RPL_QUIT(nickname, hostname, reason) (":" + std::string(nickname) + "@" + std::string(hostname) + " QUIT :" + std::string(reason) + "\r\n")

#define ERR_NOCAP ":400 \033[91mConnection refused: No cap provided\033[00m\r\n"
#define ERR_NOPASS ":400 \033[91mConnection refused: No password provided\r\n"
#define ERR_NONICK ":400 \033[91mConnection refused: No nickname provided, registration not completed\033[00m\r\n"
#define ERR_NOUSER ":400 \033[91mConnection refused: No user informations provided, registration not completed\033[00m\r\n"
#define ERR_UNKNOWNCOMMAND(cmd) ":421 \033[91m" + std::string(cmd) + ": Unknown command\033[00m\r\n"
#define ERR_NONICKNAMEGIVEN ":431 \033[91mNICK: No nickname provided\033[00m\r\n"
#define ERR_ERRONEUSNICKNAME ":432 \033[91mNICK: Erroneus nickname\033[00m\r\n"
#define ERR_NICKNAMEINUSE ":433 \033[91mNICK: Nickname is already in use\033[00m\r\n"
#define ERR_NEEDMOREPARAMS(cmd) (":461 \033[91m" + std::string(cmd) + ": Not enough parameters\033[00m\r\n")
#define ERR_NOPREFIX(cmd) (":461 \033[91m" + std::string(cmd) + ": No prefix before last param\033[00m\r\n")
#define ERR_ALREADYREGISTERED(cmd) ":462 \033[91mUSER: You may not reregister\033[00m\r\n"
#define ERR_PASSWDMISMATCH ":464 \033[91mConnection refused: Password incorrect\033[00m\r\n"

#define DIS_CREATED(date) (ORANGE BOLD "[ircserv]" RESET BOLD " created " + std::string(date) + RESET)
#define DIS_WAITCONNEC ORANGE BOLD "[ircserv]" RESET BOLD " waiting for incoming connections... 😴" RESET
#define DIS_SERVSETTEDUP ORANGE BOLD "[ircserv]" GREEN " setted up on port "
#define DIS_EMOJICHECK " \033[32m\xE2\x9C\x93\033[0m" RESET
#define DIS_CONNECTED ORANGE BOLD "=========" GREEN "=========================\n" ORANGE BOLD "[ircserv]" GREEN " new incoming connection!\n" ORANGE BOLD "=========" GREEN "=========================" RESET
#define DIS_RECV BLUE BOLD "[ircserv]" RESET BLUE " Recv    <--    " BLUE BOLD "[Client "
#define DIS_RECVEND(cmd, args) ("]" RESET BLUE ":    " + std::string(cmd) + " " + std::string(args) + RESET)
#define DIS_ERR RED BOLD "[ircserv]" RESET RED " Send    -->    " RED BOLD "[Client "
#define DIS_ERREND(msg) ("]" RESET RED ":    " + std::string(msg) + RESET)
#define DIS_EXEC GREEN BOLD "[ircserv]" RESET GREEN " Send    -->    " GREEN BOLD "[Client "
#define DIS_EXECEND(msg) ("]" RESET GREEN ":    " + std::string(msg) + RESET)

class Server;

typedef int (Server::*cmds)(User*, std::string);

class Server
{
	private:
		std::string				_creation_time;
		std::string				_password;
		char					*_port;
		int						_sd;
		std::map<int, User*>	_users;
		std::vector<pollfd>		_pfds;
		int						_fd_count;
		std::string				_buff;
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
		size_t	_recvAll(pollfd pfd);
		int		_sendError(User *user, std::string msg);
		int		_sendExecuted(User *user, std::string ret);
		int		_disconnectUser(User *user, int ret);
		int		_acceptConnection(User *user, std::pair<std::string, std::string> cmd);
		int		_registrationCompleted(User *user);
		bool	_validChars(std::string s);
		bool	_nickAlreadyUsed(User *current, std::string s);
		int		_pass(User *user, std::string args);
		int		_user(User *user, std::string args);
		int		_nick(User *user, std::string buff);
		int		_pong(User *user, std::string buff);
		int		_quit(User *user, std::string args);
};

std::string currentTime();
