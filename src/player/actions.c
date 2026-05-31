/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 15:26:42 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/28 23:11:19 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	item_select(t_tsr *tsr)
{
	uint32_t	i;

	i = 0;
	while (i <= tsr->world.tile_count / 10)
	{
		if (mbx_key_pressed(tsr->mbx, 30 + i))
			tsr->player.tile_id = i * 10 + 1;
		i++;
	}
	tsr->player.tile_id = wrap(
			(tsr->player.tile_id - tsr->mbx->scroll_delta),
			1, tsr->world.tile_count + 1);
}

static bool	place_tile(t_tsr *tsr, t_tsr_ray *ray)
{
	static t_vec3i			prev_tile = {
		.x = INT32_MIN, .y = INT32_MIN, .z = INT32_MIN};

	if (mbx_key_held(tsr->mbx, MBX_MOUSE_RIGHT)
		&& (tsr->mbx->keys[MBX_MOUSE_RIGHT].press == 0
			|| tsr->mbx->keys[MBX_MOUSE_RIGHT].press > 0.1))
	{
		if (!vec3i_eq(ray->tile_position, prev_tile))
		{
			prev_tile = vec3i_sub(ray->tile_position,
					vec3i_vd(get_tile_normal(ray->dir, ray->axis)));
			block_set(&tsr->wworld, prev_tile, tsr->player.tile_id);
			return (true);
		}
	}
	else if (mbx_key_released(tsr->mbx, MBX_MOUSE_RIGHT))
		prev_tile = vec3i_i(INT32_MIN);
	return (false);
}

static bool	break_tile(t_tsr *tsr, t_tsr_ray *ray)
{
	const double	break_time = 0.166;
	static double	break_timer = 0.0;

	if (mbx_key_held(tsr->mbx, MBX_MOUSE_LEFT)
		&& (tsr->mbx->keys[MBX_MOUSE_LEFT].press == 0
			|| tsr->mbx->keys[MBX_MOUSE_LEFT].press > break_time * 2))
	{
		if (break_timer <= 0)
		{
			block_set(&tsr->wworld, ray->tile_position, 0);
			break_timer = break_time;
			return (true);
		}
		else
			break_timer -= tsr->mbx->delta_time;
	}
	else if (mbx_key_released(tsr->mbx, MBX_MOUSE_LEFT))
		break_timer = 0.0;
	return (false);
}

static void	place_and_destroy(t_tsr *tsr)
{
	t_tsr_ray	ray;
	bool		ret;

	ray = setup_ray(tsr, tsr->camera.position, tsr->camera.forward);
	trace_ray(tsr, &ray);
	ret = true;
	if (ray.distance < 16.0)
	{
		if (mbx_key_pressed(tsr->mbx, MBX_MOUSE_MIDDLE))
			tsr->player.tile_id = block_get(&tsr->wworld, ray.tile_position);
		ret = break_tile(tsr, &ray);
		ret |= place_tile(tsr, &ray);
	}
	if (ret)
		tsr->world.tile_highlight_pos = vec3i_i(INT32_MIN);
	else
	{
		tsr->world.tile_highlight_pos = ray.tile_position;
		tsr->world.tile_highlight_axis = ray.axis;
	}
}

void	tsr_player_actions(t_tsr *tsr)
{
	place_and_destroy(tsr);
	item_select(tsr);
}
