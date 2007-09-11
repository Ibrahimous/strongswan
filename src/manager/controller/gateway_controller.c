/**
 * @file gateway_controller.c
 *
 * @brief Implementation of gateway_controller_t.
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

#include "gateway_controller.h"
#include "../manager.h"
#include "../gateway.h"

#include <template.h>

#include <library.h>


typedef struct private_gateway_controller_t private_gateway_controller_t;

/**
 * private data of the gateway_controller
 */
struct private_gateway_controller_t {

	/**
	 * public functions
	 */
	gateway_controller_t public;
	
	/**
	 * manager instance
	 */
	manager_t *manager;
	
};

static void list(private_gateway_controller_t *this,
				 request_t *request, response_t *response)
{
	template_t *t;
	enumerator_t *enumerator;
	char *name, *address;
	int id, port;
	
	t = template_create("templates/gateway/list.cs");
	enumerator = this->manager->create_gateway_enumerator(this->manager);
	while (enumerator->enumerate(enumerator, &id, &name, &port, &address))
	{
		t->setf(t, "gateways.%d.name=%s", id, name);
		if (port)
		{
			t->setf(t, "gateways.%d.address=tcp://%s:%d", id, address, port);
		}
		else
		{
			t->setf(t, "gateways.%d.address=unix://%s", id, address);
		}
	}
	enumerator->destroy(enumerator);
	t->set(t, "action", "select");
	t->render(t, response);
	t->destroy(t);
}

static void _select(private_gateway_controller_t *this,
					request_t *request, response_t *response)
{
	char *id;
	
	id = request->get_post_data(request, "gateway");
	if (id)
	{
		if (this->manager->select_gateway(this->manager, atoi(id)))
		{
			response->redirect(response, "status/ikesalist");
			return;
		}
	}
	response->printf(response, "selecting dings failed: %s", id);
}

/**
 * redirect to authentication login
 */
static void login(private_gateway_controller_t *this,
				  request_t *request, response_t *response)
{
	response->redirect(response, "auth/login");
}

/**
 * Implementation of controller_t.get_name
 */
static char* get_name(private_gateway_controller_t *this)
{
	return "gateway";
}

/**
 * Implementation of controller_t.get_handler
 */
static controller_handler_t get_handler(private_gateway_controller_t *this, char *name)
{
	if (!this->manager->logged_in(this->manager)) return (controller_handler_t)login;
	if (streq(name, "list")) return (controller_handler_t)list;
	if (streq(name, "select")) return (controller_handler_t)_select;
	return NULL;
}

/**
 * Implementation of controller_t.destroy
 */
static void destroy(private_gateway_controller_t *this)
{
	free(this);
}

/*
 * see header file
 */
controller_t *gateway_controller_create(context_t *context, void *param)
{
	private_gateway_controller_t *this = malloc_thing(private_gateway_controller_t);

	this->public.controller.get_name = (char*(*)(controller_t*))get_name;
	this->public.controller.get_handler = (controller_handler_t(*)(controller_t*,char*))get_handler;
	this->public.controller.destroy = (void(*)(controller_t*))destroy;
	
	this->manager = (manager_t*)context;
	
	return &this->public.controller;
}

