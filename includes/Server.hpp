/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:07:23 by arudy             #+#    #+#             */
/*   Updated: 2022/11/29 15:50:19 by arudy            ###   ########.fr       */
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

#define RPL_WELCOME(client, nickname) (":" + std::string(client) + " 001 " + std::string(nickname) + "\r\n")
#define RPL_YOURHOST(client, nickname) (":" + std::string(client) + " 002 " + std::string(nickname) + " :Your host is irc.server, running version 1.0\r\n")
#define RPL_CREATED(client, nickname, date) (":" + std::string(client) + " 003 " + std::string(nickname) + " :This server was created " + std::string(date) + "\r\n")
#define RPL_MYINFO(client, nickname) (":" + std::string(client) + " 004 " + std::string(nickname) + " irc.server 1.0 io kl\r\n")
#define RPL_PASS ": " GREEN "Connection accepted !\n" BOLD ORANGE "Welcome to our IRC server !\033[00m\r\n"
#define RPL_PASSSEND "Connection accepted: Password correct"
#define RPL_NICK(old_nick, nickname) (":" + std::string(old_nick) + " NICK " + std::string(nickname) + "\r\n")
#define RPL_QUIT(nickname, hostname, reason) (":" + std::string(nickname) + " QUIT :" + std::string(reason) + "\r\n")
#define RPL_JOIN(client, chan_name) (":" + std::string(client) + " JOIN :" + std::string(chan_name) + "\r\n")
#define RPL_UMODEIS(client, nickname, modes) (":" + std::string(client) + " 221 " + std::string(nickname) + " +" + modes + "\r\n")
#define RPL_WHOISUSER(client, nickuser, nickname, username, hostname, realname) (":" + std::string(client) + " 311 " + std::string(nickuser) + " " + std::string(nickname) + " " + std::string(username) + " " + std::string(hostname) + " * :" +std::string(realname) + "\r\n")
#define RPL_CHANNELMODEIS(client, nickname, chan_name, modes) (":" + std::string(client) + " 324 " + std::string(nickname) + " " + std::string(chan_name) + " +" + std::string(modes) + "\r\n")
#define RPL_NOTOPIC(client, nickname, chan_name) (":" + std::string(client) +" 331 " + std::string(nickname) + " " + std::string(chan_name) + " :No topic is set\r\n")
#define RPL_TOPIC(client, nickname, chan_name, topic) (":" + std::string(client) +" 332 " + std::string(nickname) + " " + std::string(chan_name) + " :" + std::string(topic) + "\r\n")

#define RPL_WHOREPLY(client, nickname, msg) (":" + std::string(client) + " 352 " + std::string(nickname) + " " + std::string(msg) + "\r\n")
#define RPL_ENDOFWHO(client, nickname, mask) (":" + std::string(client) + " 315 " + std::string(nickname) + " " + std::string(mask) + " :End of WHO list\r\n")

#define RPL_NAMEREPLY(client, nickname, chan_name, usernames) (":" + std::string(client) +" 353 " + std::string(nickname) + " = " + std::string(chan_name) + " :" + std::string(usernames) + "\r\n")
#define RPL_ENDOFNAMES(client, nickname, chan_name) (":" + std::string(client) +" 366 " + std::string(nickname) + " " + std::string(chan_name) + " :End of /NAMES list\r\n")
#define RPL_YOUREOPER(client, nickname) (":" + std::string(client) +" 381 " + std::string(nickname) + " :You are now an IRC Operator\r\n")
#define RPL_PARTMSG(client, nickname, chan_name, msg) (":" + std::string(client) + std::string(nickname) + " PART " + std::string(chan_name) + " :" + msg + "\r\n")
#define RPL_PARTNOMSG(client, nickname, chan_name) (":" + std::string(client) + std::string(nickname) + " PART " + std::string(chan_name) + " :" + "\r\n")

