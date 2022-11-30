/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:25:06 by arudy             #+#    #+#             */
/*   Updated: 2022/11/30 10:40:00 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"

Channel::Channel(std::string name) : _name(name), _limited(false, size_t()),
_key(false, std::string()), _topic(false, std::string()) {
}

Channel::~Channel(){
}

std::string const &Channel::getName() const {
	return _name;
}

void Channel::addUser(User *user) {
	_users.insert(std::make_pair(user->getNick(), user));
	user->addChannel(this);
}

void Channel::removeUser(User *user) {
	if (user->isOp(this))
		user->removeOps(this);
	_users.erase(user->getNick());
	user->removeChannel(this);
}

std::string	Channel::getUsersList() {
	std::string list;
	std::map<std::string, User *>::iterator it = _users.begin();

	for (; it != _users.end(); it++) {
		if (it != _users.begin())
			list += " ";
		if (isOp(it->second))
			list += "@";
		list += it->second->getNick();
	}
	return list;
}

std::string Channel::getModes() {
	std::string ret;

	if (_key.first)
		ret += "k";
	if (_limited.first)
		ret += "l";
	if (_key.first)
		ret += (" " + _key.second);
	if (_limited.first) {
		std::stringstream ss;
		ss << " " << _limited.second;
		ret += ss.str();
	}
	return ret;
}

bool Channel::isOp(User *user) {
	return _opers.count(user->getNick());
}

std::map<std::string, User *> Channel::getUsers() const {
	return _users;
}

std::pair<bool, size_t> Channel::getLimited() const {
	return _limited;
}

std::pair<bool, std::string> Channel::getKey() const {
	return _key;
}

std::pair<bool, std::string> Channel::getTopic() const {
	return _topic;
}

void Channel::addToOp(User *user) {
	_opers.insert(std::make_pair(user->getNick(), user));
	user->addOps(this);
}

void Channel::removeFromOp(User *user) {
	_opers.erase(user->getNick());
	user->removeOps(this);
}

void Channel::setLimited(bool value, size_t n) {
	_limited.first = value;
	_limited.second = n;
}

void Channel::setKey(bool value, std::string key) {
	_key.first = value;
	_key.second = key;
}

void Channel::setTopic(bool value, std::string topic) {
	_topic.first = value;
	_topic.second = topic;
}
