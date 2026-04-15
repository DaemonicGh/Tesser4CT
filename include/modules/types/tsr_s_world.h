/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_s_world.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 15:14:45 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/15 15:20:29 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_S_WORLD_H
# define TSR_S_WORLD_H

typedef unsigned char	t_type;

# define VOID 0
# define SOLID 1

typedef struct s_world
{
	size_t	width;
	size_t	height;
	size_t	depth;
	t_type	*blocks;
}	t_world;

#endif
