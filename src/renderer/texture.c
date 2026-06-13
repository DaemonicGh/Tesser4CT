/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 23:38:55 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/10 11:24:41 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static t_vec2	get_skybox_uv(t_tsr_ray *ray)
{
	const t_vec3	abs_fw = vec3_abs(ray->dir);
	t_vec2i			axis;
	t_vec2			uv;

	if (abs_fw.z > abs_fw.x && abs_fw.z > abs_fw.y)
		ray->axis = 2;
	else
		ray->axis = (abs_fw.y > abs_fw.x);
	axis = vec2i((ray->axis == 0) * 2, (ray->axis == 1) + 1);
	uv = vec2(ray->dir.v[axis.x], ray->dir.v[axis.y]);
	uv = vec2_div_d(uv, abs_fw.v[ray->axis]);
	if ((ray->dir.v[ray->axis] > 0) == !(ray->axis & 2))
		uv.x = uv.x * 0.5 + 0.5;
	else
		uv.x = -uv.x * 0.5 + 0.5;
	uv.y = -uv.y * 0.5 + 0.5;
	return (uv);
}

t_vec2	get_tile_uv(t_tsr_ray *ray)
{
	t_vec2i		axis;
	t_vec2		uv;

	axis = vec2i((ray->tile_axis == 0) * 2, (ray->tile_axis == 1) + 1);
	uv = vec2(ray->position.v[axis.x], ray->position.v[axis.y]);
	if ((ray->dir.v[ray->tile_axis] > 0) == !(ray->tile_axis))
		uv.x = uv.x - floor(uv.x);
	else
		uv.x = ceil(uv.x) - uv.x;
	if (ray->axis == 1)
		uv.y = uv.y - floor(uv.y);
	else
		uv.y = ceil(uv.y) - uv.y;
	return (uv);
}

t_mbx_color	get_texture_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile)
{
	t_vec2i	texture_uv;

	(void)tsr;
	(void)tile;
	if (ray->tile_data->skybox)
		ray->uv = get_skybox_uv(ray);
	else
		ray->uv = get_tile_uv(ray);
	ray->face = ray->axis * 2 + (ray->dir_sign.v[ray->axis] < 0);
	ray->texture = ray->tile_data->texture[ray->face];
	if (ray->uv.x >= 1 || ray->uv.y >= 1)
	{
		printf("PIXEL FAIL (? %i %i)\n",
			(ray->dir.v[ray->axis] < 0) == !(ray->axis & 2), ray->axis == 1);
		ray->uv = vec2_exec2(fmin, ray->uv, vec2_d(1 - 1e-6));
	}
	texture_uv = vec2i_mult_vd(ray->texture->texture->subregion_size, ray->uv);
	return (mbx_get_pixel_unsafe(ray->texture->texture, texture_uv));
}
