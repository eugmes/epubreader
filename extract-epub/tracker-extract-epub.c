/* Copyright © 2010 Євгеній Мещеряков <eugen@debian.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <string.h>

#include <glib.h>

#include "tracker-main.h"

/* From tracker-os-dependant.h */
gboolean tracker_spawn(gchar **argv, gint timeout, gchar **tmp_stdout, gint *exit_status);

static void parse_data(const gchar *data, GHashTable *metadata)
{
    const gchar *p = data;

    while (*p) {
        while (*p == '\n')
            p++;
        if (!*p)
            break;

        const gchar *key_start = p;
        const gchar *key_end = strchr(p, '\n');
        if (!key_end)
            break;

        p = key_end + 1;
        if (!*p)
            break;

        const gchar *val_start = p;
        const gchar *val_end = strchr(p, '\n');
        if (!val_end)
            val_end = val_start + strlen(val_start); // Points to NUL
        p = val_end;

        gchar *val = g_strndup(val_start, val_end - val_start);

        g_hash_table_insert(metadata,
                            g_strndup(key_start, key_end - key_start),
                            tracker_escape_metadata(val));
        g_free(val);
    }
}

static void extract_epub(const gchar *filename, GHashTable *metadata)
{
    gchar *argv[3];
    gchar *data;

    argv[0] = g_strdup("epub-extract-metadata");
    argv[1] = g_strdup(filename);
    argv[2] = NULL;

    if (tracker_spawn(argv, 10, &data, NULL)) {
        parse_data(data, metadata);
    }

    g_free(argv[0]);
    g_free(argv[1]);
    g_free(argv[2]);
}

static TrackerExtractData extract_data[] ={
    {"application/epub+zip", extract_epub},
    {NULL, NULL}
};

TrackerExtractData *tracker_get_extract_data(void)
{
    return extract_data;
}
