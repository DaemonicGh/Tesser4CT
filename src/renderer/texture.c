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

static void	get_skybox_uv(t_tsr_ray *ray)
{
	const t_vec3	abs_fw = vec3_abs(ray->dir);
	t_vec2i			axis;

	if (abs_fw.z > abs_fw.x && abs_fw.z > abs_fw.y)
		ray->axis = 2;
	else
		ray->axis = (abs_fw.y > abs_fw.x);
	axis = vec2i((ray->axis == 0) * 2, (ray->axis == 1) + 1);
	ray->uv = vec2(ray->dir.v[axis.x], ray->dir.v[axis.y]);
	ray->uv = vec2_div_d(ray->uv, abs_fw.v[ray->axis]);
	if ((ray->dir.v[ray->axis] > 0) == !(ray->axis & 2))
		ray->uv.x = ray->uv.x * 0.5 + 0.5;
	else
		ray->uv.x = -ray->uv.x * 0.5 + 0.5;
	ray->uv.y = -ray->uv.y * 0.5 + 0.5;
}

static void	get_tile_uv(t_tsr_ray *ray)
{
	const t_vec2i	axis = vec2i((ray->axis == 0) * 2, (ray->axis == 1) + 1);

	get_ray_position(ray);
	ray->uv = vec2(ray->position.v[axis.x], ray->position.v[axis.y]);
	if ((ray->dir.v[ray->axis] < 0) == !(ray->axis & 2))
		ray->uv.x = ray->uv.x - floor(ray->uv.x);
	else
		ray->uv.x = ceil(ray->uv.x) - ray->uv.x;
	ray->uv.y = ceil(ray->uv.y) - ray->uv.y;
}

t_mbx_color	get_texture_color(t_tsr_ray *ray, t_tsr_tile *tile)
{
	if (tile->skybox)
		get_skybox_uv(ray);
	else
		get_tile_uv(ray);
	ray->texture = tile->texture[ray->axis * 2
		+ (ray->dir_sign.v[ray->axis] > 0)];
	ray->texture_uv = vec2i_mult_vd(ray->texture->texture->size, ray->uv);
	if (ray->texture->flags & TX_PROPERTY)
		ray->texture_uv.y /= 3;
	else if (ray->texture->flags & TX_NORMAL)
		ray->texture_uv.y /= 2;
	return (mbx_get_pixel_unsafe(ray->texture->texture, ray->texture_uv));
}
