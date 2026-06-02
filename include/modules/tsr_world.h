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
}	t_tsr_tile;

typedef struct s_tsr_world_chunk	t_tsr_chunk;
struct s_tsr_world_chunk
{
	t_tsr_tile_id	tiles[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	t_tsr_chunk		*neighbors[6];
};

typedef struct s_world
{
	t_vec3i			size;
	t_tsr_tile_id	*blocks;
}	t_world;

t_tsr_tile_id
*tsr_get_tile_ptr(t_tsr_chunk *chunk, t_vec3i pos);

bool
tsr_fix_tile_pos(t_tsr_chunk **chunk, t_vec3i *pos);

t_tsr_tile
*tsr_get_tile(t_tsr *tsr, t_tsr_chunk *chunk, t_vec3i pos);

#endif
