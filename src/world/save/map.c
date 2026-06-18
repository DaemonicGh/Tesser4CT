/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 14:05:29 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/04 14:05:29 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static bool	append_vec3(t_mlem_value *object, t_mlem_string key, t_vec3 vec)
{
	t_mlem_value	value;

	value = mlem_array(3,
			mlem_float(vec.x), mlem_float(vec.y), mlem_float(vec.z));
	if (!value.type)
		return (false);
	if (!mlem_object_append(object, key, value))
	{
		mlem_destroy(value);
		return (false);
	}
	return (true);
}

static t_mlem_value	save_attributes(t_tsr *tsr)
{
	t_mlem_value	object;

	object = mlem_object_empty(10);
	if (!object.type)
		return ((t_mlem_value){0});
	mlem_object_append(&object, "skybox", mlem_reference(
			tsr->world_data.mlem.objectv.value[
			tsr->world_data.skybox.type].value));
	append_vec3(&object, "light_color",
		tsr->world_data.skylight_color);
	append_vec3(&object, "light_direction",
		vec3_neg(tsr->world_data.skylight));
	mlem_object_append(&object, "origin",
		mlem_int(tsr->world_data.origin - 1));
	mlem_object_append(&object, "player_chunk",
		mlem_int(tsr->player.chunk - 1));
	append_vec3(&object, "player_position", tsr->player.position);
	append_vec3(&object, "player_rotation", tsr->camera.rotation);
	return (object);
}

static t_mlem_value	create_map_data(t_tsr *tsr, t_mlem_string name)
{
	t_mlem_value	map;
	t_mlem_value	value;

	map = mlem_object_empty(3);
	if (!map.type)
		return ((t_mlem_value){0});
	mlem_object_append(&map, "map_id", mlem_string(name));
	value = save_attributes(tsr);
	if (!value.type)
	{
		mlem_destroy_ex(map, false, false, true);
		return ((t_mlem_value){0});
	}
	mlem_object_append(&map, "attributes", value);
	value = save_chunks(tsr);
	if (!value.type)
	{
		mlem_destroy_ex(map, false, false, true);
		return ((t_mlem_value){0});
	}
	mlem_object_append(&map, "chunks", value);
	return (map);
}

bool	save_map(t_tsr *tsr, t_mlem_string name)
{
	char			file[PROMPT_SIZE
		+ MAP_SELECT_PREFIX_LEN + MAP_SELECT_SUFFIX_LEN];
	t_mlem_value	map;
	size_t			i;
	size_t			j;

	map = create_map_data(tsr, name);
	if (!map.type)
		return (false);
	i = 0;
	j = 0;
	while (j < MAP_SELECT_PREFIX_LEN)
		file[i++] = MAP_SELECT_PREFIX[j++];
	j = 0;
	while (name[j])
		file[i++] = name[j++];
	j = 0;
	while (j < MAP_SELECT_SUFFIX_LEN)
		file[i++] = MAP_SELECT_SUFFIX[j++];
	file[i] = 0;
	mlem_print_to_file(file, map);
	mlem_destroy_ex(map, false, false, true);
	tsr_report_m(STATUS_INFO, REPORT_MAPSAVED, file);
	return (true);
}
