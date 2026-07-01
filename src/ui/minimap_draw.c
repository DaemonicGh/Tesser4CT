/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/24 16:00:27 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/24 16:00:27 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

static void	draw_line(t_tsr *tsr, t_vec3i pos1, t_vec3i pos2, t_mbx_color col)
{
	const t_vec3	cam_pos = vec3_add(tsr->camera.position,
			vec3_mult_d(tsr->camera.forward, -6));
	const t_vec3x2	cpos = vec3x2(
			vec3_sub(cam_pos, vec3_vi(pos1)),
			vec3_sub(cam_pos, vec3_vi(pos2)));
	const t_vec3x2	transformed = vec3x2(vec3(
				vec3_dot(tsr->camera.right, cpos.p1),
				vec3_dot(vec3_neg(tsr->camera.up), cpos.p1),
				-vec3_dot(tsr->camera.forward, cpos.p1)), vec3(
				vec3_dot(tsr->camera.right, cpos.p2),
				vec3_dot(vec3_neg(tsr->camera.up), cpos.p2),
				-vec3_dot(tsr->camera.forward, cpos.p2)));

	col.a = clamp(fmin((16 - fmin(
						vec3_dist(tsr->camera.position, vec3_vi(pos1)),
						vec3_dist(tsr->camera.position, vec3_vi(pos2)))) * 32,
				(fmin(transformed.p1.z, transformed.p2.z) - 4) * 80), 0, 255);
	if (!col.a)
		return ;
	mbx_set_line(tsr->ui.target, vec2ix2_xy(
			320 - transformed.p1.x / transformed.p1.z * 80,
			180 - transformed.p1.y / transformed.p1.z * 80,
			320 - transformed.p2.x / transformed.p2.z * 80,
			180 - transformed.p2.y / transformed.p2.z * 80), col);
}

static void	draw_tile_lines(t_tsr *tsr, t_vec3i pos, uint lines)
{
	uint	i;
	t_vec3i	pos1;
	t_vec3i	pos2;

	i = 11;
	while (lines)
	{
		if (lines & 1)
		{
			pos1 = vec3i_sub_d(pos, MINIMAP_CENTER * 4);
			pos1.v[(i < 4)] += (i & 1);
			pos1.v[(i < 8) + 1] += (i >> 1 & 1);
			pos2 = pos1;
			pos2.v[i / 4] += 1;
			draw_line(tsr, pos1, pos2, color(0xFFFFFF));
		}
		lines >>= 1;
		i--;
	}
}

static void	draw_tile(t_tsr *tsr,
	t_tsr_chunk_id chunk, t_vec3i chunk_pos, t_vec3i pos)
{
	const t_tsr_world	*world = &tsr->world;
	t_tsr_tile_data		*data;
	uint				lines;

	lines = 0b111111111111;
	data = tsr_get_tile_data(world, chunk, vec3i_add(pos, vec3i(-1, 0, 0)));
	if (!data->skip && !data->skybox)
		lines &= ~0b000010101010;
	data = tsr_get_tile_data(world, chunk, vec3i_add(pos, vec3i(1, 0, 0)));
	if (!data->skip && !data->skybox)
		lines &= ~0b000001010101;
	data = tsr_get_tile_data(world, chunk, vec3i_add(pos, vec3i(0, -1, 0)));
	if (!data->skip && !data->skybox)
		lines &= ~0b101000001100;
	data = tsr_get_tile_data(world, chunk, vec3i_add(pos, vec3i(0, 1, 0)));
	if (!data->skip && !data->skybox)
		lines &= ~0b010100000011;
	data = tsr_get_tile_data(world, chunk, vec3i_add(pos, vec3i(0, 0, -1)));
	if (!data->skip && !data->skybox)
		lines &= ~0b110011000000;
	data = tsr_get_tile_data(world, chunk, vec3i_add(pos, vec3i(0, 0, 1)));
	if (!data->skip && !data->skybox)
		lines &= ~0b001100110000;
	draw_tile_lines(tsr, vec3i_add(vec3i_mult_d(chunk_pos, 4), pos), lines);
}

void	draw_chunk(t_tsr *tsr,
	t_tsr_explore map[MINIMAP_SIZE][MINIMAP_SIZE][MINIMAP_SIZE], t_vec3i pos)
{
	const t_tsr_chunk_id	chunk = map[pos.x][pos.y][pos.z].id;
	t_tsr_tile_data			*data;
	t_vec3i					tile;
	uint					i;

	i = 0;
	tile.z = 0;
	while (tile.z < 4)
	{
		tile.y = 0;
		while (tile.y < 4)
		{
			tile.x = 0;
			while (tile.x < 4)
			{
				data = &tsr->world_data.tiles[
					tsr->world.chunks[chunk].tiles[i++].type];
				if (!data->skip && !data->skybox)
					draw_tile(tsr, chunk, pos, tile);
				tile.x++;
			}
			tile.y++;
		}
		tile.z++;
	}
}

void	draw_chunk_error(t_tsr *tsr, t_vec3i pos)
{
	const t_vec3i	tpos = vec3i_mult_d(vec3i_sub_d(pos, MINIMAP_CENTER), 4);
	uint			i;
	t_vec3i			pos1;
	t_vec3i			pos2;

	i = 12;
	while (i--)
	{
		pos1 = tpos;
		pos1.v[(i < 4)] += (i & 1) * 4;
		pos1.v[(i < 8) + 1] += (i >> 1 & 1) * 4;
		pos2 = pos1;
		pos2.v[i / 4] += 4;
		draw_line(tsr, pos1, pos2, color(0xCCDDFF));
	}
}
