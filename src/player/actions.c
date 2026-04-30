/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:26:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/30 16:44:27 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	place_and_destroy(t_tsr *tsr)
{
	t_tsr_ray				traversal;
	static size_t 			block_nb = 11;
	static t_tsr_tile_id	type = 1;

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
		block_set(&tsr->wworld, traversal.tile_position, type);
	}
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_Q))
	{
		if (type == 1)
			type = block_nb;
		else
			type--;
	}
	if (mbx_key_pressed(tsr->mbx, MBX_KEY_E))
		type = type % block_nb + 1;
}

void	tsr_player_actions(t_tsr *tsr)
{
	place_and_destroy(tsr);
}
