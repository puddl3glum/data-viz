
let get_bezier_point_position = function(pos, p0, p1, p2, p3) {
  /*
   * Determines the coordinates of a point on the bezier curve based on
   * the pos and the control points
   * Args:
   * pos: float from [0-1] - how far along the bezier curve the point is
   * p0-p3 : [x, y]
   * */

  let coords = [p0, p1, p2, p3];

  let get_coord = (c0, c1, c2, c3) => (Math.pow(1 - pos, 3) * c0)
    + (3 * Math.pow(1 - pos, 2) * pos * c1)
    + (3 * (1 - pos) * Math.pow(pos, 2) * c2)
    + (Math.pow(pos, 3) * c3);

  let x = get_coord(...coords.map((c) => c[0]));
  let y = get_coord(...coords.map((c) => c[1]));

  return [x, y];
}


let generate_bezier_curve = function() {

  /*
  Use D3 to Query coordinates of the control points 
  Using a for loop populate the coordinates of the 20 points on the bezier curve, 
  and use D3 to select the given 20 points and set their attributes (cx, cy) based on the previous calculation
  */
  
  let point_count = 20;

  // get the coordinates of the control points
  let control_points = d3.selectAll("circle[id^='circle']")
    .nodes()
    .map(
      (x) => [x.getAttribute("cx"), x.getAttribute("cy")]
    );

  let points = d3.selectAll("circle[id^='bezier_point_p']");

  // - 1 so the first and last points go to p0 and p3
  let precision = 1 / (point_count - 1);

  for (var i = 1; i <= point_count; i++) {
  
    // again - 1 so there are points at the ends of the curve
    let point = get_bezier_point_position(precision * (i - 1), ...control_points);

    let circle = d3.select(`circle[id='bezier_point_p${i}']`)
    circle.attr("cx", point[0]);
    circle.attr("cy", point[1]);

  }

  
  // get lines
  
  // step up lines
  
  // 

}
