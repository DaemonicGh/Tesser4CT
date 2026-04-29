/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_constants.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 17:59:03 by rprieur           #+#    #+#             */
/*   Updated: 2026/04/26 21:50:34 by rprieur          ###   ########.fr       */
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

# define TILE_COUNT				256

# define RENDER_JOB_REGION_SIZE	16

# define CAMERA_SENSITIVITY		2

#endif
