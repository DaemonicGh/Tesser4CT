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

#include "tsr_core.h"
# include "tsr_textures.h"

typedef enum e_direction
{
	WEST,
	EST,
	DOWN,
	UP,
	NORTH,
	SOUTH
}	t_direction;

typedef struct s_tsr_tile_data
{
	t_tsr_texture	*texture[6];
	bool			skip;
	bool			backface;
	bool			inner_backface;
	bool			skybox;
	bool			specular;
}	t_tsr_tile_data;

typedef struct s_tsr_tile
{
	t_tsr_tile_id	type;
	uint8_t			light;
	uint8_t			orientation;
}	t_tsr_tile;

typedef struct s_tsr_chunk_ref
{
	uint64_t		process;
	uint16_t		neighbors[6];
}	t_tsr_chunk_ref;

typedef struct s_tsr_chunk
{
	t_tsr_tile		tiles[64];
	uint16_t		id;
}	t_tsr_chunk;

t_tsr_chunk
*tsr_get_chunk_neighbor(t_tsr *tsr, t_tsr_chunk *chunk, int side);

t_tsr_chunk
*tsr_relocate_chunk(t_tsr *tsr, t_tsr_chunk *chunk, t_vec3i *pos);

t_tsr_tile
tsr_tile(t_tsr_tile_id id, uint8_t orientation);

int
tsr_get_tile_index(t_vec3i pos);

bool
tsr_set_tile(t_tsr *tsr, t_tsr_chunk *chunk, t_vec3i pos, t_tsr_tile tile);

t_tsr_tile
*tsr_get_tile(t_tsr *tsr, t_tsr_chunk *chunk, t_vec3i pos);

#endif
