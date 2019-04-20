#! /usr/bin/env python3

import argparse
import array
import os
import sys

from math import sin, cos, pi, radians
from typing import List, Tuple, Callable

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = ""

import numpy as np
import pygame

from transformation import Transformation


def within_ascii_bounds(point: np.array) -> bool:
    if 0 <= point[0] <= 127 and 0 <= point[1] <= 127:
        return True
    else:
        return False


def make_circular(point):
    r, theta, z = point
    theta = radians(theta)
    x, y = r * cos(theta), r * sin(theta)
    return x, y, z


def make_spherical(point):
    r, theta, phi = point
    theta = radians(theta)
    phi = radians(phi)
    x, y, z = r * cos(theta) * sin(phi), r * sin(theta) * sin(phi), r * cos(theta)
    return x, y, z


def make_cylindrical(point):
    r, theta, z = point
    theta = radians(theta)
    x, y, z = r * cos(theta), r * sin(theta), z
    return x, y, z


def make_render_matrix(shape: str, points: np.array, transformation: Transformation) -> dict:
    render_matrix = { 'colors': np.array(np.zeros(points.shape)) }

    for i in range(len(points)):
        if within_ascii_bounds(points[i]):
            render_matrix['colors'][i] = (255, 165, 0)
        else:
            render_matrix['colors'][i] = (255, 255, 255)

    if shape == 'square':
        render_matrix['points'] = points.copy()
    if shape == 'circular':
        render_matrix['points'] = np.array([make_circular(point) for point in points])
    if shape == 'cubic':
        render_matrix['points'] = points.copy()
    if shape == 'spherical':
        render_matrix['points'] = np.array([make_spherical(point) for point in points])
    if shape == 'cylindrical':
        render_matrix['points'] = np.array([make_cylindrical(point) for point in points])

    render_matrix['points'] = np.array([transformation.multiply_with_vector(row)
                                        for row in render_matrix['points']])

    return render_matrix


def project(point, screen_translation) -> Tuple[int, int]:
    x, y, z = point
    dx, dy, dz = screen_translation
    x1 = (x * dz / (dz + z)) + dx
    y1 = (y * dz / (dz + z)) + dy
    return int(x1), int(y1)


def make_renderables(render_matrix: dict, screen_translation: tuple) -> list:
    renderables = list()
    for i in range(len(render_matrix['points'])):
        point = render_matrix['points'][i]
        x, y = project(point, screen_translation)
        renderables.append((x, y, render_matrix['colors'][i]))
    return renderables


def render(renderables, surface):
    for renderable in renderables:
        x, y, color = renderable
        surface.set_at((int(x), int(y)), color)


def rotate_x(d) -> np.array:
    t = radians(d)
    return Transformation( ((   1   ,    0   ,    0   ,   0   ),
                            (   0   ,  cos(t), -sin(t),   0   ),
                            (   0   ,  sin(t),  cos(t),   0   ),
                            (   0   ,    0   ,    0   ,   1   )) )


def rotate_y(d) -> np.array:
    t = radians(d)
    return Transformation( (( cos(t),    0   ,  sin(t),   0   ),
                            (   0   ,    1   ,    0   ,   0   ),
                            (-sin(t),    0   ,  cos(t),   0   ),
                            (   0   ,    0   ,    0   ,   1   )) )


def rotate_z(d) -> np.array:
    t = radians(d)
    return Transformation( (( cos(t), -sin(t),    0   ,   0   ),
                            ( sin(t),  cos(t),    0   ,   0   ),
                            (   0   ,    0   ,    1   ,   0   ),
                            (   0   ,    0   ,    0   ,   1   )) )


