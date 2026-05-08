/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:26:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/08 15:10:00 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	place_and_destroy(t_tsr *tsr)
{
	t_tsr_ray				traversal;

	traversal = setup_ray(tsr, tsr->player.position, tsr->camera.forward);
	if (mbx_key_pressed(tsr->mbx, MBX_MOUSE_LEFT))
	{
		trace_ray(tsr, &traversal);
		block_set(&tsr->wworld, traversal.tile_position, 0);
	}
	if (mbx_key_pressed(tsr->mbx, MBX_MOUSE_RIGHT))
	{
		trace_ray(tsr, &traversal);
		traversal.tile_position = vec3i_sub(traversal.tile_position,
				vec3i_vd(get_normal(traversal.forward, traversal.axis)));
		block_set(&tsr->wworld, traversal.tile_position, tsr->player.tile_id);
	}
	if (mbx_key_pressed(tsr->mbx, MBX_MOUSE_MIDDLE))
	{
		trace_ray(tsr, &traversal);
		tsr->player.tile_id = block_get(&tsr->wworld, traversal.tile_position);
	}
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_Q))
	{
		if (tsr->player.tile_id == 1)
			tsr->player.tile_id = TILE_COUNT;
		else
			tsr->player.tile_id--;
	}
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_E))
		tsr->player.tile_id = tsr->player.tile_id % TILE_COUNT + 1;
}

void	tsr_player_actions(t_tsr *tsr)
{
	place_and_destroy(tsr);
}