#define ERR_NOCAP ":irc.server 400 " RED "Connection refused: No cap provided" RESET "\r\n"
#define ERR_NOPASS ":irc.server 400 " RED "Connection refused: No password provided" RESET "\r\n"
#define ERR_NONICK ":irc.server 400 " RED "Connection refused: No nickname provided, registration not completed" RESET "\r\n"
#define ERR_NOUSER ":irc.server 400 " RED "Connection refused: No user informations provided, registration not completed" RESET "\r\n"
#define ERR_NOSUCHNICK(client, nickname, target) (":" + std::string(client) +  " 401 " + std::string(nickname) + RED " " + std::string(target) + RESET "\r\n")
#define ERR_NOSUCHCHANNEL(client, nickname, chan_name) (":" + std::string(client) + " 403 " + std::string(nickname) + RED + " " + std::string(chan_name) + " :No such channel" RESET "\r\n")
#define ERR_CANNOTSENDTOCHAN(client, nickname, target) (":" + std::string(client) + " 404 " RED + std::string(target) + " :Cannot send to channel" RESET "\r\n")
#define ERR_NORECIPIENT(client, nickname) (":" + std::string(client) + " 411 " + std::string(nickname) + RED " :No recipient given (PRIVMSG)" RESET "\r\n")
#define ERR_NOTEXTTOSEND(client, nickname) (":" + std::string(client) + " 412 " + std::string(nickname) + RED " :No text to send" RESET "\r\n")
#define ERR_UNKNOWNCOMMAND(client, nickname, cmd) (":" + std::string(client) +" 421 " + std::string(nickname) + RED + " " + std::string(cmd) + " :Unknown command" RESET "\r\n")
#define ERR_NONICKNAMEGIVEN(client, nickname) ":" + std::string(client) + " 431 " + std::string(nickname) + RED " NICK :No nickname provided" RESET "\r\n"
#define ERR_ERRONEUSNICKNAME(client, nickname) ":" + std::string(client) + " 432 " + std::string(nickname) + RED " NICK :Erroneus nickname" RESET "\r\n"
#define ERR_NICKNAMEINUSE(client, nickname) ":" + std::string(client) + " 433 " + std::string(nickname) + RED " NICK :Nickname is already in use" RESET "\r\n"
#define ERR_NOTONCHANNEL(client, nickname, chan_name) (":" + std::string(client) + " 442 " + std::string(nickname) + RED + " " + std::string(chan_name) + " :You're not on channel" RESET "\r\n"
#define ERR_NEEDMOREPARAMS(client, nickname, cmd) (":" + std::string(client) + " 461 " + std::string(nickname) + RED + " " + std::string(cmd) + " :Not enough parameters" RESET "\r\n")
#define ERR_NOPREFIX(client, nickname, cmd) (":" + std::string(client) + " 461 " + std::string(nickname) + RED + " " + std::string(cmd) + " :No prefix before last param" RESET "\r\n")
#define ERR_ALREADYREGISTERED(client, nickname, cmd) ":" + std::string(client) + " 462 " + std::string(nickname) + RED " USER: You may not reregister" RESET "\r\n"
#define ERR_PASSWDMISMATCH(client, nickname) ":" + std::string(client) + " 464 " + std::string(nickname) + RED " Connection refused: Password incorrect" RESET "\r\n"
#define ERR_CHANNELISFULL(client, nickname, chan_name) (":" + std::string(client) + " 471 " + std::string(nickname) + " " + std::string(chan_name) + " :Cannot join channel (+l)" RESET "\r\n")
#define ERR_NOPRIVILEGES(client, nickname) (":" + std::string(client) + " 481 " + std::string(nickname) + RED + " :Permission Denied - You're not an IRC operator" RESET "\r\n")
#define ERR_CHANOPRIVSNEEDED(client, nickname, chan_name) (":" + std::string(client) + " 482 " + std::string(nickname) + RED + " " + std::string(chan_name) + " :You're not a channel operator" RESET "\r\n")
#define ERR_BADCHANNELKEY(client, nickname, chan_name) (":" + std::string(client) + " 475 " + std::string(nickname) + " " + std::string(chan_name) + " :Cannot join channel (+k)" RESET "\r\n")
#define ERR_UMODEUNKNOWNFLAG(client, nickname) (":" + std::string(client) + " 501 " + std::string(nickname) + RED " :Uknown MODE flag" RESET "\r\n")
#define ERR_USERSDONTMATCH(client, nickname) (":" + std::string(client) + " 502 "  + std::string(nickname) + RED " :Cant change mode for other users" RESET "\r\n")

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
class User;

typedef int (Server::*cmds)(User*, std::string);

class Server {
	private:
		std::string				_creation_time;
		std::string				_password;
		char					*_port;
		int						_sd;
		std::map<int, User*>	_users;
		std::vector<pollfd>		_pfds;
		int						_fd_count;
		std::string				_op_name;
		std::string				_op_password;
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
		std::pair<std::string, std::string>	_strToPair(std::string buff);
		std::pair<std::string, std::string>	_splitPrivMsg(std::string buff);
		int		_sendError(User *user, std::string msg);
		int		_sendExecuted(User *user, std::string ret);
		int		_disconnectUser(User *user, int ret);
		int		_acceptConnection(User *user, std::pair<std::string, std::string> cmd);
		int		_registrationCompleted(User *user);
		bool	_validChars(std::string s);
		bool	_nickAlreadyUsed(User *current, std::string s);
		void	_sendJoinMsg(User *user, Channel *chan);
		void	_sendPrivMsg(User *sender, User *tagret, std::string chan_name, std::string msg, std::string cmd_type);
		void	_sendPartMsg(User *sender, std::map<std::string, User *> targets, std::string chan_name, std::string msg);
		int		_checkModes(User *user, Channel *chan, std::string key);
		int		_applyMode(User *user, std::string mode, bool value);
		int		_applyMode(User *user, Channel *channel, std::string buff, bool value);
		int		_pass(User *user, std::string args);
		int		_user(User *user, std::string args);
		int		_nick(User *user, std::string buff);
		int		_pong(User *user, std::string buff);
		int		_quit(User *user, std::string args);
		int		_mode(User *user, std::string buff);
		int		_join(User *user, std::string buff);
		int		_part(User *user, std::string buff);
		int		_privmsg(User *user, std::string buff);
		int		_notice(User *user, std::string buff);
		int		_whois(User *user, std::string buff);
		int		_who(User *user, std::string buff);
		int		_topic(User *user, std::string buff);
		int		_oper(User *user, std::string buff);
		int		_die(User *user, std::string buff);
};

std::string currentTime();
