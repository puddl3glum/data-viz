#! /usr/bin/env python3

import argparse
import array
import itertools
import os
import random
import sys

from math import sin, cos, pi, radians
from typing import List, Tuple

os.environ["PYGAME_HIDE_SUPPORT_PROMPT"] = ""

import numpy
# import sdl2
# import sdl2.ext
import pygame

# BLACK = sdl2.ext.Color(0, 0, 0)
# WHITE = sdl2.ext.Color(255, 255, 255)

def general_transform(points, zoom, xshift, yshift) -> List[tuple]:

  transform: list = []

  for point in points:

    x, y, brightness = point

    # apply zoom
    x, y = x * zoom, y * zoom

    # apply shift
    x, y, = x + xshift, y + yshift

    # convert to ints
    x, y = int(x), int(y)

    transform.append((x, y, brightness))

  return transform

def trim(points, width, height) -> List[tuple]:

  points = filter(lambda p: 0 < p[0] <= width, points)
  points = filter(lambda p: 0 < p[1] <= height, points)

  return list(points)

def draw_points(surface, points) -> int:

  for point in points:

    x, y, color = point

    surface.set_at((x, y), color)

  return 0

def transform_data_extent(data, extent) -> List[tuple]:
  
  # sdl2.ext.fill(surface, BLACK)

  # renderer.clear(BLACK)

  # pixelview = sdl2.ext.PixelView(surface)

  transform = []

  line = 0
  count = 0
  for val in data:
    # print(dir(surface))
    # quit()
    # pixelview[line][count] = sdl2.ext.Color(val, val, val)
    # color = sdl2.ext.Color(val, val, val)
    # renderer.draw_point([count, line], color)

    brightness = val

    if count >= extent:
      line += 1
      count = 0
    else:
      count += 1

    transform.append((count, line, brightness))

  return transform

def transform_data_square(data, slide, gap) -> List[tuple]:

  # zip data to create (x, y) coords
  # coords = list(itertools.chain(*zip(data, data[1:])))
  coords = zip(data[::slide], data[1::slide*gap])

  transform: list = []

  random.seed(0)

  for coord in coords:
    x, y = coord
    # total = x + y
    # total = total if total <= 255 else 255
    # color = sdl2.ext.Color(total, total, total)

    # brightness = random.randint(0, 255)
    if 0 <= x <= 127 and 0 <= y <= 127:
      color = (255, 165, 0)
    else:
      color = (255, 255, 255)

    transform.append((x, y, color))

  # renderer.draw_point(coords, WHITE)

  return transform

def transform_data_circular(data, slide, gap) -> List[tuple]:

  coords = zip(data[::slide], data[1::slide*gap])

  transform: list = []

  random.seed(0)

  # convert these polar coords to cartesian
  for coord in coords:
    r, theta = coord

    # brightness = random.randint(0, 255)
    if 0 <= r <= 127 and 0 <= theta <= 127:
      color = (255, 165, 0)
    else:
      color = (255, 255, 255)

    theta = radians(theta)
    x, y = r * cos(theta), r * sin(theta)
    x, y, = x, y

    transform.append((x, y, color))

  return transform

def transform_data_cubic(data, slide, gap, rotation: List[float], distance) -> List[tuple]:

  coords = zip(data, data[1:], data[2:])

  transform: list = []

  for coord in coords:
    x, y, z = coord

    if 0 <= x <= 127 and 0 <= y <= 127 and 0 <= z <= 127:
      color = (255, 165, 0)
    else:
      color = (255, 255, 255)


    point = apply_rotation(coord, rotation)

    x1, y1 = project(point, distance)

    new_coord: tuple = (x1, y1, color)

    # print(coord, '->', new_coord)

    transform.append(new_coord)

  return transform

def transform_data_spherical(data, slide, gap, rotation: List[float], distance) -> List[tuple]:

  coords = zip(data, data[1:], data[2:])

  transform: list = []

  # random.seed(0)

  # convert these polar coords to cartesian
  for coord in coords:
    r, theta, phi = coord

    # brightness = random.randint(0, 255)
    if 0 <= r <= 127 and 0 <= theta <= 127:
      color = (255, 165, 0)
    else:
      color = (255, 255, 255)

    theta = radians(theta)
    phi = radians(phi)
    x, y, z = r * cos(theta) * sin(phi), r * sin(theta) * sin(phi), r * cos(theta)

    coord = x, y, z

    point = apply_rotation(coord, rotation)

    x1, y1 = project(point, distance)

    new_coord: tuple = (x1, y1, color)

    transform.append(new_coord)

  return transform

def transform_data_cylindrical(data, slide, gap, rotation: List[float], distance) -> List[tuple]:

  coords = zip(data, data[1:], data[2:])

  transform: list = []

  # random.seed(0)

  # convert these polar coords to cartesian
  for coord in coords:
    r, theta, z = coord

    # brightness = random.randint(0, 255)
    if 0 <= r <= 127 and 0 <= theta <= 127:
      color = (255, 165, 0)
    else:
      color = (255, 255, 255)

    theta = radians(theta)
    x, y, z = r * cos(theta), r * sin(theta), z

    coord = x, y, z

    point = apply_rotation(coord, rotation)

    x1, y1 = project(point, distance)

    new_coord: tuple = (x1, y1, color)

    transform.append(new_coord)

  return transform

