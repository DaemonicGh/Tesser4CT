/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tsr_renderer.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emarrot <emarrot@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/15 16:33:47 by emarrot           #+#    #+#             */
/*   Updated: 2026/04/24 17:15:16 by emarrot          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TSR_RENDERER_H
# define TSR_RENDERER_H

# include "mbx.h"
# include "types/tsr_s_tsr.h"
# include "modules/tsr_world.h"

typedef struct s_traversal
{
	int		axis;
	double	dist;
	t_vec3i	block;
}	t_traversal;

typedef struct s_shader
{
	t_mbx_region	*buffer;
	t_mbx_color		(*f)(t_vec2, void *);
	void			*data;
}	t_shader;

typedef struct s_job
{
	t_vec2i	start_coord;
}	t_job;

typedef struct s_thread_pkg
{
	pthread_t		thread;
	pthread_mutex_t	*mut;
	size_t			*job_len;
	t_job			*jobs;
	const t_shader	*shader;
}	t_thread_pkg;

typedef struct s_uniform
{
	t_vec3	forward_dir;
	t_vec3	right_dir;
	t_vec3	up_dir;
	double	aspect_ratio;
	t_tsr	*tsr;
}	t_uniform;

void	vec3_yaw_rot(t_vec3 *v, double ang);
void	vec3_pitch_rot(t_vec3 *v, double ang);
void	fragment_shader(t_mbx_region *buffer,
	t_mbx_color (*f)(t_vec2, void *), void *data);
t_mbx_color rt_shader(t_vec2 uv, void *uniform);
t_traversal	ray_traversal(t_vec3 ray_dir, t_vec3 ray_org, t_tsr *tsr);
t_vec3	get_normal(t_vec3 ray_dir, int axis);

#endif
