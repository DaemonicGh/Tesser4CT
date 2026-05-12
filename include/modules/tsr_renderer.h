/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_renderer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:33:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/12 02:45:47 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "modules/types/mbx_s_color.h"
#include "modules/types/mbx_s_region.h"
#include "tsr_world.h"

typedef struct s_tsr_render_ray
{
	t_vec3			origin;
	t_vec3			forward;
	t_vec3			delta;
	t_vec3			abs_delta;
	t_vec3			dist;
	double			distance;
	t_vec3			position;
	t_vec3i			tile_position;
	t_vec3i			delta_sign;
	t_vec3i			iter;
	int				tile_index;
	int				axis;
	int				lifetime;
	t_tsr_tile		*tile;
	t_tsr_tile		*prev_tile;
	bool			render_tile;
	bool			render_prev_tile;
	bool			is_shadow;
	t_mbx_region	*region;
	t_vec2			uv;
	t_vec2i			texture_uv;
	t_mbx_color		color;
	t_vec3			normal;
}	t_tsr_ray;

t_vec3
get_normal(t_vec3 ray_dir, int axis);

t_vec3
reflect(t_vec3 ray_dir, t_vec3 normal);

t_mbx_color
get_hit_color(t_tsr *tsr, t_tsr_ray *ray, t_tsr_tile *tile);

t_mbx_color
draw_ray(t_tsr *tsr, t_vec2 uv);

void
trace_ray(t_tsr *tsr, t_tsr_ray *ray);

t_tsr_ray
setup_ray(t_tsr *tsr, t_vec3 origin, t_vec3 forward);

void
get_ray_position(t_tsr_ray *ray);

void
*tsr_render_thread_init(void *data);

void
draw_title(t_tsr *tsr, char *str, t_vec2i pos, t_vec2 scale);