def draw_guide_cube(surface, xshift, yshift, rotation, zoom) -> int:

  vertices = [
    (0, 0, 0),
    (255, 0, 0),
    (255, 255, 0),
    (0, 255, 0),
    (0, 0, 255),
    (255, 0, 255),
    (255, 255, 255),
    (0, 255, 255),
  ]

  vertices = map(lambda v: apply_rotation(v, rotation), vertices)
  vertices = list(map(lambda v: shift(v, (xshift, yshift)), vertices))

  vertices = list(map(lambda v: project(v, zoom), vertices))

  combs = [
    (vertices[0], vertices[1]),
    (vertices[1], vertices[2]),
    (vertices[2], vertices[3]),
    (vertices[3], vertices[0]),
    (vertices[4], vertices[5]),
    (vertices[5], vertices[6]),
    (vertices[6], vertices[7]),
    (vertices[7], vertices[4]),
    (vertices[0], vertices[4]),
    (vertices[1], vertices[5]),
    (vertices[2], vertices[6]),
    (vertices[3], vertices[7])
  ]

  print(rotation)
  print(vertices)

  list(map(lambda v: pygame.draw.line(surface, (255, 255, 255), v[0], v[1], 1), combs))

  return 0

def apply_rotation(point, rotation) -> Tuple[float, float, float]:
  x, y, z = point

  a, b, c = rotation

  """
  # apply rotation around X
  x = x
  y = y * cos(a) - z * sin(a)
  z = y * sin(a) + z * sin(a)

  # apply rotation around Y
  x = z * sin(b) + x * cos(b)
  y = y
  z = z * cos(b) - x * sin(b)
  """

  # apply rotation about x and y
  # [ cos(b)       0       -sin(b)      ]
  # [ sin(a)sin(b) cos(a)  sin(a)cos(b) ]
  # [ cos(a)sin(b) -sin(a) cos(a)cos(b) ]

  """
  x = x * cos(b) - z * sin(b)
  y = x * sin(a) * sin(b) + y * cos(a) + z * sin(a) * cos(b)
  z = x * cos(a) * sin(b) - y * sin(a) + z * cos(a) * cos(b)
  """

  x = x * cos(b)                        + z * sin(b)
  y = x * sin(a) * sin(b)  + y * cos(a) - z * sin(a) * cos(b)
  z = x * -cos(a) * sin(b) + y * sin(a) + z * cos(a) * cos(b)


  return  x, y, z

def project(point, distance) -> Tuple[float, float]:

  distance = 1000

  x, y, z = point

  x1 = x * (distance / (distance + z))
  y1 = y * (distance / (distance + z))

  return x, y

def shift(point, shift) -> Tuple[float, float]:

  x, y, z = point
  xshift, yshift = shift

  return x + xshift, y + yshift, z

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
  # opts, args = getopt.getopt(args[1:], 'f:e:ls', ['square', 'linear', 'file=', 'extent='])

  # print(opts)
  # print(args)

  filename = parsedargs.file
  extent = parsedargs.extent
  shape = parsedargs.shape
  filetype = parsedargs.filetype
  slide = parsedargs.slide
  gap = parsedargs.gap


  # TODO: read in as strings
  if filetype == 'binary': 
    with open(filename, 'rb') as bf:

      bdata: bytes = bf.read()

      darray = array.array('B')
      # data.fromfile(f, 10)
      darray.frombytes(bdata)
      data = darray.tolist()
  elif filetype == 'string':

    with open(filename, 'r') as f:

      fdata: str = f.read()

      data = list(map(int, fdata.split()))

  # print(data)

  # RESOURCES = sdl2.ext.Resources(__file__, 'resources')

  WINDOW_WIDTH: int = 800
  WINDOW_HEIGHT: int = 600

  pygame.init()
  # window = sdl2.ext.Window(f'View {filename}', size=(WINDOW_WIDTH, WINDOW_HEIGHT))
  pygame.display.set_caption(f"View {filename}")
  pygame.display.set_mode((WINDOW_WIDTH, WINDOW_HEIGHT))
  # window.show()

  # surface = window.get_surface()
  surface = pygame.display.get_surface()

  # renderer = sdl2.ext.Renderer(surface)
  # renderer = sdl2.ext.Renderer(window)

  # TODO: make the drawing methods into transforms
  # which returns the transformed data as 2D coordinates (without altering)
  # and the draw method accepts this transformed data and draws
  # the data
  # transforms:
  #   zooms
  #   shifts

  zoom: float = 1
  # shift = 0
  xshift = WINDOW_WIDTH / 2
  yshift = WINDOW_HEIGHT / 2

  extent_s = ''

  changed = True

  rotation: List[float] = [0, 0, 0]

  running = True
  while running:
    # events = sdl2.ext.get_events()

    for event in pygame.event.get():
      changed = True
      if event.type == pygame.QUIT:
        running = False
        break
      elif event.type == pygame.KEYDOWN:

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

  # sdl2.ext.quit()
  # pygame.quit()

  return 0

if __name__ == '__main__':
  main(sys.argv)