def main(args: list) -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', '--file', default=0)
    parser.add_argument('-e', '--extent', default=-1, type=int)
    parser.add_argument('-s', '--shape', choices={'linear', 'square', 'circular', 'cubic', 'spherical', 'cylindrical'}, default='square')
    parser.add_argument('-b', '--filetype', choices={'string', 'binary'}, default='binary')
    parser.add_argument('-t', '--datatype', choices={'int', 'float'}, default='float')
    parser.add_argument('-l', '--slide', type=int, default=1)
    parser.add_argument('-g', '--gap', type=int, default=1)
    parsedargs = parser.parse_args(args[1:])

    filename = parsedargs.file
    extent = parsedargs.extent
    shape = parsedargs.shape
    filetype = parsedargs.filetype
    slide = parsedargs.slide
    gap = parsedargs.gap

    WINDOW_WIDTH: int = 800
    WINDOW_HEIGHT: int = 600

    pygame.init()
    pygame.display.set_caption(f"View {filename}")
    pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))

    surface = pygame.display.get_surface()

    zoom = 1.0
    distance = 1000
    xshift = WINDOW_WIDTH / 2
    yshift = WINDOW_HEIGHT / 2
    extent_s = ''
    changed = True

    # TODO: read in as strings
    if filetype == 'binary':
        with open(filename, 'rb') as bf:
            bdata: bytes = bf.read()
            darray = array.array('B')
            darray.frombytes(bdata)
            data = darray.tolist()
    elif filetype == 'string':
        with open(filename, 'r') as f:
            fdata: str = f.read()
            data = list(map(int, fdata.split()))

    def zeros():
        while True:
            yield 0

    # map data
    if shape == 'linear':
        points = np.array(tuple(zip(data[:],  zeros(),  zeros())), dtype='float64')
    if shape == 'square':
        points = np.array(tuple(zip(data[:], data[1:],  zeros())), dtype='float64')
    if shape == 'circular':
        points = np.array(tuple(zip(data[:], data[1:],  zeros())), dtype='float64')
    if shape == 'cubic':
        points = np.array(tuple(zip(data[:], data[1:], data[2:])), dtype='float64')
    if shape == 'spherical':
        points = np.array(tuple(zip(data[:], data[1:], data[2:])), dtype='float64')
    if shape == 'cylindrical':
        points = np.array(tuple(zip(data[:], data[1:], data[2:])), dtype='float64')

    transformation = Transformation()

    running = True
    while running:
        for event in pygame.event.get():
            changed = True
            #print(event)
            if event.type == pygame.QUIT:
                running = False
                break

        if event.type == pygame.KEYDOWN:
            # fov
            if event.key == pygame.K_EQUALS:
                distance += 500
            if event.key == pygame.K_MINUS:
                if distance > 500:
                    distance -= 500

            # rotations
            if event.key == pygame.K_w:
                transformation *= rotate_x(-15)
            if event.key == pygame.K_s:
                transformation *= rotate_x( 15)
            if event.key == pygame.K_a:
                transformation *= rotate_y(-15)
            if event.key == pygame.K_d:
                transformation *= rotate_y( 15)
            if event.key == pygame.K_q:
                transformation *= rotate_z( 15)
            if event.key == pygame.K_e:
                transformation *= rotate_z(-15)

            # translations
            if event.key == pygame.K_UP:
                ...
            if event.key == pygame.K_DOWN:
                ...
            if event.key == pygame.K_LEFT:
                ...
            if event.key == pygame.K_RIGHT:
                ...

        screen_translation = (xshift, yshift, distance)
        if changed:
            changed = False

            render_matrix = make_render_matrix(shape, points, transformation)
            renderables = make_renderables(render_matrix, screen_translation)

            render(renderables, surface)
            pygame.display.flip()
            surface.fill((0,0,0))

        '''
            if event.key == pygame.K_EQUALS:
                if zoom >= 1:
                    zoom += 1
                else:
                    zoom += 0.1
            elif event.key == pygame.K_MINUS:
                if zoom > 1:
                    zoom -= 1
                else:
                    zoom -= 0.1
            elif event.key == pygame.K_UP:
                yshift += abs(8 * zoom)
            elif event.key == pygame.K_DOWN:
                yshift -= abs(8 * zoom)
            elif event.key == pygame.K_LEFT:
                xshift += abs(8 * zoom)
            elif event.key == pygame.K_RIGHT:
                xshift -= abs(8 * zoom)
            elif event.key == pygame.K_w:
                x = rotation[0]
                x += 0.1
                # x %= 2
                rotation[0] = x
            elif event.key == pygame.K_s:
                x = rotation[0]
                x -= 0.1
                # x %= 2
                rotation[0] = x
            elif event.key == pygame.K_a:
                y = rotation[1]
                y -= 0.1
                # y %= 2
                rotation[1] = y
            elif event.key == pygame.K_d:
                y = rotation[1]
                y += 0.1
                # y %= 2
                rotation[1] = y
            elif event.key == pygame.K_LEFTBRACKET and pygame.key.get_mods() & pygame.KMOD_SHIFT:
                if gap > 1:
                    gap -= 1
            elif event.key == pygame.K_RIGHTBRACKET and pygame.key.get_mods() & pygame.KMOD_SHIFT:
                gap += 1
            elif event.key == pygame.K_LEFTBRACKET:
                if slide > 1:
                    slide -= 1
            elif event.key == pygame.K_RIGHTBRACKET:
                slide += 1
            elif pygame.key.get_mods() & pygame.KMOD_SHIFT and event.key == pygame.K_1:
                    shape = 'linear-extent'
                    extent_s = ''
            elif pygame.key.get_mods() & pygame.KMOD_SHIFT and event.key == pygame.K_2:
                shape = 'square'
            elif pygame.key.get_mods() & pygame.KMOD_SHIFT and event.key == pygame.K_3:
                shape = 'circular'
            elif pygame.key.get_mods() & pygame.KMOD_SHIFT and event.key == pygame.K_4:
                shape = 'cubic'
            elif pygame.key.get_mods() & pygame.KMOD_SHIFT and event.key == pygame.K_5:
                shape = 'spherical'
            elif pygame.key.get_mods() & pygame.KMOD_SHIFT and event.key == pygame.K_6:
                shape = 'cylindrical'
            # elif event.key == pygame.K_q:
                # running = False

            if shape == 'linear-extent':
                if event.key == pygame.K_0:
                    extent_s += '0'
                elif event.key == pygame.K_1:
                    extent_s += '1'
                elif event.key == pygame.K_2:
                    extent_s += '2'
                elif event.key == pygame.K_3:
                    extent_s += '3'
                elif event.key == pygame.K_4:
                    extent_s += '4'
                elif event.key == pygame.K_5:
                    extent_s += '5'
                elif event.key == pygame.K_6:
                    extent_s += '6'
                elif event.key == pygame.K_7:
                    extent_s += '7'
                elif event.key == pygame.K_8:
                    extent_s += '8'
                elif event.key == pygame.K_9:
                    extent_s += '9'
                elif event.key == pygame.K_RETURN:
                    extent = int(extent_s)
                    extent_s = ''
        '''

        '''
        if changed:
            changed = False
            transform:list = []
            if shape == 'linear-extent' and extent > 0:
                transform = transform_data_extent(data, extent)
            elif shape == 'square':
                transform = transform_data_square(data, slide, gap)
            elif shape == 'circular':
                transform = transform_data_circular(data, slide, gap)
            elif shape == 'cubic':
                transform = transform_data_cubic(data, slide, gap, rotation, zoom)
                draw_guide_cube(surface, xshift, yshift, rotation, zoom)
            elif shape == 'spherical':
                transform = transform_data_spherical(data, slide, gap, rotation, zoom)
            elif shape == 'cylindrical':
                transform = transform_data_cylindrical(data, slide, gap, rotation, zoom)

            # apply zoom and shift
            transform = general_transform(transform, zoom, xshift, yshift)

            # trim data which does not fit
            transform = trim(transform, WINDOW_WIDTH, WINDOW_HEIGHT)

            # DRAW DATA
            draw_points(surface, transform)
            pygame.display.flip()
            surface.fill((0,0,0))
        '''
    return 0

if __name__ == '__main__':
  main(sys.argv)
