/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arudy <arudy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:35:57 by arudy             #+#    #+#             */
/*   Updated: 2022/11/22 15:50:04 by arudy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/Channel.hpp"
#include "../../includes/Server.hpp"
#include "../../includes/User.hpp"

static std::vector<std::pair<std::string, std::string> > strtovec(std::string buff) {
	std::string keys;
	std::string tmp_key;
	std::string channels;
	std::string tmp_channel;
	size_t space = buff.find(' ');
	std::vector<std::pair<std::string, std::string> > args;

	if (space != buff.npos) {
		channels = std::string(buff.begin(), buff.begin() + space);
		keys = std::string(buff.begin() + space + 1, buff.end());
	}
	else
		channels = std::string(buff.begin(), buff.end());
	std::istringstream channels_buffstream(channels);
	std::istringstream keys_buffstream(keys);
	while(std::getline(channels_buffstream, tmp_channel, ',')) {
		if (space != buff.npos)
			std::getline(keys_buffstream, tmp_key, ',');
		args.push_back(std::make_pair(tmp_channel, tmp_key));
	}
	return args;
}

void Server::_sendJoinMsg(User *user, Channel *chan) {
	_sendExecuted(user, RPL_JOIN(user->getNick(), chan->getName()));
	if (chan->getTopic().first)
		_sendExecuted(user, RPL_TOPIC(user->getNick(), chan->getTopic().second));
	_sendExecuted(user, RPL_NAMEREPLY(user->getNick(), chan->getName(), chan->getUsersList()));
	_sendExecuted(user, RPL_ENDOFNAMES(user->getNick(), chan->getName()));
}

// manage 0 to disco user from all chans ?

int Server::_checkModes(User *user, Channel *new_chan, std::string key) {
	if (new_chan->getKey().first && new_chan->getKey().second != key)
		return _sendError(user, ERR_BADCHANNELKEY(new_chan->getName()));
	if (new_chan->getLimited().first && new_chan->getLimited().second == new_chan->getUsers().size())
		return _sendError(user, ERR_CHANNELISFULL(new_chan->getName()));
	return 0;
}

int Server::_join(User *user, std::string buff) {
	Channel *new_chan;
	std::vector<std::pair<std::string, std::string> > args = strtovec(buff);

	for (std::vector<std::pair<std::string, std::string> >::iterator it = args.begin(); it != args.end(); it++)
		std::cout << "Args : | " << it->first << " | " << it->second << "\n";

	for (std::vector<std::pair<std::string, std::string> >::iterator it = args.begin(); it != args.end(); it++) {
		if (!_channels.count(it->first)) {
			new_chan = new Channel(it->first);
			_channels.insert(std::pair<std::string, Channel *>(it->first, new_chan));
			new_chan->addToOp(user);
			// std::cout << "New chan\n";
		}
		else {
			new_chan = _channels[it->first];
			// std::cout << "Already exist chan : " << *it << std::endl;
		}
		if (_checkModes(user, new_chan, it->second))
			continue;
		new_chan->addUser(user);
		_sendJoinMsg(user, new_chan);
	}
	return 0;
}
