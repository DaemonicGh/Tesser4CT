/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chunk.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/11 02:14:00 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/11 02:14:00 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	create_chunk(t_tsr *tsr, t_tsr_chunk_id id, bool one_way)
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
	if (id && !one_way)
		tsr->world.chunk_refs[id].neighbors[
			axis * 2 + (tsr->camera.forward.v[axis] <= 0)] = tsr->player.chunk;
}

static bool	parse_chunk_prompt(t_tsr *tsr, t_tsr_chunk_id *id, bool *one_way)
{
	uint32_t	i;

	i = 0;
	*id = 0;
	*one_way = false;
	while (i < tsr->ui.prompt.cursor)
	{
		if (tsr->ui.prompt.buffer[i] == '-')
			*one_way = true;
		else if (tsr->ui.prompt.buffer[i] >= '0'
			&& tsr->ui.prompt.buffer[i] <= '9')
			*id = *id * 10 + tsr->ui.prompt.buffer[i] - '0';
		else
			return (false);
		i++;
	}
	return (true);
}

void	update_chunk_prompt(t_tsr *tsr)
{
	t_tsr_chunk_id	id;
	bool			one_way;

	if (!prompt_update(tsr) || tsr->ui.prompt.cursor == 0)
		return ;
	if (!parse_chunk_prompt(tsr, &id, &one_way))
	{
		tsr->ui.prompt.is_error = true;
		tsr->ui.prompt.shake = 1;
		return ;
	}
	tsr->ui.prompt.visible = false;
	tsr->ui.prompt_state = PROMPT_STATE_NONE;
	create_chunk(tsr, id, one_way);
}
