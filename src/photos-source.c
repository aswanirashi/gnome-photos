/*
 * Photos - access, organize and share your photos on GNOME
 * Copyright © 2012, 2013, 2014 Red Hat, Inc.
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


#include "config.h"

#include <gio/gio.h>

#include "photos-filterable.h"
#include "photos-query-builder.h"
#include "photos-source.h"


struct _PhotosSourcePrivate
{
  GIcon *icon;
  GoaObject *object;
  gboolean builtin;
  gchar *id;
  gchar *name;
};

enum
{
  PROP_0,
  PROP_BUILTIN,
  PROP_ID,
  PROP_NAME,
  PROP_OBJECT
};

static void photos_filterable_interface_init (PhotosFilterableInterface *iface);


G_DEFINE_TYPE_WITH_CODE (PhotosSource, photos_source, G_TYPE_OBJECT,
                         G_ADD_PRIVATE (PhotosSource)
                         G_IMPLEMENT_INTERFACE (PHOTOS_TYPE_FILTERABLE,
                                                photos_filterable_interface_init));


static gchar *
photos_source_build_filter_resource (PhotosSource *self)
{
  PhotosSourcePrivate *priv = self->priv;
  gchar *filter;

  if (!priv->builtin)
    filter = g_strdup_printf ("(nie:dataSource (?urn) = '%s')", priv->id);
  else
    filter = g_strdup ("(false)");

  return filter;
}


static gchar *
photos_source_get_filter (PhotosFilterable *iface)
{
  PhotosSource *self = PHOTOS_SOURCE (iface);
  PhotosSourcePrivate *priv = self->priv;

  g_assert_cmpstr (priv->id, !=, PHOTOS_SOURCE_STOCK_ALL);

  if (g_strcmp0 (priv->id, PHOTOS_SOURCE_STOCK_LOCAL) == 0)
    return photos_query_builder_filter_local ();

  return photos_source_build_filter_resource (self);
}


static const gchar *
photos_source_get_id (PhotosFilterable *filterable)
{
  PhotosSource *self = PHOTOS_SOURCE (filterable);
  return self->priv->id;
}


static void
photos_source_dispose (GObject *object)
{
  PhotosSource *self = PHOTOS_SOURCE (object);
  PhotosSourcePrivate *priv = self->priv;

  g_clear_object (&priv->icon);
  g_clear_object (&priv->object);

  G_OBJECT_CLASS (photos_source_parent_class)->dispose (object);
}


static void
photos_source_finalize (GObject *object)
{
  PhotosSource *self = PHOTOS_SOURCE (object);
  PhotosSourcePrivate *priv = self->priv;

  g_free (priv->id);
  g_free (priv->name);

  G_OBJECT_CLASS (photos_source_parent_class)->finalize (object);
}


static void
photos_source_get_property (GObject *object, guint prop_id, GValue *value, GParamSpec *pspec)
{
  PhotosSource *self = PHOTOS_SOURCE (object);
  PhotosSourcePrivate *priv = self->priv;

  switch (prop_id)
    {
    case PROP_BUILTIN:
      g_value_set_boolean (value, priv->builtin);
      break;

    case PROP_ID:
      g_value_set_string (value, priv->id);
      break;

    case PROP_NAME:
      g_value_set_string (value, priv->name);
      break;

    case PROP_OBJECT:
      g_value_set_object (value, (gpointer) priv->object);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}


static void
photos_source_set_property (GObject *object, guint prop_id, const GValue *value, GParamSpec *pspec)
{
  PhotosSource *self = PHOTOS_SOURCE (object);
  PhotosSourcePrivate *priv = self->priv;

  switch (prop_id)
    {
    case PROP_BUILTIN:
      priv->builtin = g_value_get_boolean (value);
      break;

    case PROP_ID:
      priv->id = g_value_dup_string (value);
      break;

    case PROP_NAME:
      priv->name = g_value_dup_string (value);
      break;

    case PROP_OBJECT:
      {
        GoaAccount *account;
        const gchar *provider_icon;
        const gchar *provider_name;

        priv->object = GOA_OBJECT (g_value_dup_object (value));
        if (priv->object == NULL)
          break;

        account = goa_object_peek_account (priv->object);
        priv->id = g_strdup_printf ("gd:goa-account:%s", goa_account_get_id (account));

        provider_icon = goa_account_get_provider_icon (account);
        priv->icon = g_icon_new_for_string (provider_icon, NULL); /* TODO: use a GError */

        provider_name = goa_account_get_provider_name (account);
        priv->name = g_strdup (provider_name);
        break;
      }

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}


static void
photos_source_init (PhotosSource *self)
{
  self->priv = photos_source_get_instance_private (self);
}


static void
photos_source_class_init (PhotosSourceClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);

  object_class->dispose = photos_source_dispose;
  object_class->finalize = photos_source_finalize;
  object_class->get_property = photos_source_get_property;
  object_class->set_property = photos_source_set_property;

  g_object_class_install_property (object_class,
                                   PROP_BUILTIN,
                                   g_param_spec_boolean ("builtin",
                                                         "",
                                                         "",
                                                         FALSE,
                                                         G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));

  g_object_class_install_property (object_class,
                                   PROP_ID,
                                   g_param_spec_string ("id",
                                                        "",
                                                        "",
                                                        "",
                                                        G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));

  g_object_class_install_property (object_class,
                                   PROP_NAME,
                                   g_param_spec_string ("name",
                                                        "",
                                                        "",
                                                        "",
                                                        G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));

  g_object_class_install_property (object_class,
                                   PROP_OBJECT,
                                   g_param_spec_object ("object",
                                                        "GoaObject instance",
                                                        "A GOA configured account from which the source was created",
                                                        GOA_TYPE_OBJECT,
                                                        G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE));
}


static void
photos_filterable_interface_init (PhotosFilterableInterface *iface)
{
  iface->get_filter = photos_source_get_filter;
  iface->get_id = photos_source_get_id;
}


PhotosSource *
photos_source_new (const gchar *id, const gchar *name, gboolean builtin)
{
  return g_object_new (PHOTOS_TYPE_SOURCE, "id", id, "name", name, "builtin", builtin, NULL);
}


PhotosSource *
photos_source_new_from_goa_object (GoaObject *object)
{
  g_return_val_if_fail (GOA_IS_OBJECT (object), NULL);
  return g_object_new (PHOTOS_TYPE_SOURCE, "object", object, NULL);
}


const gchar *
photos_source_get_name (PhotosSource *self)
{
  return self->priv->name;
}


GoaObject *
photos_source_get_goa_object (PhotosSource *self)
{
  return self->priv->object;
}
