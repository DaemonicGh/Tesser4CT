/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ray.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:43:42 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/28 22:08:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

t_tsr_ray	setup_ray(
	t_tsr *tsr, t_vec3 origin, t_tsr_chunk *chunk, t_vec3 forward)
{
	const t_vec3i	iter = vec3i(1, 4, 16);
	t_vec3i			tile_pos;
	t_tsr_ray		ray;

	tile_pos = vec3i_vd(vec3_exec(floor, origin));
	chunk = tsr_relocate_chunk(tsr, chunk, &tile_pos);
	ray = (t_tsr_ray){.origin = origin,
		.dir = forward, .dir_sign = vec3i_vd(vec3_sign(forward)),
		.delta = vec3_div_rd(1, forward),
		.chunk = chunk, .chunk_id = chunk->id,
		.tile_position = tile_pos, .lifetime = 128};
	ray.abs_delta = vec3_abs(ray.delta);
	ray.iter = vec3i_mult(iter, ray.dir_sign);
	ray.tile_index = tile_pos.x * iter.x
		+ tile_pos.y * iter.y + tile_pos.z * iter.z;
	ray.tile = tsr_get_tile(tsr, chunk, ray.tile_position);
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

	ray = setup_ray(tsr, tsr->camera.chunk_position, tsr->camera.chunk,
			vec3_add(vec3_mult_d(tsr->camera.forward, tsr->extras.focal_length),
				vec3_add(vec3_mult_d(tsr->camera.right, uvc.x),
					vec3_mult_d(tsr->camera.up, -uvc.y))));
	while (true)
	{
		trace_ray(tsr, &ray);
		if (set_ray_tile_color(tsr, &ray, ray.tile))
			break ;
	}
	return (vec4_to_color(ray.color));
}
