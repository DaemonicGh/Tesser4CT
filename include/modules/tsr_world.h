/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_world.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:20:44 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/30 17:30:31 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_WORLD_H
# define TSR_WORLD_H

# include "tsr_core.h"

typedef enum e_direction
{
	EST,
	WEST,
	UP,
	DOWN,
	NORTH,
	SOUTH
}	t_direction;

typedef struct s_tsr_tile_data
{
	const char		*keys[6];
	t_mbx_region	*region[6];
	bool			skip_process;
	bool			skybox;
}	t_tsr_tile;

static const t_tsr_tile		g_tile_data[TILE_COUNT] = {
{.keys = {0, 0, 0, 0, 0, 0}, .skip_process = true},
{.keys = {"stone", "stone", "stone", "stone", "stone", "stone"}},
{.keys = {"dirt", "dirt", "dirt", "dirt", "dirt", "dirt"}},
{.keys = {"oak_planks", "oak_planks", "oak_planks",
	"oak_planks", "oak_planks", "oak_planks"}},
{.keys = {"iron_block", "iron_block", "iron_block",
	"iron_block", "iron_block", "iron_block"}},
{.keys = {"moss_block", "moss_block", "moss_block",
	"moss_block", "moss_block", "moss_block"}},
{.keys = {"skybox_est", "skybox_west", "skybox_top",
		"skybox_down", "skybox_north", "skybox_south"
	}, .skybox = true},
{.keys = {"glass", "glass", "dirt", "glass", "glass", "glass"}},
{.keys = {"white_glass", "white_glass", "white_glass",
	"white_glass", "white_glass", "white_glass"}},
{.keys = {"grass_block_side", "grass_block_side", "grass_block_top",
	"dirt", "grass_block_side", "grass_block_side"}},
{.keys = {"oak_leaves", "oak_leaves", "oak_leaves",
	"oak_leaves", "oak_leaves", "oak_leaves"}},
{.keys = {"oak_log_side", "oak_log_side", "oak_log_top",
	"oak_log_top", "oak_log_side", "oak_log_side"}},
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
