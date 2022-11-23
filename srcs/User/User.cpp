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

#include "../../includes/User.hpp"

User::User(int sd) : _user_sd(sd), _auth(0), _tried_to_auth(0), _first_try(1),
_cap(0), _invisible(0), _nick(), _user_name(), _real_name(), _host_name() {
}

User::~User() {
}

int const &User::getUserSd() const {
	return _user_sd;
}

std::string const User::getClient() const {
	if (getFirstTry())
		return "";
	return (getNick() + "!" + getUserName() + "@" + getHostName());
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
	return _tried_to_auth;
}

bool const &User::getFirstTry() const {
	return _first_try;
}

bool const &User::getCap() const {
	return _cap;
}

bool User::isOp(Channel *channel) {
	return _ops.count(channel->getName());
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
	_tried_to_auth = value;
}

void User::setFirstTry(bool value) {
	_first_try = value;
}

void User::setCap(bool value) {
	_cap = value;
}

void User::addChannel(Channel *channel) {
	_joined.insert(std::make_pair(channel->getName(), channel));
}

void User::addOps(Channel *channel) {
	_ops.insert(std::make_pair(channel->getName(), channel));
}

void User::removeChannel(Channel *channel) {
	_joined.erase(channel->getName());
}

void User::removeOps(Channel *channel) {
	_ops.erase(channel->getName());
}

void User::removeFromAll() {
	for (std::map<std::string, Channel *>::iterator it = _joined.begin();; it++) {
		if (isOp(it->second))
			it->second->removeFromOp(this);
		it->second->removeUser(this);
		if (!_joined.size())
			break;
	}
}

