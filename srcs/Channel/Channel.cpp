/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:25:06 by arudy             #+#    #+#             */
/*   Updated: 2022/11/22 16:20:28 by arudy            ###   ########.fr       */
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
	_users.erase(user->getNick());
	user->removeChannel(this);
}

std::string	Channel::getUsersList() {
	std::string list;
	std::map<std::string, User *>::iterator it = _users.begin();

	if (isOp(it->second))
		list += "@";
	list += it->second->getNick();
	it++;
	for (; it != _users.end(); it++) {
		list += " ";
		if (isOp(it->second))
			list += "@";
		list += it->second->getNick();
	}
	return list;
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
<<<<<<< HEAD
=======
	user->addOps(this);
>>>>>>> 9336dd569a37f558743f1d15dda9e93f125b8138
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
