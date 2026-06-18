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

#include "tsr.h"
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
	const int	face = tsr->player.face.z;

	if (chunk >= tsr->world.chunk_count)
	{
		chunk = tsr_new_chunk(&tsr->world);
		if (!chunk)
			return ;
	}
	tsr->world.chunks[tsr->player.chunk].neighbors[face] = chunk;
	if (flags == CHUNK_CONNECT_FRONT_ONEWAY)
		return ;
	if (chunk)
		tsr->world.chunks[chunk].neighbors[face ^ 1] = tsr->player.chunk;
	if (flags == CHUNK_CONNECT_FRONT)
		return ;
	link_neighbor_chunks(tsr, tsr->player.chunk, chunk, tsr->player.face);
}

static bool	prompt_inputs(t_tsr *tsr)
{
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_C)
		&& mbx_key_held(tsr->mbx, MBX_KEY_LCTRL))
	{
		tsr->player.prompt_state = PROMPT_STATE_CHUNK;
		prompt_init(tsr, vec2i(240, 200), "Link to Chunk");
	}
	else if (mbx_key_pressed(tsr->mbx, MBX_KEY_B))
	{
		tsr->player.prompt_state = PROMPT_STATE_TP;
		prompt_init(tsr, vec2i(240, 200), "TP to Chunk");
	}
	else if (mbx_key_pressed(tsr->mbx, MBX_KEY_M)
		&& (mbx_key_held(tsr->mbx, MBX_KEY_LCTRL)
			|| !tsr->world_data.name[0]))
	{
		tsr->player.prompt_state = PROMPT_STATE_SAVE;
		prompt_init(tsr, vec2i(240, 200), "Save location");
	}
	else
		return (false);
	return (true);
}

void	player_inputs(t_tsr *tsr)
{
	if (prompt_inputs(tsr))
		return ;
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_G))
		tsr->extras.show_chunks = !tsr->extras.show_chunks;
	else if (mbx_key_pressed(tsr->mbx, MBX_KEY_C))
		create_chunk(tsr, -1, CHUNK_DEFAULT);
	else if (mbx_key_pressed(tsr->mbx, MBX_KEY_V))
		tsr->world.chunks[tsr->player.chunk]
			.limits[tsr->player.face.z] = tsr->player.hotbar_tile;
	else if (mbx_key_held(tsr->mbx, MBX_KEY_LCTRL)
		&& mbx_key_pressed(tsr->mbx, MBX_KEY_N))
		trash_chunks(tsr);
	else if (mbx_key_pressed(tsr->mbx, MBX_KEY_M))
		save_map(tsr, tsr->world_data.name);
}
