/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_world.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:20:44 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/13 10:41:18 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_WORLD_H
# define TSR_WORLD_H

# include "tsr_core.h"
# include "tsr_pbr.h"

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
	const char	*keys[6];
	t_pbr		pbr[6];
	bool		skip_process;
	bool		backface;
	bool		inner_backface;
	bool		skybox;
}	t_tsr_tile;

static const t_tsr_tile		g_tile_data[TILE_BUFFER_COUNT] = {
{.keys = {0, 0, 0, 0, 0, 0}, .skip_process = true},
{.keys = {"stone"}},
{.keys = {"dirt"}},
{.keys = {"oak_planks"}},
{.keys = {"iron_block"}},
{.keys = {"moss_block"}},
{.keys = {"skybox_west", "skybox_east", "skybox_down",
	"skybox_up", "skybox_north", "skybox_south"}, .skybox = true},
{.keys = {"glass", "glass", "glass", "glass", "glass", "glass"},
	.backface = true},
{.keys = {"white_glass"}, .backface = true},
{.keys = {"grass_block_side", "grass_block_side", "grass_block_top",
	"dirt", "grass_block_side", "grass_block_side"}},
{.keys = {"oak_leaves"}, .backface = true, .inner_backface = true},
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
