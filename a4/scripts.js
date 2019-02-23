const circle_data = [
  { x: 100, y: 74.6 }
  , { x: 80, y: 67.7 }
  , { x: 130, y: 124.4 }
  , { x: 90, y: 71.1 }
  , { x: 110, y: 78.1 }
  , { x: 140, y: 88.4}
  , { x: 60, y: 69.8}
  , { x: 40, y: 53.9 }
  , { x: 120, y: 81.5 }
  , { x: 70, y: 64.2 }
  , { x: 50, y: 57.3 }
];

const rect_data = [
  100
  , 80
  , 130
  , 90
  , 110
  , 140
  , 60
  , 40
  , 120
  , 70
  , 50
];


const load_content = () => {
  // create a plot
  old_scatter_plot();
  old_bar_plot();
  random_rects();
  scatter_plot(circle_data);

  bar_plot(rect_data);

  staircase_plot(rect_data);

  bezier_curve();

}

const create_svg = (parentselector, newid, newclass, width, height, pad) => {

  const svg = d3.select(parentselector)
    .append("svg")
    .attr("id", newid)
    .attr("class", newclass)
    .attr("width", width)
    .attr("height", height)
    .attr("viewBox", `${-pad} ${-pad} ${220 + pad * 2} ${220 + pad * 2}`)
    .attr("transform", "scale(1 -1)");

  return svg
}

/*
 * QUESTION 1
 * */

const old_scatter_plot = () => {

  const width = 220;
  const height = 220;

  const pad = 10;

  const svg = create_svg("div#old-scatter.plot-container", "old-scatter", "plot", width, height, pad);


  for(var i = 0; i < circle_data.length; ++i)
    svg.append("circle")
      .attr("r", 0)
      .attr("cx", width / 2)
      .attr("cy", height / 2)
      .transition()
      .attr("r", "0.1cm")
      .attr("cx", Math.random() * width)
      .attr("cy", Math.random()* height);
}

const plot_scatter_points = (e) => {
  e.target.remove();

  const xmax = Math.max(...circle_data.map((d) => d.x));
  const ymax = Math.max(...circle_data.map((d) => d.y));

  const svg = d3.select("svg#old-scatter.plot");

  svg.selectAll("circle")
    .data(circle_data)
    .transition()
    .attr("cx", (d) => d.x * svg.attr("width") / xmax)
    .attr("cy", (d) => d.y * svg.attr("height") / ymax);
}

/*
 * QUESTION 2
 * */

const old_bar_plot = () => {

  const width = 220;
  const height = 220;
  const pad = 10;

  const ymax = Math.max(...rect_data);
  const xmax = rect_data.length;

  const bar_width = 10;

  const svg = create_svg("div#old-bar.plot-container", "old-bar", "plot", width, height, pad);

  for (let i = 0; i < rect_data.length; ++i)
    svg.append("rect")
      .attr("width", bar_width)
      .attr("x", (i * width / xmax) + bar_width / 2)
      .attr("y", 0)
      .transition()
      .attr("height", Math.random() * ymax);
}

const plot_old_bars = (e) => {
  e.target.remove();

  const ymax = Math.max(...rect_data);

  const svg = d3.select("svg#old-bar.plot");
  
  svg.selectAll("rect")
    .data(rect_data)
    .transition()
    .attr("height", (d) => d * svg.attr("height") / ymax);
}

/*
 * QUESTION 3
 * */

const random_rects = () => {

  const width = 220;
  const height = 220;

  const pad = 10;

  const svg = create_svg("div#random.plot-container", "random", "plot", width, height, pad);
}

const add_rect = () => {
  d3.select("svg#random.plot")
    .append("rect")
    .attr("x", 110)
    .attr("y", 110)
    .transition()
    .attr("width", 25)
    .attr("height", 25)
    .attr("x", Math.random() * 220)
    .attr("y", Math.random() * 220)
}

const delete_rects = () => {
  d3.selectAll("svg#random.plot>rect")
    .transition()
    .attr("x", 110)
    .attr("y", 110)
    .attr("width", 0)
    .attr("height", 0)
    .remove();
}

/*
 * QUESTION 4
 * */

const scatter_plot = (data) => {

  // const scatter_plot_div = document.querySelector('#scatter.plot-container');
  // put scatter plot into div
  // scatter_plot_div.appendChild(svg);
  // svg.id = "scatter";
  // svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");
  // get the scatter plot elements
  
  const height = 220;
  const width = 220;

  const pad = 10;

  const svg = create_svg("div#scatter.plot-container", "scatter", "plot", width, height, pad);

  const xmax = Math.max(...circle_data.map((d) => d.x));
  const ymax = Math.max(...circle_data.map((d) => d.y));

  svg.selectAll("circle")
    .data(circle_data)
    .enter()
    .append("circle")
    .attr("r", 0)
    .attr("cx", width / 2)
    .attr("cy", height / 2)
    .transition()
    .attr("r", "0.1cm")
    .attr("cx", (d) => d.x * width / xmax)
    .attr("cy", (d) => d.y * height / ymax);
  
}

/*
 * QUESTION 5
 * */

const bar_plot = (data) => {

  const height = 220;
  const width = 220;

  const pad = 10;

  const ymax = Math.max(...data);
  const xmax = rect_data.length;

  const bar_width = 10;

  const svg = create_svg("div#bar.plot-container", "bar", "plot", width, height, pad);

  svg.selectAll("rect")
    .data(data)
    .enter()
    .append("rect")
    .attr("x", (d, i) => (i * width / xmax) + bar_width / 2)
    .attr("width", bar_width)
    .transition()
    .attr("height", (d) => d * height / ymax);
    // center the bars with + pad / 2
}

