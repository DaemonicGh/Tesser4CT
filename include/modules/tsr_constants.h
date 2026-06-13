/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_constants.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 17:59:03 by rprieur           #+#    #+#             */
/*   Updated: 2026/06/10 11:42:35 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_CONSTANTS_H
# define TSR_CONSTANTS_H
# pragma once

# include "modules/mbx_constants.h"

# ifndef TSR_DEBUG
#  define TSR_DEBUG	0
# endif

# define TSR_NAME				"Tesser4CT"

# define DEFAULT_VIEWPORT_W		640
# define DEFAULT_VIEWPORT_H		360
# define DEFAULT_WINDOW_TITLE	TSR_NAME
# define DEFAULT_WINDOW_FLAGS	MBX_WINDOW_FLAG_NONE

# define DEFAULT_REGION_PATH	"assets/default.png"

# define TILE_BUFFER_SIZE		256
# define TEXTURE_BUFFER_SIZE	256

# define RENDER_JOB_REGION_W	32
# define RENDER_JOB_REGION_H	18

# define PROMPT_SIZE			64
# define MAP_SELECT_PREFIX		"maps/"
# define MAP_SELECT_PREFIX_LEN	5
# define MAP_SELECT_SUFFIX		".mlem"
# define MAP_SELECT_SUFFIX_LEN	5

# define MAP_INFO_INPUT			"Enter map name:"
# define MAP_INFO_ERROR			"ERROR: Unable to open map"

# define CAMERA_SENSITIVITY		2

#endif
