/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 22:01:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/11 22:01:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_constants.h"
#include "tsr.h"
#include "tsr_core.h"
#include "tsr_player.h"
#include "tsr_world.h"

static void	link_neighbor_chunks(t_tsr *tsr,
	t_tsr_chunk_id chunk1, t_tsr_chunk_id chunk2, t_vec3i face)
{
	const int		faces[4] = {face.x, face.x ^ 1, face.y, face.y ^ 1};
	size_t			i;
	t_tsr_chunk_id	chunk;

	i = 0;
	while (i < 4)
	{
		chunk = tsr->world.chunks[chunk1].neighbors[faces[i]];
		if (chunk)
			chunk = tsr->world.chunks[chunk].neighbors[face.z];
		if (chunk)
		{
			tsr->world.chunks[chunk].neighbors[faces[i] ^ 1] = chunk2;
			if (chunk2)
				tsr->world.chunks[chunk2].neighbors[faces[i]] = chunk;
		}
		i++;
	}
}

void	create_chunk(t_tsr *tsr, t_tsr_chunk_id chunk, int flags)
{
	const int		face = tsr->player.face.z;
	t_tsr_chunk_id	neighbor;

	if (chunk >= tsr->world.chunk_count)
	{
		chunk = tsr_new_chunk(&tsr->world);
		if (!chunk)
			return ;
	}
	neighbor = tsr->world.chunks[tsr->player.chunk].neighbors[face];
	tsr->world.chunks[tsr->player.chunk].neighbors[face] = chunk;
	if (flags == CHUNK_CONNECT_FRONT_ONEWAY)
		return ;
	tsr->world.chunks[neighbor].neighbors[face ^ 1] = 0;
	if (chunk)
		tsr->world.chunks[chunk].neighbors[face ^ 1] = tsr->player.chunk;
	if (flags == CHUNK_CONNECT_FRONT)
		return ;
	link_neighbor_chunks(tsr, tsr->player.chunk, chunk, tsr->player.face);
}

static void	player_rmb_tools(t_tsr *tsr)
{
	if (tsr->player.hotbar_tool == TOOL_CREATE_CHUNK)
	{
		tsr->player.prompt_state = PROMPT_STATE_CHUNK;
		prompt_init(tsr, vec2i(240, 200), "Link to Chunk");
	}
	else if (tsr->player.hotbar_tool == TOOL_SET_LIMIT)
		tsr->world_data.skybox = tsr->player.hotbar_tile;
	else if (tsr->player.hotbar_tool == TOOL_TRASH_CHUNKS)
		trash_chunks(tsr);
}

static void	player_lmb_tools(t_tsr *tsr)
{
	if (tsr->player.hotbar_tool == TOOL_CREATE_CHUNK)
		create_chunk(tsr, -1, CHUNK_DEFAULT);
	else if (tsr->player.hotbar_tool == TOOL_HIGHLIGHT_LIMITS)
		tsr->extras.show_chunks = !tsr->extras.show_chunks;
	else if (tsr->player.hotbar_tool == TOOL_SET_LIMIT)
		tsr->world.chunks[tsr->player.chunk]
			.limits[tsr->player.face.z] = tsr->player.hotbar_tile;
	else if (tsr->player.hotbar_tool == TOOL_TELEPORT)
	{
		tsr->player.prompt_state = PROMPT_STATE_TP;
		prompt_init(tsr, vec2i(240, 200), "TP to Chunk");
	}
	else if (tsr->player.hotbar_tool == TOOL_SKYLIGHT)
		tsr->world_data.skylight = vec3_neg(tsr->camera.forward);
}

void	player_tools(t_tsr *tsr)
{
	const bool	left = mbx_key_pressed(tsr->mbx, MBX_MOUSE_LEFT);
	const bool	right = mbx_key_pressed(tsr->mbx, MBX_MOUSE_RIGHT);

	if (mbx_key_pressed(tsr->mbx, MBX_MOUSE_MIDDLE))
		tsr->player.hotbar_tool = TOOL_CREATE_CHUNK;
	else if (tsr->player.hotbar_tool == TOOL_SAVE_MAP)
	{
		if (right || (left && !tsr->world_data.name[0]))
		{
			tsr->player.prompt_state = PROMPT_STATE_SAVE;
			prompt_init(tsr, vec2i(240, 200), "Save location");
		}
		else if (left)
			save_map(tsr, tsr->world_data.name);
	}
	else if (left)
		player_lmb_tools(tsr);
	else if (right)
		player_rmb_tools(tsr);
}