/*
 * QUESTION 6
 * */

const staircase_plot = (data) => {

  const width = 220;
  const height = 220;
  const pad = 10;

  const ymax = Math.max(...data);
  const xmax = rect_data.length;
  const bar_width = 10;

  const svg = create_svg("div#staircase.plot-container", "staircase", "plot", width, height, pad);

  svg.selectAll("rect")
    .data(data)
    .enter()
    .append("rect")
    .attr("width", bar_width)
    .attr("height", (d) => d * height / ymax)
    // center the bars with + pad / 2
    .attr("x", (d, i) => (i * width / xmax) + bar_width / 2);

}

const staircase_toggle = (e) => {

  const states = ["Staircase", "Reset"];

  const svg = d3.select("svg#staircase.plot");

  const height = svg.attr("height");
  const ymax = Math.max(...rect_data);

  const button = e.target;
  const text = button.innerText;

  // toggle the button
  button.innerText = states[ (states.indexOf(text) + 1) % states.length ];

  // transition rects
  const rects = svg.selectAll("rect");
  rects.transition()
    .attr("height", function(d, i) {
      const rect = d3.select(this);
      const rheight = rect.attr("height");
      // can't base it on data easily bc of rounding errors
      return (text == "Staircase") ? ++i / rects.size() * height : d * height / ymax;
    });

  /*
  if (text === "Staircase") {
    // toggle to the staircase
    const rects = svg.selectAll("rect")
    rects
      .attr("height", (d, i) => ++i / rects.size() * height);
  }
  else {
    // toggle to the """data"""
    svg.selectAll("rect")
      .attr("height", (d) => d * height / ymax)
  }
  */
}

/*
 * QUESTION 7
 * */

const bezier_curve = () => {

  
  const width = 220;
  const height = 220;
  const pad = 10;

  const svg = create_svg("div#bezier.plot-container", "bezier", "plot", width, height, pad);


  const controls = Array.from( { length:4 }, () =>
    (
      { x : Math.random() * width
      , y : Math.random() * height}
    ));

  // Initialize random control points
  const circles = svg.selectAll("circle")

  circles.data(controls)
    .enter()
    .append("circle")
    .attr("class", "control-point")
    .attr("r", "0.2cm")
    .attr("cx", d => d.x)
    .attr("cy", d => d.y)
    .call(d3.drag()
      .on("drag", function(d) {

        d3.select(this)
          .attr("cx", d.x = d3.event.x)
          .attr("cy", d.y = d3.event.y);
        update_curve()
      })
    );
  
  update_curve();
  
  // Generate initial bezier curve

}

const head = function(l) {
  // gets first element of list.
  return l[0];
}

const tail = function(l) {
  return l.slice(1);
}

const zip = function(a, b) {
  if (a.length === 0 || b.length === 0) {
    return [];
  }
  else {
    return [[head(a), head(b)]].concat(zip(tail(a), tail(b)));
  }
}

const update_curve = function() {

  const slider = d3.select("input#bezier.slider[type='range']").node();
  const pointcount = parseInt(slider.value) + 1;

  const svg = d3.select("svg#bezier.plot");

  // generate the curve
  const controls = svg.selectAll("circle.control-point");

  const bezierdata = d3.range(0, pointcount + 1).map(function(d) {
    // generate points 
    const position = d / pointcount;
    // console.log(position);
    const cdata = svg.selectAll("circle.control-point").data();

    // console.log("Xes");
    // console.log(x);
    
    return get_bezier_point_position(position, cdata);

  });


  const points = svg.selectAll("circle.curve-point")
    .data(bezierdata);

  points
    .attr("cx", d => d.x)
    .attr("cy", d => d.y);

  points
    .enter()
    .insert("circle", ".control-point")
    .attr("class", "curve-point")
    .attr("cx", d => d.x)
    .attr("cy", d => d.y)
    .attr("r", "0.05cm");

  points
    .exit()
    .remove();

  // points
    // .attr("cx", d => d.x)
    // .attr("cy", d => d.y)

  // points.exit()
    // .remove();
  
  const linedata = zip(bezierdata, tail(bezierdata));

  // draw lines from control points, too
  const lines = svg.selectAll("line.curve-line")
    .data(linedata)

  lines
    .attr("x1", d => d[0].x) 
    .attr("y1", d => d[0].y) 
    .attr("x2", d => d[1].x) 
    .attr("y2", d => d[1].y)

  lines
    .enter()
    .insert("line", ".curve-point")
    .attr("class", "curve-line")
    .attr("x1", d => d[0].x) 
    .attr("y1", d => d[0].y) 
    .attr("x2", d => d[1].x) 
    .attr("y2", d => d[1].y)

  lines
    .exit()
    .remove();

}

const get_bezier_point_position = function(pos, controls) {
  /*
   * Determines the coordinates of a point on the bezier curve based on
   * the pos and the control points
   * Args:
   * pos: float from [0-1] - how far along the bezier curve the point is
   * p0-p3 : [x, y]
   * */

  const get_coord = (c) => (Math.pow(1 - pos, 3) * c[0])
    + (3 * Math.pow(1 - pos, 2) * pos * c[1])
    + (3 * (1 - pos) * Math.pow(pos, 2) * c[2])
    + (Math.pow(pos, 3) * c[3]);

  const x = get_coord(controls.map((d) => d.x));
  const y = get_coord(controls.map((d) => d.y));

  return {x : x, y : y};
}

