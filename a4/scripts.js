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
  scatter_plot(create_plot(), circle_data);

  bar_plot(create_plot(), rect_data);

  random_rects(create_plot());

  old_scatter_plot(create_plot());

  old_bar_plot(create_plot());

  staircase_plot(create_plot(), rect_data);

  bezier_curve(create_plot());

}

const create_plot = () => {
  const svg = document.createElement("svg");
  svg.classList.add("plot")
  return svg;
}

const create_svg = (parentselector, newid, newclass, width, height, pad) => {

  const d3svg = d3.select(parentselector)
    .append("svg")
    .attr("id", newid)
    .attr("class", newclass)
    .attr("width", width)
    .attr("height", height)
    .attr("viewBox", `${-pad} ${-pad} ${220+pad*2} ${220+pad*2}`)
    .attr("transform", "scale(1 -1)");

  return d3svg
}

/*
 * QUESTION 1
 * */

const old_scatter_plot = (svg) => {

  const width = 220;
  const height = 220;

  const pad = 10;

  const d3svg = create_svg("div#old-scatter.plot-container", "old-scatter", "plot", width, height, pad);


  for(var i = 0; i < circle_data.length; ++i)
    d3svg.append("circle")
      .attr("r", "0.1cm")
      .attr("cx", Math.random() * width)
      .attr("cy", Math.random()* height);
}

const plot_scatter_points = (e) => {
  e.target.remove();

  const xmax = Math.max(...circle_data.map((d) => d.x));
  const ymax = Math.max(...circle_data.map((d) => d.y));

  const d3svg = d3.select("svg#old-scatter.plot");

  const points = d3svg.selectAll("circle")
    .data(circle_data)
    .attr("cx", (d) => d.x * d3svg.attr("width") / xmax)
    .attr("cy", (d) => d.y * d3svg.attr("height") / ymax);
}

/*
 * QUESTION 2
 * */

const old_bar_plot = (svg) => {

  const width = 220;
  const height = 220;
  const pad = 10;

  const ymax = Math.max(...rect_data);
  const xmax = rect_data.length;

  const bar_width = 10;

  const d3svg = create_svg("div#old-bar.plot-container", "old-bar", "plot", width, height, pad);

  for (let i = 0; i < rect_data.length; ++i)
    d3svg.append("rect")
      .attr("width", bar_width)
      .attr("height", Math.random() * ymax)
      .attr("x", (i * width / xmax) + bar_width / 2)
      .attr("y", 0);
}

const plot_old_bars = (e) => {
  e.target.remove();

  const ymax = Math.max(...rect_data);

  const d3svg = d3.select("svg#old-bar.plot");
  
  d3svg.selectAll("rect")
    .data(rect_data)
    .attr("height", (d) => d * d3svg.attr("height") / ymax);
}

/*
 * QUESTION 3
 * */

const random_rects = (svg) => {

  const width = 220;
  const height = 220;

  const pad = 10;

  const d3svg = create_svg("div#random.plot-container", "random", "plot", width, height, pad);
}

const add_rect = () => {
  d3.select("svg#random.plot")
    .append("rect")
    .attr("width", 25)
    .attr("height", 25)
    .attr("x", Math.random() * 220)
    .attr("y", Math.random() * 220)
}

const delete_rects = () => {
  d3.selectAll("svg#random.plot>rect")
    .remove();
}

/*
 * QUESTION 4
 * */

const scatter_plot = (svg, data) => {

  // const scatter_plot_div = document.querySelector('#scatter.plot-container');
  // put scatter plot into div
  // scatter_plot_div.appendChild(svg);
  // svg.id = "scatter";
  // svg.setAttribute("xmlns", "http://www.w3.org/2000/svg");
  // get the scatter plot elements
  
  const height = 220;
  const width = 220;

  const pad = 10;

  const d3svg = create_svg("div#scatter.plot-container", "scatter", "plot", width, height, pad);

  const xmax = Math.max(...circle_data.map((d) => d.x));
  const ymax = Math.max(...circle_data.map((d) => d.y));

  d3svg.selectAll("circle")
    .data(circle_data)
    .enter()
    .append("circle")
    .attr("r", "0.1cm")
    .attr("cx", (d) => d.x * width / xmax)
    .attr("cy", (d) => d.y * height / ymax);
  
}

/*
 * QUESTION 5
 * */

const bar_plot = (svg, data) => {

  const height = 220;
  const width = 220;

  const pad = 10;

  const ymax = Math.max(...data);
  const xmax = rect_data.length;

  const bar_width = 10;

  const d3svg = create_svg("div#bar.plot-container", "bar", "plot", width, height, pad);

  d3svg.selectAll("rect")
    .data(data)
    .enter()
    .append("rect")
    .attr("width", bar_width)
    .attr("height", (d) => d * height / ymax)
    // center the bars with + pad / 2
    .attr("x", (d, i) => (i * width / xmax) + bar_width / 2);
}

/*
 * QUESTION 6
 * */

const staircase_plot = (svg, data) => {

  const width = 220;
  const height = 220;
  const pad = 10;

  const ymax = Math.max(...data);
  const xmax = rect_data.length;
  const bar_width = 10;

  const d3svg = create_svg("div#staircase.plot-container", "staircase", "plot", width, height, pad);

  d3svg.selectAll("rect")
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


  const d3svg = d3.select("svg#staircase.plot");

  const height = d3svg.attr("height");
  const ymax = Math.max(...rect_data);

  const button = e.target;
  const text = button.innerText;

  // toggle the button
  button.innerText = states[ (states.indexOf(text) + 1) % states.length ];

  if (text === "Staircase") {
    // toggle to the staircase
    const rects = d3svg.selectAll("rect")
    rects
      .attr("height", (d, i) => ++i / rects.size() * height);
  }
  else {
    // toggle to the """data"""
    d3svg.selectAll("rect")
      .attr("height", (d) => d * height / ymax)
  }
}

/*
 * QUESTION 7
 * */

const bezier_curve = (svg) => {
  
  const width = 220;
  const height = 220;
  const pad = 10;

  const d3svg = create_svg("div#bezier.plot-container", "bezier", "plot", width, height, pad);

  // Initialize random control points
  
  // Generate initial bezier curve

}
