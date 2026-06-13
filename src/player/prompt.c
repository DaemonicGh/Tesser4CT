/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 03:43:06 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/11 17:04:19 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static bool	parse_chunk_prompt(t_tsr *tsr,
	t_tsr_chunk_id *id, int *flags)
{
	uint32_t	i;

	i = 0;
	while (i < tsr->ui.prompt.cursor
		&& tsr->ui.prompt.buffer[i] >= '0'
		&& tsr->ui.prompt.buffer[i] <= '9')
		*id = *id * 10 + tsr->ui.prompt.buffer[i++] - '0';
	while (i < tsr->ui.prompt.cursor)
	{
		if (tsr->ui.prompt.buffer[i] == '^' && *flags > 0)
			*flags -= 1;
		else if (tsr->ui.prompt.buffer[i] != ' ')
			return (false);
		i++;
	}
	return (true);
}

static void	update_chunk_prompt(t_tsr *tsr)
{
	t_tsr_chunk_id	id;
	int				flags;

	id = 0;
	flags = CHUNK_DEFAULT;
	if (!parse_chunk_prompt(tsr, &id, &flags))
	{
		tsr->ui.prompt.is_error = true;
		tsr->ui.prompt.shake = 1;
		return ;
	}
	tsr->ui.prompt.visible = false;
	tsr->player.prompt_state = PROMPT_STATE_NONE;
	create_chunk(tsr, id, flags);
}

static void	update_tp_prompt(t_tsr *tsr)
{
	t_tsr_chunk_id	id;
	uint32_t		i;

	id = 0;
	i = 0;
	while (i < tsr->ui.prompt.cursor
		&& tsr->ui.prompt.buffer[i] >= '0'
		&& tsr->ui.prompt.buffer[i] <= '9')
		id = id * 10 + tsr->ui.prompt.buffer[i++] - '0';
	if (id == 0 || id >= tsr->world.chunk_count)
	{
		tsr->ui.prompt.is_error = true;
		tsr->ui.prompt.shake = 1;
		return ;
	}
	tsr->ui.prompt.visible = false;
	tsr->player.prompt_state = PROMPT_STATE_NONE;
	tsr->player.chunk = id;
}

static void	update_save_prompt(t_tsr *tsr)
{
	uint32_t	i;

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
	tsr->player.prompt_state = PROMPT_STATE_NONE;
}

void	player_update_prompt(t_tsr *tsr)
{
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_ESCAPE))
	{
		tsr->player.prompt_state = PROMPT_STATE_NONE;
		tsr->ui.prompt.visible = false;
		return ;
	}
	if (!prompt_update(tsr) || tsr->ui.prompt.cursor == 0)
		return ;
	if (tsr->player.prompt_state == PROMPT_STATE_CHUNK)
		update_chunk_prompt(tsr);
	else if (tsr->player.prompt_state == PROMPT_STATE_TP)
		update_tp_prompt(tsr);
	else if (tsr->player.prompt_state == PROMPT_STATE_SAVE)
		update_save_prompt(tsr);
}
