
const destroy_button = function() {
  /*
   * @SideEffect
   * Hides button.create_curve
   * */

  const button = document.querySelector("button.create-curve");
  button.remove();
}

const get_bezier_point_position = function(pos, p0, p1, p2, p3) {
  /*
   * Determines the coordinates of a point on the bezier curve based on
   * the pos and the control points
   * Args:
   * pos: float from [0-1] - how far along the bezier curve the point is
   * p0-p3 : [x, y]
   * */

  const coords = [p0, p1, p2, p3];

  const get_coord = (c0, c1, c2, c3) => (Math.pow(1 - pos, 3) * c0)
    + (3 * Math.pow(1 - pos, 2) * pos * c1)
    + (3 * (1 - pos) * Math.pow(pos, 2) * c2)
    + (Math.pow(pos, 3) * c3);

  const x = get_coord(...coords.map((c) => c[0]));
  const y = get_coord(...coords.map((c) => c[1]));

  return [x, y];
}

