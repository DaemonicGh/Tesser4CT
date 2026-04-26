/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_s_world.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:14:45 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/24 16:05:51 by rprieur          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_S_WORLD_H
# define TSR_S_WORLD_H

typedef unsigned char	t_tsr_tile;

# define VOID 0
# define SOLID 1

typedef struct s_world
{
	size_t	width;
	size_t	height;
	size_t	depth;
	t_tsr_tile	*blocks;
}	t_world;

#endif
