/**
 * @file database.c
 *
 * @brief Implementation of database_t.
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

#include "database.h"

#include <sqlite3.h>
#include <library.h>
#include <enumerator.h>


typedef struct private_database_t private_database_t;

/**
 * private data of database
 */
struct private_database_t {

	/**
	 * public functions
	 */
	database_t public;
	
	/**
	 * SQLite database handle
	 */
	sqlite3 *db;
};

/**
 * database enumerator implements enumerator_t
 */
typedef struct  {
	enumerator_t enumerator;
	sqlite3_stmt *stmt;
} db_enumerator_t;

/**
 * destroy a database enumerator
 */
static void db_enumerator_destroy(db_enumerator_t* this)
{
	sqlite3_finalize(this->stmt);
	free(this);
}

/**
 * create a database enumerator
 */
static enumerator_t *db_enumerator_create(bool(*enumerate)(db_enumerator_t*,...),
										 	 sqlite3_stmt *stmt)
{
	db_enumerator_t *this = malloc_thing(db_enumerator_t);
	this->enumerator.enumerate = (void*)enumerate;
	this->enumerator.destroy = (void*)db_enumerator_destroy;
	this->stmt = stmt;
	return &this->enumerator;
}

/**
 * enumerator function for empty enumerator
 */
static bool empty_enumerate(enumerator_t *enumerator, ...)
{
	return FALSE;
}

/**
 * create an empty enumerator
 */
static enumerator_t* empty_enumerator_create()
{
	enumerator_t *this = malloc_thing(enumerator_t);
	this->enumerate = empty_enumerate;
	this->destroy = (void*)free;
	return this;
}

/**
 * Implementation of database_t.login.
 */
static int login(private_database_t *this, char *username, char *password)
{
	sqlite3_stmt *stmt;
	int uid = 0;
	
	if (sqlite3_prepare_v2(this->db,
			"SELECT oid FROM users WHERE username = ? AND password = ?;",
			-1, &stmt, NULL) == SQLITE_OK)
	{
		if (sqlite3_bind_text(stmt, 1, username, -1, SQLITE_STATIC) == SQLITE_OK &&
			sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC) == SQLITE_OK &&
			sqlite3_step(stmt) == SQLITE_ROW)
		{
			uid = sqlite3_column_int(stmt, 0);
		}
		sqlite3_finalize(stmt);
	}
	return uid;
}

/**
 * enumerate function for gateway enumrator
 */
static bool gateway_enumerate(db_enumerator_t* e, ...)
{
	va_list args;
	int *id, *port;
	const char **name, **address;
	
	va_start(args, e);
	id = va_arg(args, typeof(id));
	name = va_arg(args, typeof(name));
	port = va_arg(args, typeof(port));
	address = va_arg(args, typeof(address));
	va_end(args);
	
	if (sqlite3_step(e->stmt) == SQLITE_ROW)
	{
		*id = sqlite3_column_int(e->stmt, 0);
		*name = sqlite3_column_text(e->stmt, 1);
		*port = sqlite3_column_int(e->stmt, 2);
		*address = sqlite3_column_text(e->stmt, 3);
		return TRUE;
	}
	return FALSE;
}

/**
 * Implementation of database_t.create_gateway_enumerator.
 */
static enumerator_t* create_gateway_enumerator(private_database_t *this, int user)
{
	sqlite3_stmt *stmt;
	
	if (sqlite3_prepare_v2(this->db,
			"SELECT gateways.oid AS gid, name, port, address FROM "
			"gateways, user_gateway AS ug ON gid = ug.gateway WHERE ug.user = ?;",
			-1, &stmt, NULL) == SQLITE_OK)
	{
		if (sqlite3_bind_int(stmt, 1, user) == SQLITE_OK)
		{
			return db_enumerator_create((void*)gateway_enumerate, stmt);
		}
		sqlite3_finalize(stmt);
	}
	return empty_enumerator_create();
}

/**
 * Implementation of database_t.destroy
 */
static void destroy(private_database_t *this)
{
	sqlite3_close(this->db);
	free(this);
}

/*
 * see header file
 */
database_t *database_create(char *dbfile)
{
	private_database_t *this = malloc_thing(private_database_t);
	
	this->public.login = (int(*)(database_t*, char *username, char *password))login;
	this->public.create_gateway_enumerator = (enumerator_t*(*)(database_t*,int))create_gateway_enumerator;
	this->public.destroy = (void(*)(database_t*))destroy;
	
	if (sqlite3_open(dbfile, &this->db) != SQLITE_OK)
	{
		destroy(this);
		return NULL;
	}
	return &this->public;
}

