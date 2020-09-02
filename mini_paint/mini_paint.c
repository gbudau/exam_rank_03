/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_paint.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gbudau <gbudau@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/02 13:21:59 by gbudau            #+#    #+#             */
/*   Updated: 2020/09/02 13:55:33 by gbudau           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define ERR_ARG "Error: argument\n"
#define ERR_OTHER "Error: Operation file corrupted\n"

typedef struct	s_zone
{
	int		width;
	int		height;
	char	color;
	char	*buffer;
}				t_zone;

typedef struct	s_circle
{
	char	id;
	float	x;
	float	y;
	float	radius;
	char	color;
}				t_circle;

enum			t_position
{
	OUTSIDE,
	INSIDE,
	BORDER
};

size_t		ft_strlen(const char *str)
{
	const char	*start;

	start = str;
	while (*str)
		str++;
	return (str- start);
}

void		print_zone(t_zone zone)
{
	int	y;

	y = 0;
	while (y < zone.height)
	{
		write(1, &zone.buffer[y * zone.width], zone.width);
		write(1, "\n", 1);
		y++;
	}
}

int			check_zone(t_zone *zone)
{
	return (zone->width <= 0 || zone->height <= 0 || zone->width > 300 || zone->height > 300);
}

int			get_zone(t_zone *zone, FILE *file)
{
	if (fscanf(file, "%d %d %c\n", &zone->width, &zone->height, &zone->color) != 3)
		return (1);
	if (check_zone(zone) == 1)
		return (1);
	zone->buffer = malloc(zone->width * zone->height);
	if (zone->buffer == NULL)
		return (1);
	memset(zone->buffer, zone->color, zone->width * zone->height);
	return (0);
}

int			check_circle(t_circle c)
{
	return (c.radius <= 0 || (c.id != 'c' && c.id != 'C'));
}

float		distance_points(float Xa, float Ya, float Xb, float Yb)
{
	return (sqrtf((Xa - Xb) * (Xa - Xb) + (Ya - Yb) * (Ya - Yb)));
}

int			check_point(int x, int y, t_circle c)
{
	float	distance;

	distance = distance_points(x, y, c.x, c.y);
	if (distance <= c.radius)
	{
		if (c.radius - distance < 1)
			return (BORDER);
		return (INSIDE);
	}
	return (OUTSIDE);
}

void		draw_circle(t_zone *zone, t_circle c)
{
	int	y;
	int	x;
	int	ret;

	y = 0;
	while (y < zone->height)
	{
		x = 0;
		while (x < zone->width)
		{
			ret = check_point(x, y, c);
			if (ret == BORDER || (ret == INSIDE && c.id == 'C'))
				zone->buffer[y * zone->width + x] = c.color;
			x++;
		}
		y++;
	}
}

int			draw(t_zone *zone, FILE *file)
{
	t_circle	c;
	int			ret;

	while ((ret = fscanf(file, "%c %f %f %f %c\n",
					&c.id, &c.x, &c.y, &c.radius, &c.color)) == 5)
	{
		if (check_circle(c) == 1)
			return (1);
		draw_circle(zone, c);
	}
	return (ret == EOF ? 0 : 1);
}

int			mini_paint(const char *path)
{
	t_zone	zone;
	int		ret;
	FILE	*file;

	file = fopen(path, "r");
	if (file == NULL)
		return (1);
	if (get_zone(&zone, file) == 1)
	{
		fclose(file);
		return (1);
	}
	ret = draw(&zone, file);
	if (ret == 0)
		print_zone(zone);
	fclose(file);
	free(zone.buffer);
	return (ret);
}

int			main(int argc, char **argv)
{
	if (argc == 2)
	{
		if (mini_paint(argv[1]) == 0)
			return (0);
		write(1, ERR_OTHER, ft_strlen(ERR_OTHER));
	}
	else
		write(1, ERR_ARG, ft_strlen(ERR_ARG));
	return (1);
}
