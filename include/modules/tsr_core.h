/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_core.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/26 22:12:13 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/28 21:52:53 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdatomic.h>

#include "mlem.h"
#include "mbx.h"
#include "tsr_constants.h"

typedef uint8_t					t_tsr_tile_id;
typedef uint16_t				t_tsr_chunk_id;
typedef struct s_tsr_context	t_tsr;

typedef t_mbx_atlas *restrict	t_tsr_atlas;

typedef struct s_tsr_pbr_texture_atlas
{
	enum e_tsr_texture_flags: uint8_t
	{
		TX_NONE			= 0x00,
		TX_NORMAL		= 0x01,
		TX_BUMP			= 0x02,
		TX_PBR			= 0x04,
	}				flags;
	t_tsr_atlas		texture[MIPMAP_LEVELS];
	double			anim_speed;
}	t_tsr_texture;

typedef struct s_tsr_tile_data
{
	t_tsr_texture	*texture[6];
	char			*name;
	t_tsr_tile_id	interact_to;
	t_tsr_tile_id	timeout_to;
	double			timeout_time;
	t_vec3			light;
	bool			skip;
	bool			skybox;
	bool			transparent;
	bool			cast_shadow;
	bool			backface;
}	t_tsr_tile_data;

typedef struct s_tsr_tile
{
	t_tsr_tile_id	type;
	uint8_t			rotation;
	t_vec3			light;
	t_vec3			prev_light;
	double			timer;
}	t_tsr_tile;

typedef struct s_tsr_chunk
{
	t_tsr_tile		tiles[64];
	t_tsr_chunk_id	neighbors[6];
	t_tsr_tile		limits[6];
}	t_tsr_chunk;

typedef struct s_tsr_render_ray
{
	t_vec3			origin;
	t_vec3			dir;
	t_vec3			delta;
	t_vec3			abs_delta;
	t_vec3i			dir_sign;
	t_vec3i			iter;
	t_vec3			dist;
	double			distance;
	t_vec3			position;
	t_tsr_chunk_id	chunk;
	t_tsr_chunk_id	prev_chunk;
	t_vec3i			tile_position;
	t_vec3i			prev_tile_position;
	int				tile_index;
	int				lifetime;
	int8_t			axis;
	int8_t			tile_axis;
	t_vec3i			tile_normal;
	int8_t			face;
	bool			backface;
	t_tsr_tile		*tile;
	t_tsr_tile_data	*tile_data;
	t_tsr_tile		*prev_tile;
	t_vec2			uv;
	t_tsr_texture	*texture;
	t_vec3			normal;
	uint8_t			mipmap;
	t_mbx_color		color;
}	t_tsr_ray;

typedef struct s_tsr_player
{
	t_vec3			rotation;
	t_vec3			forward;
	t_vec3			right;
	t_vec3			up;
	t_vec3			velocity;
	t_tsr_chunk_id	chunk;
	t_vec3			position;
	t_vec3x2		hitbox;
	double			air_time;
	bool			can_jump;
	bool			is_jumping;
	t_tsr_tile		hotbar_tile;
	enum	e_tsr_player_tool: uint
	{
		TOOL_CREATE_CHUNK,
		TOOL_HIGHLIGHT_LIMITS,
		TOOL_SET_LIMIT,
		TOOL_TELEPORT,
		TOOL_SKYLIGHT,
		TOOL_TRASH_CHUNKS,
		TOOL_SAVE_MAP,
	}				hotbar_tool;
	t_vec3i			face;
	t_tsr_ray		ray;
	t_tsr_chunk_id	tile_highlight_chunk;
	t_vec3i			tile_highlight_pos;
	int				tile_highlight_axis;
	enum	e_tsr_game_prompt_state: int
	{
		PROMPT_STATE_NONE,
		PROMPT_STATE_CHUNK,
		PROMPT_STATE_TP,
		PROMPT_STATE_SAVE,
	}				prompt_state;
	double			speed;
	bool			on_toolbar;
	bool			godmode;
}	t_tsr_player;

typedef struct s_tsr_camera
{
	t_vec3			rotation;
	t_vec3			view_rotation;
	t_vec3			right;
	t_vec3			up;
	t_vec3			forward;
	t_vec3			velocity;
	t_tsr_chunk_id	chunk;
	t_vec3			position;
}	t_tsr_camera;

