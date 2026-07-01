/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 17:16:20 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/11 22:00:25 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_inputs.h"
#include "tsr.h"

static void	raycast(t_tsr *tsr)
{
	tsr->player.ray = setup_ray(tsr,
			tsr->camera.position, tsr->camera.chunk, tsr->camera.forward);
	trace_ray(tsr, &tsr->player.ray, false);
	tsr->player.tile_highlight_chunk = 0;
}

static void	update_toolbar(t_tsr *tsr)
{
	if (tsr->extras.toggle_toolbar)
	{
		if (mbx_key_pressed(tsr->mbx, MBX_KEY_LCTRL))
			tsr->player.on_toolbar = !tsr->player.on_toolbar;
	}
	else
		tsr->player.on_toolbar = mbx_key_held(tsr->mbx, MBX_KEY_LCTRL);
}

static void	toggle_godmode(t_tsr *tsr)
{
	const bool	checks[7] = {
		mbx_btnp(tsr->mbx, MBX_KEY_UP),
		mbx_btnp(tsr->mbx, MBX_KEY_DOWN),
		mbx_btnp(tsr->mbx, MBX_KEY_LEFT),
		mbx_btnp(tsr->mbx, MBX_KEY_RIGHT),
		mbx_text_input_has(tsr->mbx, "B"),
		mbx_text_input_has(tsr->mbx, "A"),
		mbx_btnp(tsr->mbx, MBX_KEY_ENTER)};
	const uint	sequence[11] = {0, 0, 1, 1, 2, 3, 2, 3, 4, 5, 6};
	static uint	step = 0;

	if (checks[sequence[step]])
		step++;
	else if (checks[0] || checks[1] || checks[2]
		|| checks[3] || checks[4] || checks[5])
		step = 0;
	if (step == 11
		|| (mbx_btn(tsr->mbx, MBX_KEY_F3) && mbx_btnp(tsr->mbx, MBX_KEY_F4)))
	{
		tsr->player.godmode = !tsr->player.godmode;
		step = 0;
	}
}

void	tsr_update_player(t_tsr *tsr)
{
	player_rotation(tsr);
	raycast(tsr);
	if (tsr->player.prompt_state)
	{
		player_update_prompt(tsr);
		return ;
	}
	toggle_godmode(tsr);
	player_movement(tsr);
	if (!tsr->player.godmode)
		return ;
	update_toolbar(tsr);
	if (tsr->player.on_toolbar)
	{
		tsr->player.hotbar_tool = wrap(
				(tsr->player.hotbar_tool - tsr->mbx->scroll_delta), 0, 7);
		player_tools(tsr);
	}
	else
	{
		item_select(tsr);
		place_and_destroy(tsr);
	}
}
