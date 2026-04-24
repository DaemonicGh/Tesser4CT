/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 16:38:53 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/24 18:27:00 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tsr.h"

typedef unsigned int t_u32;

t_vec3	movement(t_mbx *mbx)
{
	t_vec3			input;
	const double	speed = 10 * mbx->delta_time;

	input = vec3_zero();
	if (mbx_key_held(mbx, MBX_KEY_W))
		input.z += 1.0;
	if (mbx_key_held(mbx, MBX_KEY_A))
		input.x -= 1.0;
	if (mbx_key_held(mbx, MBX_KEY_S))
		input.z -= 1.0;
	if (mbx_key_held(mbx, MBX_KEY_D))
		input.x += 1.0;
	if (mbx_key_held(mbx, MBX_KEY_LSHIFT))
		input.y -= 1.0;
	if (mbx_key_held(mbx, MBX_KEY_SPACE))
		input.y += 1.0;
	input = vec3_mult_d(vec3_normalize(input), speed);
	return (input);
}

void	mouse_cam(t_mbx *mbx, t_vec2 *ang)
{
	const t_vec2i midbuffer = vec2i(
		mbx->viewport->size.x / 2, mbx->viewport->size.y / 2);

	mbx_warp_cursor(mbx, midbuffer);
	ang->x += (double)mbx->cursor_delta.x / 256;
	ang->y += (double)mbx->cursor_delta.y / 256;
}

void	place_and_destroy_logic(t_tsr *tsr, t_mbx *mbx,
	t_uniform *uniform, t_type *type)
{
	t_traversal		traversal;

	if (mbx_key_pressed(mbx, MBX_MOUSE_LEFT))
	{
		traversal = ray_traversal(uniform->forward_dir, tsr->pos, tsr);
		block_set(&tsr->world, traversal.block, 0);
	}
	if (mbx_key_pressed(mbx, MBX_MOUSE_RIGHT))
	{
		traversal = ray_traversal(uniform->forward_dir, tsr->pos, tsr);
		traversal.block = vec3i_sub(traversal.block,
			vec3i_vd(get_normal(uniform->forward_dir, traversal.axis)));
		block_set(&tsr->world, traversal.block, *type);
	}
	if (mbx_key_pressed(mbx, MBX_KEY_Q))
	{
		if (*type == 1)
			*type = 6;
		else
			(*type)--;
	}
	if (mbx_key_pressed(mbx, MBX_KEY_E))
		*type = *type % 6 + 1;
}

void	update(t_mbx *mbx, void *args)
{
	const t_mbx_color	background = color(0x101010);
	t_tsr				*tsr;
	t_vec3				mov;
	static char			info[16] = " ";
	static double		mspf = 0.0;
	static size_t		oldframe_elapsed = 0;
	static double		chrono = 0.0;
	t_uniform			uniform;
	static t_type		type = 1;

	tsr = args;
	mov = movement(mbx);
	vec3_yaw_rot(&mov, -tsr->ang.x);
	tsr->pos = vec3_add(tsr->pos, mov);
	mouse_cam(mbx, &tsr->ang);
	uniform.forward_dir = vec3(0.0, 0.0, 1.0);
	vec3_pitch_rot(&uniform.forward_dir, -tsr->ang.y);
	vec3_yaw_rot(&uniform.forward_dir, -tsr->ang.x);
	uniform.right_dir = vec3(1.0, 0.0, 0.0);
	vec3_pitch_rot(&uniform.right_dir, -tsr->ang.y);
	vec3_yaw_rot(&uniform.right_dir, -tsr->ang.x);
	uniform.up_dir = vec3(0.0, 1.0, 0.0);
	vec3_pitch_rot(&uniform.up_dir, -tsr->ang.y);
	vec3_yaw_rot(&uniform.up_dir, -tsr->ang.x);
	uniform.aspect_ratio = (double)(mbx->viewport->size.y)
		/ mbx->viewport->size.x;
	uniform.tsr = tsr;
	place_and_destroy_logic(tsr, mbx, &uniform, &type);
	vec3_yaw_rot(&tsr->light, 1e-1 * mbx->delta_time);
	mbx_clear(mbx->viewport, background);
	fragment_shader(mbx->viewport, rt_shader, &uniform);
	mbx_set_pixel(mbx->viewport,
		vec2i_div_d(mbx->viewport->size, 2), color(0xffffff));
	mbx_set_region_scaled(mbx->viewport, tsr->tex[type],
		vec2i(8, mbx->viewport->size.y - 56), vec2_d(3));
	if (chrono > 1.0)
	{
		snprintf(info, 16, "%.2f ms\n",
			mspf / (mbx->frames_elapsed - oldframe_elapsed));
		mspf = 0.0;
		chrono = 0.0;
		oldframe_elapsed = mbx->frames_elapsed;
	}
	mspf += mbx->seconds_per_frame * 1000;
	chrono += mbx->delta_time;
	mbx_set_text(mbx->viewport, info, vec2i(0, 0), tsr->font);
}

