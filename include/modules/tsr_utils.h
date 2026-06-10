/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rprieur <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 17:15:03 by rprieur           #+#    #+#             */
/*   Updated: 2026/05/27 18:00:58 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "mbx.h"

typedef enum e_tsr_report_status
{
	STATUS_INFO		= 0,
	STATUS_DEBUG	= -1,
	STATUS_WARNING	= 1,
	STATUS_ERROR	= 2,
}	t_tsr_status;

#define ANSI_INFO			"\x1B[39m"
#define ANSI_DEBUG			"\x1B[30m"
#define ANSI_WARNING		"\x1B[1;33m"
#define ANSI_ERROR			"\x1B[1;31m"
#define ANSI_RESET			"\x1B[0m"

#define REPORT_SUCCESS		"Exited successfully"

#define REPORT_DEBUG_ON		"Launched in debug mode"

#define REPORT_MEMORY		"Heap allocation failed"
#define REPORT_NULLMBX		"Failed to initialize MacroBoX"
#define REPORT_NULLMBXWIN	"Failed to initialize the MacroBoX window"
#define REPORT_NULLDEFIMGF	"Failed to load the default image file"
#define REPORT_NULLIMGF		"Failed to load image file at "
#define REPORT_NULLTHREAD	"Failed to create thread"
#define REPORT_MAPSAVED		"Sucessfully saved map at "

t_vec3
get_tile_normal(t_vec3 ray_dir, int axis);

int
tsr_report(t_tsr_status status, const char *message);

int
tsr_report_m(t_tsr_status status, const char *message, const char *message2);

void
*tsr_batch_free(size_t ptr_count, ...);

void
*tsr_memreport(void *pointer);

size_t
tsr_strlen(const char *s);

int
tsr_strcmp(const char *s1, const char *s2);

char
*tsr_strcat(char *dst, const char *src);

char
*tsr_strdup(const char *src);

char
tsr_tolower(char c);
