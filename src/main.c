/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 16:38:53 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/15 11:45:49 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include "mbx.h"

typedef unsigned int t_u32;
typedef unsigned char t_u8;

typedef struct s_glb
{
	t_vec2i		size;
	t_mbx_atlas	font;
	t_vec3		pos;
	t_vec3		vel;
	t_vec2		ang;
	size_t		order;
	t_u8		*blocks;
}	t_glb;

void	vec3_yaw_rot(t_vec3 *v, double ang)
{
	const double	x = v->x;
	const double	cosa = cos(ang);
	const double	sina = sin(ang);

	v->x = x * cosa - v->z * sina;
	v->z = x * sina + v->z * cosa;
}

void	vec3_pitch_rot(t_vec3 *v, double ang)
{
	const double	z = v->z;
	const double	cosa = cos(ang);
	const double	sina = sin(ang);

	v->z = z * cosa - v->y * sina;
	v->y = z * sina + v->y * cosa;
}

t_u8	block_get(t_glb *glb, t_vec3i block)
{
	return (glb->blocks[
		block.x + glb->order * (block.y + glb->order * block.z)
	]);
}

void	block_set(t_glb *glb, t_vec3i block, t_u8 type)
{
	glb->blocks[block.x + glb->order * (block.y + glb->order * block.z)] = type;
}

bool	inbound(t_glb *glb, t_vec3i block)
{
	return (
		0 <= block.x && block.x < (int)glb->order
		&& 0 <= block.y && block.y < (int)glb->order
		&& 0 <= block.z && block.z < (int)glb->order
	);
}

int		step_tdist(t_vec3 *tDist, t_vec3 *tDelta, t_vec3i *block, t_vec3i *sgn)
{
	if (tDist->x < tDist->y)
	{
		if (tDist->x < tDist->z)
		{
			tDist->x += tDelta->x;
			block->x += sgn->x;
			return (0);
		}
	}
	else if (tDist->y < tDist->z)
	{
		tDist->y += tDelta->y;
		block->y += sgn->y;
		return (1);
	}
	tDist->z += tDelta->z;
	block->z += sgn->z;
	return (2);
}

void	ray_traversal(
	t_mbx_region *buffer, t_vec2i coord, t_vec3 rayDir, t_glb *glb)
{
	t_vec3	tDelta;
	t_vec3	tDist;
	t_vec3i	block;
	t_vec3i	sgn;
	int		axis;
	int		hex_color;

	//rayDir = vec3_normalize(rayDir);
	tDelta = vec3_div_rd(1.0, rayDir);
	sgn = vec3i(1, 1, 1);
	if (tDelta.x < 0)
		sgn.x = -1;
	if (tDelta.y < 0)
		sgn.y = -1;
	if (tDelta.z < 0)
		sgn.z = -1;
	tDist = vec3(
		clamp(floor(glb->pos.x) + 1, 1, glb->order - 2) - glb->pos.x,
		clamp(floor(glb->pos.y) + 1, 1, glb->order - 2) - glb->pos.y,
		clamp(floor(glb->pos.z) + 1, 1, glb->order - 2) - glb->pos.z
	);
	if (sgn.x == -1)
		tDist.x = glb->pos.x - clamp(floor(glb->pos.x), 1, glb->order - 2);
	if (sgn.y == -1)
		tDist.y = glb->pos.y - clamp(floor(glb->pos.y), 1, glb->order - 2);
	if (sgn.z == -1)
		tDist.z = glb->pos.z - clamp(floor(glb->pos.z), 1, glb->order - 2);
	tDelta = vec3_abs(tDelta);
	tDist = vec3_mult(tDist, tDelta);
	block = vec3i(
		clamp(floor(glb->pos.x), 0, glb->order - 1),
		clamp(floor(glb->pos.y), 0, glb->order - 1),
		clamp(floor(glb->pos.z), 0, glb->order - 1));
	while (inbound(glb, block) && !block_get(glb, block))
		axis = step_tdist(&tDist, &tDelta, &block, &sgn);
	/*tDist = vec3_abs(tDist);
	int depth = (int [3]){
		tDist.x - tDelta.x, tDist.y - tDelta.y, tDist.z - tDelta.z}[axis];
	depth = clamp(depth * 8.0, 0, 255);
	if (!inbound(glb, block))
		depth = 255;
	hex_color = depth << 16 | depth << 8 | depth;*/
	hex_color = (int [3]){0xff0000, 0x00ff00, 0x0000ff}[axis];
	if (!inbound(glb, block))
		hex_color = 0x000000;
	mbx_set_pixel_raw(buffer, coord, color(hex_color));
}

