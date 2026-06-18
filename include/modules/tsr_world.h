/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_world.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:20:44 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/27 15:37:08 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_WORLD_H
# define TSR_WORLD_H

# include "tsr_core.h"

typedef struct s_tsr_chunk_trash_data
{
	t_tsr_chunk_id	new_id;
	bool			explored;
}	t_tsr_trash_data;

t_tsr_chunk_id
tsr_relocate_chunk(const t_tsr_world *world,
	t_tsr_chunk_id chunk, t_vec3i *pos);

t_tsr_chunk_id
tsr_new_chunk(t_tsr_world *world);

t_tsr_tile
tsr_tile(t_tsr_tile_id id, uint8_t face, uint8_t roll);


t_tsr_tile
tsr_tile_r(t_tsr_tile_id id, uint8_t rotation);

t_tsr_tile
tsr_tile_l(t_tsr_tile_id id, uint8_t rotation, uint8_t light);

uint8_t
tsr_get_tile_texture(t_tsr_tile *tile, uint8_t face);

int
tsr_get_tile_index(t_vec3i pos);

bool
tsr_set_tile(const t_tsr_world *world,
	t_tsr_chunk_id chunk, t_vec3i pos, t_tsr_tile tile);

t_tsr_tile
*tsr_get_tile(const t_tsr_world *world, t_tsr_chunk_id chunk, t_vec3i pos);

void
clear_light(t_tsr *tsr);

void
cast_illumination_rays(t_tsr *tsr);

void
update_light(t_tsr *tsr);

void
load_tile_data(t_tsr *tsr);

void
prepare_chunk_data(t_tsr *tsr, t_mlem_value array);

void
load_chunk_data(t_tsr *tsr, t_mlem_value object);

bool
load_map(t_tsr *tsr);

t_mlem_value
save_chunks(t_tsr *tsr);

bool
save_map(t_tsr *tsr, t_mlem_string name);

void
trash_chunks(t_tsr *tsr);

#endif
