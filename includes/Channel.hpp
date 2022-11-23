/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lleveque <lleveque@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/21 15:26:42 by arudy             #+#    #+#             */
/*   Updated: 2022/11/23 21:20:18 by lleveque         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Exception.hpp"
#include "../includes/Server.hpp"
#include "../includes/User.hpp"

class User;

class Channel {
	private:
		std::string						_name;
		std::pair<bool, size_t>			_limited;
		std::pair<bool, std::string>	_key;
		std::pair<bool, std::string>	_topic;
		std::map<std::string, User *>	_users;
		std::map<std::string, User *>	_opers; // keep map ?

	public:
		Channel(std::string name);
		~Channel();
		std::string const &getName() const;
		void addUser(User *user);
		void removeUser(User *user);
		bool isOp(User *user);
		std::string getUsersList();
		std::string getModes();
		std::map<std::string, User *> getUsers() const;
		std::pair<bool, size_t> getLimited() const;
		std::pair<bool, std::string> getKey() const;
		std::pair<bool, std::string> getTopic() const;
		void addToOp(User *user);
		void removeFromOp(User *user);
		void setLimited(bool value, size_t n);
		void setKey(bool value, std::string key);
		void setTopic(bool value, std::string topic);

};
