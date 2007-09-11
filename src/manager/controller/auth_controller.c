/**
 * @file auth_controller.c
 *
 * @brief Implementation of auth_controller_t.
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

#include "auth_controller.h"
#include "../manager.h"

#include <template.h>

#include <library.h>


typedef struct private_auth_controller_t private_auth_controller_t;

/**
 * private data of the task manager
 */
struct private_auth_controller_t {

	/**
	 * public functions
	 */
	auth_controller_t public;
	
	/**
	 * manager instance
	 */
	manager_t *manager;
};

static void login(private_auth_controller_t *this,
				  request_t *request, response_t *response)
{
	template_t *t = template_create("templates/auth/login.cs");
	t->set(t, "action", "check");
	t->render(t, response);
	t->destroy(t);
}

static void check(private_auth_controller_t *this,
				  request_t *request, response_t *response)
{
	char *username, *password;
	
	username = request->get_post_data(request, "username");
	password = request->get_post_data(request, "password");
	if (username && password &&
		this->manager->login(this->manager, username, password))
	{
		response->redirect(response, "status/test");
	}
	else
	{
		response->redirect(response, "auth/login");
	}
}

static void logout(private_auth_controller_t *this,
				   request_t *request, response_t *response)
{
	this->manager->logout(this->manager);
	response->redirect(response, "auth/login");
}

/**
 * Implementation of controller_t.get_name
 */
static char* get_name(private_auth_controller_t *this)
{
	return "auth";
}

/**
 * Implementation of controller_t.get_handler
 */
static controller_handler_t get_handler(private_auth_controller_t *this, char *name)
{
	if (streq(name, "login")) return (controller_handler_t)login;
	if (streq(name, "check")) return (controller_handler_t)check;
	if (streq(name, "logout")) return (controller_handler_t)logout;
	return NULL;
}

/**
 * Implementation of controller_t.destroy
 */
static void destroy(private_auth_controller_t *this)
{
	free(this);
}

/*
 * see header file
 */
controller_t *auth_controller_create(context_t *context, void *param)
{
	private_auth_controller_t *this = malloc_thing(private_auth_controller_t);

	this->public.controller.get_name = (char*(*)(controller_t*))get_name;
	this->public.controller.get_handler = (controller_handler_t(*)(controller_t*,char*))get_handler;
	this->public.controller.destroy = (void(*)(controller_t*))destroy;
	
	this->manager = (manager_t*)context;
	
	return &this->public.controller;
}

