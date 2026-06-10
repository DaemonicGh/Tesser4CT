/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 03:43:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/10 03:43:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	create_chunk(t_tsr *tsr, t_tsr_chunk_id id)
{
	const t_vec3	dir = vec3_abs(tsr->camera.forward);
	int				axis;

	if (dir.z > dir.x && dir.z > dir.y)
		axis = 2;
	else
		axis = (dir.y > dir.x);
	if (id >= tsr->world.chunk_count)
	{
		id = tsr_new_chunk(&tsr->world);
		if (!id)
			return ;
	}
	tsr->world.chunk_refs[tsr->player.chunk].neighbors[
		axis * 2 + (tsr->camera.forward.v[axis] > 0)] = id;
}

static void	update_chunk_prompt(t_tsr *tsr)
{
	t_tsr_chunk_id	id;
	uint32_t		i;

	if (!prompt_update(tsr))
		return ;
	i = 0;
	id = 0;
	while (i < tsr->ui.prompt.cursor
		&& tsr->ui.prompt.buffer[i] >= '0'
		&& tsr->ui.prompt.buffer[i] <= '9')
		id = id * 10 + tsr->ui.prompt.buffer[i++] - '0';
	if (i < tsr->ui.prompt.cursor)
	{
		tsr->ui.prompt.is_error = true;
		tsr->ui.prompt.shake = 1;
		return ;
	}
	tsr->ui.prompt.visible = false;
	tsr->ui.prompt_state = PROMPT_STATE_NONE;
	create_chunk(tsr, id);
}

static void	update_save_prompt(t_tsr *tsr)
{
	uint32_t	i;

	if (!prompt_update(tsr) || tsr->ui.prompt.cursor == 0)
		return ;
	i = 0;
	while (i < tsr->ui.prompt.cursor)
	{
		tsr->ui.prompt.buffer[i] = tsr_tolower(tsr->ui.prompt.buffer[i]);
		i++;
	}
	if (!save_map(tsr, tsr->ui.prompt.buffer))
	{
		tsr->ui.prompt.is_error = true;
		tsr->ui.prompt.shake = 1;
		return ;
	}
	tsr->ui.prompt.visible = false;
	tsr->ui.prompt_state = PROMPT_STATE_NONE;
}

static void	update_inputs(t_tsr *tsr)
{
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_P))
	{
		if (mbx_key_held(tsr->mbx, MBX_KEY_LALT))
		{
			tsr->ui.prompt_state = PROMPT_STATE_SAVE;
			prompt_init(tsr, vec2i(240, 200), "Save location");
		}
		else
			save_map(tsr, tsr->world_data.name);
	}
	else if (mbx_key_pressed(tsr->mbx, MBX_KEY_EQUALS))
	{
		if (mbx_key_held(tsr->mbx, MBX_KEY_LALT))
		{
			tsr->ui.prompt_state = PROMPT_STATE_CHUNK;
			prompt_init(tsr, vec2i(240, 200), "Chunk ID");
		}
		else
			create_chunk(tsr, -1);
	}
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_ESCAPE))
	{
		tsr->ui.prompt_state = PROMPT_STATE_NONE;
		tsr->ui.prompt.visible = false;
	}
}

bool	tsr_player_update_prompt(t_tsr *tsr)
{
	if (tsr->ui.prompt_state == PROMPT_STATE_CHUNK)
		update_chunk_prompt(tsr);
	else if (tsr->ui.prompt_state == PROMPT_STATE_SAVE)
		update_save_prompt(tsr);
	else
	{
		update_inputs(tsr);
		return (false);
	}
	return (true);
}
