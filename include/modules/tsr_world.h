/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_world.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:20:44 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/13 18:54:00 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_WORLD_H
# define TSR_WORLD_H

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
	const char		*keys[6];
	t_tsr_texture	texture[6];
	bool			skip_process;
	bool			backface;
	bool			inner_backface;
	bool			skybox;
	bool			specular;
}	t_tsr_tile;

static const t_tsr_tile		g_tile_data[TILE_BUFFER_COUNT] = {
{.keys = {0, 0, 0, 0, 0, 0}, .skip_process = true},
{.keys = {"skybox_west", "skybox_east", "skybox_down",
	"skybox_up", "skybox_north", "skybox_south"}, .skybox = true},
{.keys = {"stone"}},
{.keys = {"dirt"}},
{.keys = {"grass_block_side", "grass_block_side", "grass_block_top",
	"dirt", "grass_block_side", "grass_block_side"}},
{.keys = {"oak_leaves"}, .backface = true, .inner_backface = true},
{.keys = {"oak_log", "oak_log", "oak_log_top",
	"oak_log_top", "oak_log", "oak_log"}},
{.keys = {"oak_planks"}},
{.keys = {"stone_bricks"}},
{.keys = {"cobblestone"}},
{.keys = {"iron_block"}, .specular = true},
{.keys = {"moss"}},
{.keys = {"glass"}, .backface = true, .specular = true},
{.keys = {"white_stained_glass"}, .backface = true, .specular = true},
{.keys = {"red_stained_glass"}, .backface = true, .specular = true},
{.keys = {"green_stained_glass"}, .backface = true, .specular = true},
{.keys = {"blue_stained_glass"}, .backface = true, .specular = true},
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
