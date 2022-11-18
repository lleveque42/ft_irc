/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/16 10:09:52 by arudy             #+#    #+#             */
/*   Updated: 2022/11/16 10:10:21 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/User.hpp"

User::User(int sd) : _user_sd(sd), _auth(0), _triedToAuth(0), _nick(), _user_name(),
_real_name(), _host_name()
{
}

User::~User()
{
}

int const &User::getUserSd() const {
	return _user_sd;
}

std::string const &User::getNick() const {
	return _nick;
}

std::string const &User::getUserName() const {
	return _user_name;
}

std::string const &User::getHostName() const {
	return _host_name;
}

std::string const &User::getRealName() const {
	return _real_name;
}

bool const &User::getAuth() const {
	return _auth;
}

bool const &User::getTriedToAuth() const {
	return _triedToAuth;
}

void User::setNick(std::string value) {
	_nick = std::string(value);
}

void User::setHostName(std::string value) {
	_host_name = std::string(value);
}

void User::setUserName(std::string value) {
	_user_name = std::string(value);
}

void User::setRealName(std::string value) {
	_real_name = std::string(value);
}

void User::setAuth(bool value) {
	_auth = value;
}

void User::setTriedToAuth(bool value) {
	_triedToAuth = value;
}

void	User::setNick(std::string nick)
{
	_nick = nick;
}
