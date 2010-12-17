/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2008, Nokia
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301, USA.
 */

#ifndef __TRACKER_ESCAPE_H__
#define __TRACKER_ESCAPE_H__

#include <glib.h>

#define SHOULD_VALIDATE_UTF8

G_BEGIN_DECLS

#ifdef SHOULD_VALIDATE_UTF8

gchar *tracker_escape_metadata        (const gchar *str);
gchar *tracker_escape_metadata_printf (const gchar *format,
				       ...);

#else  /* SHOULD_VALIDATE_UTF8 */

/* We used to escape strings before we used DBus */
#define tracker_escape_metadata        g_strdup
#define tracker_escape_metadata_printf g_strdup_printf

#endif /* SHOULD_VALIDATE_UTF8 */

G_END_DECLS

#endif /* __TRACKER_ESCAPE_H__ */
