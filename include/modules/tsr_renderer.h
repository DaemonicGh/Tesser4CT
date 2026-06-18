/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_renderer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:33:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/05/28 22:40:06 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tsr_core.h"
#include "tsr_world.h"

typedef struct s_tsr_pbr_properties
{
	double		specular;
	double		f0;
	double		emissive;
}	t_tsr_pbr;

typedef struct s_tsr_illumination_data
{
	t_vec2i			axis;
	t_vec2			uv;
	t_vec2i			dir;
	t_vec3i			pos;
	t_tsr_chunk_id	chunk;
	double			light[4];
}	t_tsr_illum;

enum	e_tsr_tile_rotations
{
	W0,
	W90,
	W180,
	W270,
	E0,
	E90,
	E180,
	E270,
	D0,
	D90,
	D180,
	D270,
	U0,
	U90,
	U180,
	U270,
	N0,
	N90,
	N180,
	N270,
	S0,
	S90,
	S180,
	S270
};

t_vec3
reflect(t_vec3 ray_dir, t_vec3 normal);

double
get_global_illumination(
	t_tsr *restrict tsr, t_tsr_ray *restrict ray);

void
apply_lighting_effects(
	t_tsr *restrict tsr, t_tsr_ray *restrict ray,
	t_mbx_color *restrict col);

void
get_normal(
	t_tsr *restrict tsr, t_tsr_ray *restrict ray);

t_vec2
get_tile_uv(t_tsr_ray *ray);

uint8_t
get_tile_texture(
	t_tsr_tile *tile, uint8_t face, t_vec2 *uv);

uint8_t
get_tile_texture_dir(
	t_tsr_tile *tile, uint8_t face, t_vec2i *dir);

t_mbx_color
get_texture_color(t_tsr_ray *restrict ray);

bool
set_ray_tile_color(
	t_tsr *restrict tsr, t_tsr_ray *restrict ray);

t_mbx_color
draw_ray(t_tsr *tsr, t_vec2 uv);

void
step_ray(
	t_tsr *restrict tsr, t_tsr_ray *restrict ray);

void
trace_ray(
	t_tsr *restrict tsr, t_tsr_ray *restrict ray, bool lighting_ray);

t_tsr_ray
setup_ray_unsafe(
	t_tsr *tsr, t_vec3 origin, t_tsr_chunk_id chunk, t_vec3 forward);

t_tsr_ray
setup_ray(
	t_tsr *tsr, t_vec3 origin, t_tsr_chunk_id chunk, t_vec3 forward);

void
*tsr_render_thread_init(void *data);

void
draw_title(t_tsr *tsr, char *str, t_vec2i pos, t_vec2 scale);
