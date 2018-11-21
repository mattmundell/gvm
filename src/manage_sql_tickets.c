/* GVM
 * $Id$
 * Description: GVM management layer SQL: Tickets.
 *
 * Authors:
 * Matthew Mundell <matthew.mundell@greenbone.net>
 *
 * Copyright:
 * Copyright (C) 2018 Greenbone Networks GmbH
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/**
 * @file manage_sql_tickets.c
 * @brief GVM management layer: Ticket SQL
 *
 * The Ticket SQL for the GVM management layer.
 */

#include "manage_tickets.h"
#include "manage_sql_tickets.h"
#include "manage_sql.h"
#include "sql.h"

#undef G_LOG_DOMAIN
/**
 * @brief GLib log domain.
 */
#define G_LOG_DOMAIN "md manage"

/**
 * @brief Filter columns for ticket iterator.
 */
#define TICKET_ITERATOR_FILTER_COLUMNS                                         \
 { GET_ITERATOR_FILTER_COLUMNS, "host",                                        \
   NULL }

/**
 * @brief Ticket iterator columns.
 */
#define TICKET_ITERATOR_COLUMNS                             \
 {                                                          \
   GET_ITERATOR_COLUMNS (tickets),                          \
   { "host", NULL, KEYWORD_TYPE_STRING },                   \
   { NULL, NULL, KEYWORD_TYPE_UNKNOWN }                     \
 }

/**
 * @brief Ticket iterator columns for trash case.
 */
#define TICKET_ITERATOR_TRASH_COLUMNS                               \
 {                                                                  \
   GET_ITERATOR_COLUMNS (tickets_trash),                            \
   { "host", NULL, KEYWORD_TYPE_STRING },                           \
   { NULL, NULL, KEYWORD_TYPE_UNKNOWN }                             \
 }

/**
 * @brief Count number of tickets.
 *
 * @param[in]  get  GET params.
 *
 * @return Total number of tickets in filtered set.
 */
int
ticket_count (const get_data_t *get)
{
  static const char *extra_columns[] = TICKET_ITERATOR_FILTER_COLUMNS;
  static column_t columns[] = TICKET_ITERATOR_COLUMNS;
  static column_t trash_columns[] = TICKET_ITERATOR_TRASH_COLUMNS;

  return count ("ticket", get, columns, trash_columns, extra_columns, 0, 0, 0,
                TRUE);
}

/**
 * @brief Initialise a ticket iterator.
 *
 * @param[in]  iterator    Iterator.
 * @param[in]  get         GET data.
 *
 * @return 0 success, 1 failed to find ticket, 2 failed to find filter,
 *         -1 error.
 */
int
init_ticket_iterator (iterator_t *iterator, const get_data_t *get)
{
  static const char *filter_columns[] = TICKET_ITERATOR_FILTER_COLUMNS;
  static column_t columns[] = TICKET_ITERATOR_COLUMNS;
  static column_t trash_columns[] = TICKET_ITERATOR_TRASH_COLUMNS;

  return init_get_iterator (iterator,
                            "ticket",
                            get,
                            columns,
                            trash_columns,
                            filter_columns,
                            0,
                            NULL,
                            NULL,
                            TRUE);
}

/**
 * @brief Get the host from a ticket iterator.
 *
 * @param[in]  iterator  Iterator.
 *
 * @return Host of the ticket or NULL if iteration is complete.
 */
DEF_ACCESS (ticket_iterator_host, GET_ITERATOR_COLUMN_COUNT);

/**
 * @brief Return whether a ticket is in use.
 *
 * @param[in]  ticket  Ticket.
 *
 * @return 1 if in use, else 0.
 */
int
ticket_in_use (ticket_t ticket)
{
  return 0;
}

/**
 * @brief Return whether a trashcan ticket is in use.
 *
 * @param[in]  ticket  Ticket.
 *
 * @return 1 if in use, else 0.
 */
int
trash_ticket_in_use (ticket_t ticket)
{
  return 0;
}

/**
 * @brief Return whether a ticket is writable.
 *
 * @param[in]  ticket  Ticket.
 *
 * @return 1 if writable, else 0.
 */
int
ticket_writable (ticket_t ticket)
{
  return 1;
}

/**
 * @brief Return whether a trashcan ticket is writable.
 *
 * @param[in]  ticket  Ticket.
 *
 * @return 1 if writable, else 0.
 */
int
trash_ticket_writable (ticket_t ticket)
{
  return trash_ticket_in_use (ticket) == 0;
}
