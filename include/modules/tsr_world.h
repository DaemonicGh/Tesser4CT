/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_world.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:20:44 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/29 16:42:10 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_WORLD_H
# define TSR_WORLD_H

# include "tsr_core.h"

typedef struct s_tsr_tile_data
{
	const char		*texture_path;
	t_mbx_region	*region;
	bool			skip_process;
	bool			skybox;
}	t_tsr_tile;

static const t_tsr_tile		g_tile_data[TILE_COUNT] = {
{.texture_path = NULL, .skip_process = true},
{.texture_path = "assets/tiles/stone.png"},
{.texture_path = "assets/tiles/dirt.png"},
{.texture_path = "assets/tiles/oak_planks.png"},
{.texture_path = "assets/tiles/iron_block.png"},
{.texture_path = "assets/tiles/moss_block.png"},
{.texture_path = "assets/tiles/testeract1.png", .skybox = true},
{.texture_path = "assets/tiles/glass.png"},
{.texture_path = "assets/tiles/white_glass.png"},
{0}
};

typedef struct s_world
{
	t_vec3i			size;
	t_tsr_tile_id	*blocks;
}	t_world;

void
world_create(t_world *world, t_vec3i size);

void
world_destroy(t_world *world);

bool
inbound(t_world *world, t_vec3i pos);

t_tsr_tile_id
block_get(t_world *world, t_vec3i pos);

void
block_set(t_world *world, t_vec3i pos, t_tsr_tile_id type);

#endif