void	world_build(t_world *world)
{
	for (size_t x = 0; x < world->width; x++)
		for (size_t y = 0; y < world->height; y++)
			for (size_t z = 0; z < world->depth; z++)
				block_set(world, vec3i(x, y, z), (y == 0 || y == 15)
					|| ((5 <= x && x <= 10 && 5 <= y && y <= 10 && 5 <= z && z <= 10)
					&& !(((6 <= x && x <= 9) && (6 <= y && y <= 9))
					|| ((6 <= y && y <= 9) && (6 <= z && z <= 9))
					|| ((6 <= x && x <= 9) && (6 <= z && z <= 9))))
				);
}

static void	load_textures(t_tsr *tsr, t_mbx *mbx)
{
	static char *paths[] = {
		"assets/stone.png", "assets/dirt.png",
		"assets/oak_plank.png", "assets/iron_block.png",
		"assets/moss_block.png", "assets/blue_coral_block.png"
	};
	size_t	i;

	tsr->tex[0] = NULL;
	i = 1;
	while (i < 7)
	{
		tsr->tex[i] = mbx_make_region_from_file(mbx, paths[i - 1]);
		if (!tsr->tex[i])
			printf("Can't load texture '%s'\n", paths[i - 1]);
		i++;
	}
	while (i < 256)
		tsr->tex[i++] = NULL;
}

static void	unload_textures(t_tsr *tsr, t_mbx *mbx)
{
	size_t	i;

	i = 0;
	while (i < 256)
	{
		if (tsr->tex[i])
			mbx_destroy_region(mbx, tsr->tex[i]);
		i++;
	}
}

int main()
{
	t_mbx	*mbx;
	t_tsr	tsr;

	tsr.size = vec2i(1280, 720);
	mbx = mbx_init(tsr.size, "Tesser4ct", 0);
	if (!mbx)
	{
		printf("Can't init MacroBox\n");
		return (1);
	}
	tsr.font = mbx_make_region_from_file(
		mbx, "lib/MacroBoX/assets/font_5x7.png");
	tsr.font->subregion_size = vec2i(5, 7);
	load_textures(&tsr, mbx);
	tsr.pos = vec3(8.0, 8.0, 4.0);
	tsr.vel = vec3_zero();
	tsr.ang = vec2_zero();
	world_create(&tsr.world, 16, 16, 16);
	world_build(&tsr.world);
	tsr.light = vec3_normalize(vec3(0.35, 1.0, 0.55));
	mbx->settings.show_cursor = false;
	mbx_refresh_settings(mbx);
	mbx_move_cursor(mbx, vec2i(tsr.size.x / 2, tsr.size.y / 2));
	mbx_run(mbx, update, &tsr);
	mbx_destroy_region(mbx, tsr.font);
	unload_textures(&tsr, mbx);
	mbx_exit(mbx);
	world_destroy(&tsr.world);
}
