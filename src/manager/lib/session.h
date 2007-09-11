/**
 * @file session.h
 * 
 * @brief Interface of session_t.
 * 
 */

/*
 * Copyright (C) 2007 Martin Willi
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#ifndef SESSION_H_
#define SESSION_H_

#include "request.h"
#include "response.h"
#include "controller.h"

typedef struct session_t session_t;

/**
 * @brief A session, identified by a session ID.
 *
 */
struct session_t {
	
	/**
	 * @brief Get the session ID of the session.
	 *
	 * @return				session ID
	 */
	char* (*get_sid)(session_t *this);
	
	/**
	 * @brief Add a controller instance to the session.
	 *
	 * @param controller	controller to add
	 */
	void (*add_controller)(session_t *this, controller_t *controller);
	
	/**
	 * @brief Process a request in this session.
	 *
	 * @param request		request to process
	 * @param response		response to send
	 */
	void (*process)(session_t *this, request_t *request, response_t *response);
	
	/**
	 * @brief Destroy the session_t.
	 *
	 * @param this 			calling object
	 */
	void (*destroy) (session_t *this);
};

/**
 * @brief Create a session.
 *
 * @param context			user defined session context instance
 */
session_t *session_create(context_t *context);

#endif /* SESSION_H_ */
