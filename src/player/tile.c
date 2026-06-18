/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tile.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:26:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/11 21:38:22 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

void	item_select(t_tsr *tsr)
{
	uint32_t	i;

	i = 0;
	while (i <= tsr->world_data.tile_count / 10)
	{
		if (mbx_key_pressed(tsr->mbx, 30 + i))
			tsr->player.hotbar_tile.type = i * 10 + 1;
		i++;
	}
	if (mbx_key_held(tsr->mbx, MBX_KEY_LALT))
		tsr->player.hotbar_tile.rotation = wrap(
				(tsr->player.hotbar_tile.rotation - tsr->mbx->scroll_delta),
				0, 24);
	else
		tsr->player.hotbar_tile.type = wrap(
				(tsr->player.hotbar_tile.type - tsr->mbx->scroll_delta),
				1, tsr->world_data.tile_count);
}

static void	place_tile(t_tsr *tsr, const t_tsr_ray *ray)
{
	static t_vec3i	position = {.x = -1, .y = -1, .z = -1};
	t_tsr_chunk_id	chunk;

	if (mbx_key_held(tsr->mbx, MBX_MOUSE_RIGHT)
		&& (tsr->mbx->inputs[MBX_MOUSE_RIGHT].press == 0
			|| tsr->mbx->inputs[MBX_MOUSE_RIGHT].press > 0.1))
	{
		if (!vec3i_eq(ray->tile_position, position))
		{
			position = vec3i_sub(ray->tile_position, ray->tile_normal);
			chunk = tsr_relocate_chunk(&tsr->world, ray->chunk, &position);
			if (chunk)
				tsr_set_tile(&tsr->world, chunk, position,
					tsr->player.hotbar_tile);
		}
	}
	else if (mbx_key_released(tsr->mbx, MBX_MOUSE_RIGHT))
		position = vec3i_i(-1);
}

static void	break_tile(t_tsr *tsr, const t_tsr_ray *ray)
{
	const double	break_time = 0.166;
	static double	break_timer = 0.0;

	if (mbx_key_held(tsr->mbx, MBX_MOUSE_LEFT)
		&& (tsr->mbx->inputs[MBX_MOUSE_LEFT].press == 0
			|| tsr->mbx->inputs[MBX_MOUSE_LEFT].press > break_time * 2))
	{
		if (break_timer <= 0 && tsr_set_tile(
				&tsr->world, ray->chunk, ray->tile_position,
				tsr_tile_r(0, 0)))
			break_timer = break_time;
		else
			break_timer -= tsr->mbx->delta_time;
	}
	else if (mbx_key_released(tsr->mbx, MBX_MOUSE_LEFT))
		break_timer = 0.0;
}

void	place_and_destroy(t_tsr *tsr)
{
	if (tsr->player.ray.distance < 8.0)
	{
		if (mbx_key_pressed(tsr->mbx, MBX_MOUSE_MIDDLE))
			tsr->player.hotbar_tile = *tsr_get_tile(
					&tsr->world, tsr->player.ray.chunk,
					tsr->player.ray.tile_position);
		break_tile(tsr, &tsr->player.ray);
		place_tile(tsr, &tsr->player.ray);
		tsr->player.tile_highlight_chunk = tsr->player.ray.chunk;
		tsr->player.tile_highlight_pos = tsr->player.ray.tile_position;
		tsr->player.tile_highlight_axis = tsr->player.ray.axis;
	}
	else
		tsr->player.tile_highlight_chunk = 0;
}
