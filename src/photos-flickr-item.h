/*
 * Photos - access, organize and share your photos on GNOME
 * Copyright © 2012 Red Hat, Inc.
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

/* Based on code from:
 *   + Documents
 */

#ifndef PHOTOS_FLICKR_ITEM_H
#define PHOTOS_FLICKR_ITEM_H

#include <tracker-sparql.h>

#include "photos-base-item.h"

G_BEGIN_DECLS

#define PHOTOS_TYPE_FLICKR_ITEM (photos_flickr_item_get_type ())

#define PHOTOS_FLICKR_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), \
   PHOTOS_TYPE_FLICKR_ITEM, PhotosFlickrItem))

#define PHOTOS_FLICKR_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), \
   PHOTOS_TYPE_FLICKR_ITEM, PhotosFlickrItemClass))

#define PHOTOS_IS_FLICKR_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), \
   PHOTOS_TYPE_FLICKR_ITEM))

#define PHOTOS_IS_FLICKR_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), \
   PHOTOS_TYPE_FLICKR_ITEM))

#define PHOTOS_FLICKR_ITEM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), \
   PHOTOS_TYPE_FLICKR_ITEM, PhotosFlickrItemClass))

typedef struct _PhotosFlickrItem        PhotosFlickrItem;
typedef struct _PhotosFlickrItemClass   PhotosFlickrItemClass;
typedef struct _PhotosFlickrItemPrivate PhotosFlickrItemPrivate;

struct _PhotosFlickrItem
{
  PhotosBaseItem parent_instance;
  PhotosFlickrItemPrivate *priv;
};

struct _PhotosFlickrItemClass
{
  PhotosBaseItemClass parent_class;
};

GType               photos_flickr_item_get_type           (void) G_GNUC_CONST;

PhotosBaseItem     *photos_flickr_item_new                (TrackerSparqlCursor *cursor);

G_END_DECLS

#endif /* PHOTOS_LOCAL_ITEM_H */
