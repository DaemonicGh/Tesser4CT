/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:43:42 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/15 17:11:47 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/tsr_renderer.h"

static int	step_tdist(
	t_vec3 *t_dist, t_vec3 *t_delta, t_vec3i *block, t_vec3i *sgn)
{
	if (t_dist->x < t_dist->y)
	{
		if (t_dist->x < t_dist->z)
		{
			t_dist->x += t_delta->x;
			block->x += sgn->x;
			return (0);
		}
	}
	else if (t_dist->y < t_dist->z)
	{
		t_dist->y += t_delta->y;
		block->y += sgn->y;
		return (1);
	}
	t_dist->z += t_delta->z;
	block->z += sgn->z;
	return (2);
}

static void	ray_traversal(
	t_mbx_region *buffer, t_vec2i coord, t_vec3 ray_dir, t_tsr *tsr)
{
	t_vec3	t_delta;
	t_vec3	t_dist;
	t_vec3i	block;
	t_vec3i	sgn;
	int		axis;
	int		hex_color;

	t_delta = vec3_div_rd(1.0, ray_dir);
	sgn = vec3i(1, 1, 1);
	if (t_delta.x < 0)
		sgn.x = -1;
	if (t_delta.y < 0)
		sgn.y = -1;
	if (t_delta.z < 0)
		sgn.z = -1;
	t_dist = vec3(
			clamp(floor(tsr->pos.x) + 1, 1, tsr->world.width - 2) - tsr->pos.x,
			clamp(floor(tsr->pos.y) + 1, 1, tsr->world.height - 2) - tsr->pos.y,
			clamp(floor(tsr->pos.z) + 1, 1, tsr->world.depth - 2) - tsr->pos.z
			);
	if (sgn.x == -1)
		t_dist.x = tsr->pos.x - clamp(
				floor(tsr->pos.x), 1, tsr->world.width - 2);
	if (sgn.y == -1)
		t_dist.y = tsr->pos.y - clamp(
				floor(tsr->pos.y), 1, tsr->world.height - 2);
	if (sgn.z == -1)
		t_dist.z = tsr->pos.z - clamp(
				floor(tsr->pos.z), 1, tsr->world.depth - 2);
	t_delta = vec3_abs(t_delta);
	t_dist = vec3_mult(t_dist, t_delta);
	block = vec3i(
			clamp(floor(tsr->pos.x), 0, tsr->world.width - 1),
			clamp(floor(tsr->pos.y), 0, tsr->world.height - 1),
			clamp(floor(tsr->pos.z), 0, tsr->world.depth - 1));
	while (inbound(&tsr->world, block) && !block_get(&tsr->world, block))
		axis = step_tdist(&t_dist, &t_delta, &block, &sgn);
	hex_color = (int [3]){0xff0000, 0x00ff00, 0x0000ff}[axis];
	if (!inbound(&tsr->world, block))
		hex_color = 0x000000;
	mbx_set_pixel_raw(buffer, coord, color(hex_color));
}

void	setup_frag(t_mbx_region *buffer, t_tsr *tsr)
{
	const double	ratio = (double)buffer->size.y / buffer->size.x;
	t_vec3			ray_dir;
	t_vec3			ray_dir_tmp;
	t_vec3			dxy[2];
	t_vec2i			frag;

	ray_dir = vec3(-1.0, ratio, 1.0);
	vec3_pitch_rot(&ray_dir, -tsr->ang.y);
	vec3_yaw_rot(&ray_dir, -tsr->ang.x);
	dxy[0] = vec3(2.0 / buffer->size.x, 0.0, 0.0);
	dxy[1] = vec3(0.0, -2.0 * ratio / buffer->size.y, 0.0);
	vec3_pitch_rot(dxy + 0, -tsr->ang.y);
	vec3_yaw_rot(dxy + 0, -tsr->ang.x);
	vec3_pitch_rot(dxy + 1, -tsr->ang.y);
	vec3_yaw_rot(dxy + 1, -tsr->ang.x);
	frag.y = 0;
	while (frag.y < buffer->size.y)
	{
		ray_dir_tmp = ray_dir;
		frag.x = 0;
		while (frag.x < buffer->size.x)
		{
			ray_traversal(buffer, frag, ray_dir, tsr);
			ray_dir = vec3_add(ray_dir, dxy[0]);
			frag.x++;
		}
		ray_dir = vec3_add(ray_dir_tmp, dxy[1]);
		frag.y++;
	}
}
