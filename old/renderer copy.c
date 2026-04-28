/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer copy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:43:42 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/27 16:06:03 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_drawing.h"
#include "modules/types/mbx_s_color.h"
#include "tsr.h"
#include "tsr_core.h"

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

t_traversal	ray_traversal(t_vec3 ray_dir, t_vec3 ray_org, t_tsr *tsr)
{
	t_vec3	t_delta;
	t_vec3	t_dist;
	t_vec3i	block;
	t_vec3i	sgn;
	int		axis;

	t_delta = vec3_div_rd(1.0, ray_dir);
	sgn = vec3i_vd(vec3_sign(t_delta));
	t_dist = vec3(
			clamp(floor(ray_org.x), 0, tsr->wworld.width - 1) + 1 - ray_org.x,
			clamp(floor(ray_org.y), 0, tsr->wworld.height - 1) + 1 - ray_org.y,
			clamp(floor(ray_org.z), 0, tsr->wworld.depth - 1) + 1 - ray_org.z
			);
	if (sgn.x == -1)
		t_dist.x = ray_org.x - clamp(
				floor(ray_org.x), 0, tsr->wworld.width - 1);
	if (sgn.y == -1)
		t_dist.y = ray_org.y - clamp(
				floor(ray_org.y), 0, tsr->wworld.height - 1);
	if (sgn.z == -1)
		t_dist.z = ray_org.z - clamp(
				floor(ray_org.z), 0, tsr->wworld.depth - 1);
	t_delta = vec3_abs(t_delta);
	t_dist = vec3_mult(t_dist, t_delta);
	block = vec3i_vd(vec3_exec(floor, ray_org));
	block.x = clamp(block.x, 0, tsr->wworld.width - 1);
	block.y = clamp(block.y, 0, tsr->wworld.height - 1);
	block.z = clamp(block.z, 0, tsr->wworld.depth - 1);
	axis = step_tdist(&t_dist, &t_delta, &block, &sgn);
	while (inbound(&tsr->wworld, block) && !block_get(&tsr->wworld, block))
		axis = step_tdist(&t_dist, &t_delta, &block, &sgn);
	t_dist = vec3_sub(t_dist, t_delta);
	return ((t_traversal){axis, (double [3]){
		t_dist.x, t_dist.y, t_dist.z}[axis], block});
}

t_vec3	get_normal(t_vec3 ray_dir, int axis)
{
	t_vec3	normal;

	normal = vec3_zero();
	normal.comp[axis] = fsign(ray_dir.comp[axis]);
	return (normal);
}

/*static t_vec3	reflection(t_vec3 incident, t_vec3 normal)
{
	return (vec3_sub(incident, vec3_mult_d(normal,
		2.0 * vec3_dot(incident, normal))));
}*/

static t_mbx_color	combiner(t_vec3i icolor)
{
	return (color(icolor.x << 16 | icolor.y << 8 | icolor.z));
}

static t_vec3i	unmixer(t_mbx_color icolor)
{
	return (vec3i(icolor.r, icolor.g, icolor.b));
}

static t_mbx_color	get_skybox(t_tsr *tsr, t_vec3 ray_dir)
{
	return (color(0xBBBBFF));
	return (mbx_get_pixel_xy(tsr->world.tiles[2].region,
			(ray_dir.x / 2 + 0.5) * tsr->world.tiles[2].region->size.x,
			asin(ray_dir.y)));
}

t_mbx_color	rt_shader(t_vec2 uv, void *data)
{
	t_vec3		ray_dir;
	t_tsr		*tsr;
	t_traversal	traversal;
	t_vec3i		frag_color;
	t_vec3		point;
	//t_vec3	reflect;
	t_vec2		tuv;
	uint8_t		type;

	tsr = data;
	ray_dir = vec3_add(vec3_add(tsr->camera.forward,
				vec3_mult_d(tsr->camera.right, uv.x * 2.0 - 1.0)),
			vec3_mult_d(tsr->camera.up,
				(1.0 - uv.y * 2.0) * tsr->extras.aspect_ratio));
	traversal = ray_traversal(ray_dir, tsr->camera.position, tsr);
	if (!inbound(&tsr->wworld, traversal.block))
		return (get_skybox(tsr, ray_dir));
	point = vec3_add(tsr->camera.position,
			vec3_mult_d(ray_dir, traversal.dist));
	if (traversal.axis == 0)
		tuv = vec2(point.z, point.y);
	else if (traversal.axis == 1)
		tuv = vec2(point.x, point.z);
	else
		tuv = vec2(point.x, point.y);
	tuv = vec2(tuv.x - floor(tuv.x), tuv.y - floor(tuv.y));
	type = block_get(&tsr->wworld, traversal.block);
	frag_color = unmixer(mbx_get_pixel_unsafe(tsr->world.tiles[type].region,
				vec2i_mult_vd(tsr->world.tiles[type].region->size, tuv)));
	/* Code for double reflexion
	frag_color = vec3i_mult_d(frag_color, 3);
	reflect = reflection(ray_dir, get_normal(ray_dir, traversal.axis));
	point = vec3_add(point, vec3_mult_d(reflect, 1e-12));
	traversal = ray_traversal(reflect, point, tsr);
	if (!inbound(&tsr->wworld, traversal.block))
	{
		frag_color = vec3i_add(frag_color, vec3i_mult_d(
			vec3i(144, 224, 252), 3));
		return (combiner(vec3i_div_d(frag_color, 6)));
	}
	point = vec3_add(point, vec3_mult_d(reflect, traversal.dist));
	if (traversal.axis == 0)
		tuv = vec2(fmod(point.z, 1.0), fmod(point.y, 1.0));
	else if (traversal.axis == 1)
		tuv = vec2(fmod(point.x, 1.0), fmod(point.z, 1.0));
	else
		tuv = vec2(fmod(point.x, 1.0), fmod(point.y, 1.0));
	frag_color = vec3i_add(frag_color, vec3i_mult_d(unmixer(
		mbx_get_pixel_unsafe(tsr->world.tiles[type].region,
			vec2i_mult_vd(tsr->world.tiles[type].region->size, tuv))), 2));
	reflect = reflection(reflect, get_normal(reflect, traversal.axis));
	point = vec3_add(point, vec3_mult_d(reflect, 1e-12));
	traversal = ray_traversal(reflect, point, tsr);
	if (!inbound(&tsr->wworld, traversal.block))
	{
		frag_color = vec3i_add(frag_color, vec3i(144, 224, 252));
		return (combiner(vec3i_div_d(frag_color, 6)));
	}
	point = vec3_add(point, vec3_mult_d(reflect, traversal.dist));
	if (traversal.axis == 0)
		tuv = vec2(fmod(point.z, 1.0), fmod(point.y, 1.0));
	else if (traversal.axis == 1)
		tuv = vec2(fmod(point.x, 1.0), fmod(point.z, 1.0));
	else
		tuv = vec2(fmod(point.x, 1.0), fmod(point.y, 1.0));
	frag_color = vec3i_add(frag_color, unmixer(
		mbx_get_pixel_unsafe(tsr->world.tiles[type].region, vec2i_mult_vd(tsr->world.tiles[type].region->size, tuv))));
	*/
	return (combiner(frag_color));
}

t_mbx_color	draw_ray(t_tsr *tsr, t_vec2i frag_pos)
{
	return (rt_shader(vec2i_truediv(frag_pos, tsr->mbx->vp->size), tsr));
}
