/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:43:42 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/13 17:52:00 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

t_tsr_ray	setup_ray(t_tsr *tsr, t_vec3 origin, t_vec3 forward)
{
	const t_vec3i	iter = vec3i(1, tsr->wworld.size.x,
			tsr->wworld.size.x * tsr->wworld.size.y);
	t_tsr_ray		ray;

	ray = (t_tsr_ray){.origin = origin, .dir = forward};
	ray.dir_sign = vec3i_vd(vec3_sign(ray.dir));
	ray.delta = vec3_div_rd(1, forward);
	ray.abs_delta = vec3_abs(ray.delta);
	ray.iter = vec3i_mult(iter, ray.dir_sign);
	ray.tile_position = vec3i_vd(vec3_exec(floor, ray.origin));
	ray.tile_index = ray.tile_position.x * iter.x
		+ ray.tile_position.y * iter.y
		+ ray.tile_position.z * iter.z;
	ray.tile = &tsr->world.tiles[tsr->wworld.blocks[ray.tile_index]];
	ray.prev_tile = ray.tile;
	ray.dist = vec3_mult(vec3_sub(
				vec3_vi(vec3i_add(ray.tile_position, vec3i(
							ray.dir_sign.x > 0,
							ray.dir_sign.y > 0,
							ray.dir_sign.z > 0))),
				ray.origin), ray.delta);
	return (ray);
}

t_mbx_color	draw_ray(t_tsr *tsr, t_vec2 uv)
{
	const t_vec2	uvc = vec2((uv.x - 0.5) * 16 / 9, uv.y - 0.5);
	t_tsr_ray		ray;

	ray = setup_ray(tsr, tsr->camera.position,
			vec3_add(tsr->camera.forward, vec3_add(
					vec3_mult_d(tsr->camera.right, uvc.x),
					vec3_mult_d(tsr->camera.up, -uvc.y))));
	while (true)
	{
		trace_ray(tsr, &ray);
		if (ray.draw_prev_tile)
		{
			ray.color = vec4_blend(
					ray_tile_color(tsr, &ray, ray.prev_tile), ray.color);
			if (ray.color.a == 1)
				break ;
		}
		if (ray.draw_tile)
		{
			ray.color = vec4_blend(
					ray_tile_color(tsr, &ray, ray.tile), ray.color);
			if (ray.color.a == 1)
				break ;
		}
	}
	return (vec4_to_color(ray.color));
}