typedef struct s_tsr_world_data
{
	t_tsr_tile_data		tiles[TILE_BUFFER_SIZE];
	size_t				tile_count;
	t_mlem_value		mlem;
	t_mlem_string		name;
	t_vec3				skylight;
	t_vec3				ambient_color;
	t_tsr_chunk_id		origin;
	t_tsr_tile			skybox;
}	t_tsr_world_data;

typedef struct s_tsr_world_content
{
	t_tsr_chunk			*chunks;
	size_t				chunk_count;
	size_t				chunk_capacity;
	t_tsr_world_data	*data;
}	t_tsr_world;

typedef struct s_tsr_button
{
	t_vec2i		position;
	const char	*message;
	bool		hovered;
	bool		clicked;
	bool		visible;
}	t_tsr_button;

typedef struct s_tsr_toggle
{
	t_vec2i		position;
	const char	*message;
	bool		value;
	bool		hovered;
	bool		visible;
}	t_tsr_toggle;

typedef struct s_tsr_slider
{
	t_vec2i		position;
	const char	*message;
	double		value;
	double		bounds[2];
	double		step;
	bool		hovered;
	bool		visible;
}	t_tsr_slider;

typedef struct s_tsr_prompt
{
	t_vec2i				position;
	const char			*message;
	char				buffer[PROMPT_SIZE];
	double				shake;
	uint32_t			cursor;
	bool				visible;
	bool				is_error;
}	t_tsr_prompt;

typedef struct s_tsr_hotbar
{
	double				offset;
	double				delta;
}	t_tsr_hotbar;

typedef struct s_tsr_context
{
	t_mbx				*mbx;
	struct	s_tsr_texture_manager
	{
		t_tsr_texture		textures[TEXTURE_BUFFER_SIZE];
		t_mlem_value		mlem;
		size_t				count;
		t_mbx_region		*default_region;
		t_mbx_region		*tile_highlight;
		t_mbx_region		*tile_face_highlight;
		struct s_tsr_gui_textures
		{
			t_mbx_region		*hotbar_selection;
			t_mbx_region		*minimap_cursor;
			t_mbx_region		*mouse_click;
			t_mbx_atlas			*bar;
			t_mbx_atlas			*button;
			t_mbx_atlas			*toggle;
			t_mbx_atlas			*slider;
		}					gui;
		t_mbx_region		*tools[8];
		t_mbx_atlas			*font_small;
		t_mbx_atlas			*font_title;
	}	textures;
	struct	s_tsr_rendering_manager
	{
		t_mbx_region		*target;
		t_mbx_region		*swap_target;
		size_t				thread_count;
		pthread_t			*threads;
		pthread_barrier_t	wait_barrier;
		atomic_uint			threads_waiting;
		atomic_bool			cross_cycle;
		atomic_bool			running;
		size_t				job_count;
		atomic_size_t		current_job;
		t_vec2i				job_region_count;
		t_mbx_color			(*frag_shader)(t_tsr * tsr, t_vec2i frag_pos);
		struct s_tsr_rendering_data
		{
			t_tsr_camera		camera;
			t_tsr_world			world;
			t_mbx_color			(*frag_shader)(t_tsr * tsr, t_vec2i frag_pos);
		}	data;
	}	rendering;
	struct	s_tsr_ui_manager
	{
		t_mbx_region		*target;
		t_vec2i				cursor;
		t_tsr_prompt		prompt;
		t_tsr_button		buttons[3];
		t_tsr_slider		sliders[12];
		t_tsr_toggle		toggles[2];
		enum	e_tsr_ui_state: int
		{
			UI_STATE_GAME,
			UI_STATE_MAIN,
			UI_STATE_PAUSE,
			UI_STATE_SETTING
		}					state;
		t_tsr_hotbar		hotbar;
		t_tsr_hotbar		tool_hotbar;
	}	ui;
	t_tsr_world_data	world_data;
	t_tsr_world			world;
	t_tsr_player		player;
	t_tsr_camera		camera;
	struct s_tsr_extras
	{
		double				aspect_ratio;
		double				fov;
		double				focal_length;
		double				camera_sensitivity;
		int					render_scale;
		int					render_distance;
		int					shadow_distance;
		int					skybox_distance;
		bool				show_chunks;
		bool				toggle_toolbar;
		bool				cross_fill;
	}	extras;
}	t_tsr;
