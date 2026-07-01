/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hotbar.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 23:54:59 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/29 00:07:19 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "modules/mbx_constants.h"
#include "tsr.h"
#include "tsr_core.h"

void	set_hotbar_offset(
	t_tsr_hotbar *hotbar, double target, t_vec2 limits)
{
	const double	span = limits.y - limits.x;
	const double	diff = target - hotbar->offset;
	double			delta;

	if (fabs(diff) < span / 2)
		delta = 0.2 * diff;
	else
		delta = 0.2 * fsign(diff) * (fabs(diff) - span);
	if (fabs(delta) < 0.01)
	{
		hotbar->offset = target;
		return ;
	}
	hotbar->delta += delta;
	hotbar->delta *= 0.5;
	hotbar->offset = fwrap(
			hotbar->offset + hotbar->delta, limits.x, limits.y);
}

static void	setter(
	void *data, t_mbx_region *region, int index, t_mbx_color col)
{
	const t_vec3	light = *(t_vec3 *)data;

	col.r *= light.r;
	col.g *= light.g;
	col.b *= light.b;
	region->pixels[index] = col;
}

static void	set_light_color(t_tsr *tsr, t_vec3 normal, bool skybox)
{
	const t_vec3	sky_color = tsr->world_data.tiles
	[tsr->world_data.skybox.type].light;
	static t_vec3	light;
	double			diffuse;

	tsr->ui.target->pipeline.data = &light;
	if (skybox)
		diffuse = 1;
	else
		diffuse = fmax(vec3_dot(normal, tsr->world_data.skylight), 0);
	light = vec3_add(tsr->world_data.ambient_color,
			vec3_mult_d(sky_color, diffuse + 0.2));
	light = vec3_exec2(fmin, vec3_exec(sqrt, light), vec3_d(1));
}

static void	draw_iso_tile(t_tsr *tsr, t_vec2i pos, t_tsr_tile tile)
{
	const bool			skybox = tsr->world_data.tiles[tile.type].skybox;
	t_mbx_region		*texture;
	t_vec2i				dir;

	dir = vec2i(-1, 1);
	texture = tsr->world_data.tiles[tile.type].texture
	[get_tile_texture_dir(&tile, 4, &dir)]->texture[0];
	set_light_color(tsr, tsr->camera.right, skybox);
	mbx_set_subregion_transformed(tsr->ui.target, texture,
		vec2ix3(pos, vec2i(0, 0), vec2i_mult(texture->subsize, dir)), vec2x2(
			vec2_mult_d(vec2(-0.8125, -0.375), 16. / texture->subsize.x),
			vec2_mult_d(vec2(0, 1), 16. / texture->subsize.y)));
	dir = vec2i(1, 1);
	texture = tsr->world_data.tiles[tile.type].texture
	[get_tile_texture_dir(&tile, 0 ^ skybox, &dir)]->texture[0];
	set_light_color(tsr, tsr->camera.forward, skybox);
	mbx_set_subregion_transformed(tsr->ui.target, texture,
		vec2ix3(pos, vec2i(0, 0), vec2i_mult(texture->subsize, dir)), vec2x2(
			vec2_mult_d(vec2(0.8125, -0.375), 16. / texture->subsize.x),
			vec2_mult_d(vec2(0, 1), 16. / texture->subsize.y)));
	dir = vec2i(1, 1);
	texture = tsr->world_data.tiles[tile.type].texture
	[get_tile_texture_dir(&tile, 3 ^ skybox, &dir)]->texture[0];
	set_light_color(tsr, tsr->camera.up, skybox);
	mbx_set_subregion_transformed(tsr->ui.target, texture,
		vec2ix3(pos, vec2i(0, 0), vec2i_mult(texture->subsize, dir)), vec2x2(
			vec2_mult_d(vec2(-0.825, -0.4), 16. / texture->subsize.x),
			vec2_mult_d(vec2(0.8125, -0.375), 16. / texture->subsize.y)));
}

static void	draw_focused(t_tsr *tsr)
{
	const char	dirs[6] = "WEDUNS";
	char		name[70];
	size_t		len;

	mbx_set_region(tsr->ui.target, tsr->textures.gui.hotbar_selection,
		vec2i(302, 319));
	len = snprintf(name, 70, "%.64s [%c%.1i]",
			tsr->world_data.tiles[tsr->player.hotbar_tile.type].name,
			dirs[tsr->player.hotbar_tile.rotation >> 2],
			tsr->player.hotbar_tile.rotation & 3);
	mbx_set_text(tsr->ui.target, name,
		vec2i(320 - len * 2.5, 312), tsr->textures.font_small);
}

void	draw_hotbar(t_tsr *tsr)
{
	double		off;
	int			i;

	set_hotbar_offset(&tsr->ui.hotbar, tsr->player.hotbar_tile.type,
		vec2(1, tsr->world_data.tile_count));
	tsr->ui.target->pipeline.set = setter;
	i = -6;
	while (i <= 6)
	{
		off = i - tsr->ui.hotbar.offset + (int)(tsr->ui.hotbar.offset);
		draw_iso_tile(tsr,
			vec2i(320 + 32 * off, 336 - 32 * (cos(off * off / 20.65) - 1)),
			tsr_tile_r(tsr, wrap((int)(tsr->ui.hotbar.offset) + i,
					1, tsr->world.data->tile_count),
				tsr->player.hotbar_tile.rotation));
		i++;
	}
	tsr->ui.target->pipeline = tsr->mbx->settings.default_pipeline;
	if (!tsr->player.on_toolbar
		|| tsr->player.hotbar_tool == TOOL_SET_LIMIT)
		draw_focused(tsr);
}
