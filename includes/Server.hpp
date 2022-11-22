/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:07:23 by arudy             #+#    #+#             */
/*   Updated: 2022/11/22 16:16:49 by arudy            ###   ########.fr       */
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
#include <sstream>
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
#include "../includes/Channel.hpp"
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
#define RPL_MYINFO(nickname) (":irc.server 004 " + std::string(nickname) + " irc.server 1.0 io kl\r\n")
#define RPL_PASS ": " GREEN "Connection accepted !\n" BOLD ORANGE "Welcome to our IRC server !\033[00m\r\n"
#define RPL_PASSSEND "Connection accepted: Password correct"
#define RPL_NICK(old_nick, nickname) (":" + std::string(old_nick) + " NICK " + std::string(nickname) + "\r\n")
#define RPL_QUIT(nickname, hostname, reason) (":" + std::string(nickname) + " QUIT :" + std::string(reason) + "\r\n")
#define RPL_JOIN(nickname, chan_name) (":" + std::string(nickname) + " JOIN " + std::string(chan_name) + "\r\n")
#define RPL_TOPIC(chan_name, topic) (":irc.server 332 " + std::string(chan_name) + ": " + std::string(topic))
#define RPL_NAMEREPLY(nickname, chan_name, usernames) (":irc.server 353 " + std::string(nickname) + " " + " = " + std::string(chan_name) + " :" + std::string(usernames) + "\r\n")
#define RPL_ENDOFNAMES(nickname, chan_name) (":irc.server 366 " + std::string(nickname) + " " + std::string(chan_name) + " :End of NAMES list\r\n")

#define ERR_NOCAP ":irc.server 400 " RED "Connection refused: No cap provided" RESET "\r\n"
#define ERR_NOPASS ":irc.server 400 " RED "Connection refused: No password provided\r\n"
#define ERR_NONICK ":irc.server 400 " RED "Connection refused: No nickname provided, registration not completed" RESET "\r\n"
#define ERR_NOUSER ":irc.server 400 " RED "Connection refused: No user informations provided, registration not completed" RESET "\r\n"
#define ERR_UNKNOWNCOMMAND(cmd) (":irc.server 421 " RED + std::string(cmd) + ": Unknown command" RESET "\r\n")
#define ERR_NONICKNAMEGIVEN ":irc.server 431 " RED "NICK: No nickname provided" RESET "\r\n"
#define ERR_ERRONEUSNICKNAME ":irc.server 432 " RED "NICK: Erroneus nickname" RESET "\r\n"
#define ERR_NICKNAMEINUSE ":irc.server 433 " RED "NICK: Nickname is already in use" RESET "\r\n"
#define ERR_NEEDMOREPARAMS(cmd) (":irc.server 461 " RED + std::string(cmd) + ": Not enough parameters" RESET "\r\n")
#define ERR_NOPREFIX(cmd) (":irc.server 461 " RED + std::string(cmd) + ": No prefix before last param" RESET "\r\n")
#define ERR_ALREADYREGISTERED(cmd) ":irc.server 462 " RED "USER: You may not reregister" RESET "\r\n"
#define ERR_PASSWDMISMATCH ":irc.server 464 " RED "Connection refused: Password incorrect" RESET "\r\n"
#define ERR_BADCHANNELKEY(chan_name) (":irc.server 465 " RED + std::string(chan_name) + ": Cannot join channel (+k)" RESET "\r\n")
#define ERR_CHANNELISFULL(chan_name) (":irc.server 471 " + std::string(chan_name) + ": Cannot join channel (+l)" RESET "\r\n")

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
class Channel;

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
		std::vector<std::pair<std::string, std::string> >	_recvs;
		std::map<std::string, Channel *>					_channels;
		std::map<const std::string, cmds>					_cmds;


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
		void	_sendJoinMsg(User *user, Channel *chan);
		int		_checkModes(User *user, Channel *chan, std::string key);
		int		_pass(User *user, std::string args);
		int		_user(User *user, std::string args);
		int		_nick(User *user, std::string buff);
		int		_pong(User *user, std::string buff);
		int		_quit(User *user, std::string args);
		int		_mode(User *user, std::string buff);
		int		_join(User *user, std::string buff);
		int		_privmsg(User *user, std::string buff);
};

std::string currentTime();
