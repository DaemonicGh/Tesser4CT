/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_context.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 22:12:13 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/12 02:45:47 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "tsr_world.h"
#include "tsr_atlas.h"

typedef struct s_tsr_player
{
	t_vec3			position;
	t_vec3			rotation;
	t_vec3			hitbox;
	t_vec3			right;
	t_vec3			up;
	t_vec3			forward;
	t_vec3			velocity;
	t_tsr_tile_id	tile_id;
}	t_tsr_player;

typedef struct s_tsr_camera
{
	t_vec3		position;
	t_vec3		rotation;
	t_vec3		right;
	t_vec3		up;
	t_vec3		forward;
	t_vec3		velocity;
}	t_tsr_camera;

typedef struct s_tsr_context
{
	t_mbx				*mbx;
	struct	s_tsr_rendering_manager
	{
		t_mbx_region		*target;
		size_t				thread_count;
		pthread_t			*threads;
		pthread_barrier_t	wait_barrier;
		atomic_bool			running;
		size_t				job_count;
		atomic_size_t		current_job;
		t_vec2i				job_region_count;
		t_mbx_color			(*frag_shader)(t_tsr * tsr, t_vec2 uv);
	}	rendering;
	struct	s_tsr_ui_manager
	{
		t_mbx_region		*target;
		struct s_tsr_fonts
		{
			t_mbx_atlas			*small;
		}					fonts;
		struct s_tsr_gui
		{
			t_mbx_region			*hotbar;
			t_mbx_region			*hotbar_selection;
			double					hotbar_offset;
			double					hotbar_delta;
		}					gui;
	}	ui;
	struct s_tsr_world_manager
	{
		t_tsr_tile			tiles[TILE_BUFFER_COUNT];
		t_vec3				global_light;
		pthread_t			loader_thread;
		pthread_barrier_t	loader_wait_barrier;
	}	world;
	t_world				wworld;
	t_atlas				atlas;
	t_mbx_region		*nmap;
	t_tsr_player		player;
	t_tsr_camera		camera;
	struct s_tsr_extras
	{
		t_mbx_region		*default_region;
		double				aspect_ratio;
	}	extras;
}	t_tsr;
