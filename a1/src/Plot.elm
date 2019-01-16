module Plot exposing (makeBarPlot)

import Html
import Svg exposing (..)
import Svg.Attributes exposing (..)

import Models exposing (Model)

-- Bar Plot

-- make bars

makeBarPlot : Model -> Svg msg
makeBarPlot {data} =
  let
    svgheight = "500"
    svgwidth = "500"
  in
  svg
    [ width svgwidth, height svgheight, viewBox "0 0 100 100", transform "translate(0,500)", transform "scale(1, -1)"]
    [
      rect [ fill "green", x "0", y "0", width "10", height "90", rx "1", ry "1" ] [],
      rect [ fill "red", x "10", y "0", width "10", height "100", rx "1", ry "1" ] []
    ]