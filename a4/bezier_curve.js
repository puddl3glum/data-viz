
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


const generate_bezier_curve = function() {

  /*
  Use D3 to Query coordinates of the control points 
  Using a for loop populate the coordinates of the 20 points on the bezier curve, 
  and use D3 to select the given 20 points and set their attributes (cx, cy) based on the previous calculation
  */
  
  const point_count = 20;

  // get the coordinates of the control points
  const control_points = d3.selectAll("circle[id^='circle']")
    .nodes()
    .map(
      (x) => [x.getAttribute("cx"), x.getAttribute("cy")]
    );

  const points = d3.selectAll("circle[id^='bezier_point_p']");

  // - 1 so the first and last points go to p0 and p3
  const precision = 1 / (point_count - 1);

  for (let i = 1; i <= point_count; i++) {
  
    // again - 1 so there are points at the ends of the curve
    const point = get_bezier_point_position(precision * (i - 1), ...control_points);

    const circle = d3.select(`circle[id='bezier_point_p${i}']`)
    circle.attr("cx", point[0]);
    circle.attr("cy", point[1]);

  }

}