void	setup_frag(t_mbx_region *buffer, t_glb *glb)
{
	const double	ratio = (double)buffer->size.y / buffer->size.x;
	t_vec3			rayDir;
	t_vec3			rayDirTmp;
	t_vec3			dxy[2];
	t_vec2i			frag;

	rayDir = vec3(-1.0, ratio, 1.0);
	vec3_pitch_rot(&rayDir, -glb->ang.y);
	vec3_yaw_rot(&rayDir, -glb->ang.x);
	dxy[0] = vec3(2.0 / buffer->size.x, 0.0, 0.0);
	dxy[1] = vec3(0.0, -2.0 * ratio / buffer->size.y, 0.0);
	vec3_pitch_rot(dxy + 0, -glb->ang.y);
	vec3_yaw_rot(dxy + 0, -glb->ang.x);
	vec3_pitch_rot(dxy + 1, -glb->ang.y);
	vec3_yaw_rot(dxy + 1, -glb->ang.x);
	frag.y = 0;
	while (frag.y < buffer->size.y)
	{
		rayDirTmp = rayDir;
		frag.x = 0;
		while (frag.x < buffer->size.x)
		{
			ray_traversal(buffer, frag, rayDir, glb);
			rayDir = vec3_add(rayDir, dxy[0]);
			frag.x++;
		}
		rayDir = vec3_add(rayDirTmp, dxy[1]);
		frag.y++;
	}
}

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
		mbx->viewport.size.x / 2, mbx->viewport.size.y / 2);

	mbx_warp_cursor(mbx, midbuffer);
	ang->x += (double)mbx->cursor_delta.x / 256;
	ang->y += (double)mbx->cursor_delta.y / 256;
}

void	update(t_mbx *mbx, void *args)
{
	const t_mbx_color	background = color(0x101010);
	t_glb				*glb;
	t_vec3				mov;
	static char			info[16] = " ";
	static double		mspf = 0.0;
	static size_t		oldframe_elapsed = 0;
	static double		chrono = 0.0;

	glb = args;
	mov = movement(mbx);
	vec3_yaw_rot(&mov, -glb->ang.x);
	glb->pos = vec3_add(glb->pos, mov);
	mouse_cam(mbx, &glb->ang);
	mbx_clear(&mbx->viewport, background);
	setup_frag(&mbx->viewport, glb);
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
	mbx_set_text(&mbx->viewport, info, vec2i(0, 0), &glb->font);
}

void	*ft_memset(void *p, int c, size_t n)
{
	t_u8	fill;
	t_u8	*u8p;
	size_t	i;

	u8p = p;
	fill = c & 255;
	i = 0;
	while (i < n)
		u8p[i++] = fill;
	return (p);
}

void	world_build(t_glb *glb)
{
	for (size_t x = 0; x < glb->order; x++)
		for (size_t y = 0; y < glb->order; y++)
			for (size_t z = 0; z < glb->order; z++)
				block_set(glb, vec3i(x, y, z),
					(5 <= x && x <= 10 && 5 <= y && y <= 10 && 5 <= z && z <= 10)
					&& !(((6 <= x && x <= 9) && (6 <= y && y <= 9))
					|| ((6 <= y && y <= 9) && (6 <= z && z <= 9))
					|| ((6 <= x && x <= 9) && (6 <= z && z <= 9)))
				);
}

int main()
{
	t_mbx			*mbx;
	t_mbx_region	atlas;
	t_glb			glb;

	glb.size = vec2i(640, 360);
	mbx = mbx_init(glb.size, "Tesser4ct", 0);
	if (!mbx)
	{
		printf("Can't init MacroBox\n");
		return (1);
	}
	atlas = mbx_make_region_from_file(mbx, "lib/MacroBoX/assets/font_5x7.png"); 
	glb.font = mbx_make_atlas(atlas, vec2i(5, 7));
	glb.order = 16;
	glb.pos = vec3(8.0, 8.0, 4.0);
	glb.vel = vec3_zero();
	glb.ang = vec2_zero();
	glb.blocks = malloc(glb.order * glb.order * glb.order * sizeof(t_u8));
	if (!glb.blocks)
	{
		printf("Can't allocate blocks\n");
		mbx_exit(mbx);
		return (1);
	}
	ft_memset(glb.blocks, 0, glb.order * glb.order * glb.order * sizeof(t_u8));
	world_build(&glb);	
	mbx->settings.show_cursor = false;
	mbx_refresh_settings(mbx);
	mbx_move_cursor(mbx, vec2i(glb.size.x / 2, glb.size.y / 2));
	mbx_run(mbx, update, &glb);
	mbx_destroy_atlas(mbx, &glb.font);
	mbx_exit(mbx);
	free(glb.blocks);
}
