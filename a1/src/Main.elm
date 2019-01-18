module Main exposing (main)

import Browser
import Html exposing (Html, button, div)
import Html.Events exposing (onClick)
import Svg exposing (..)
import Svg.Attributes exposing (..)


type alias Datapair =
    { x : Float, y : Float }


type alias Dataset =
    { data : List Datapair }


main =
    let
        dataset0 =
            Dataset
                [ Datapair 100 74.6
                , Datapair 80 67.7
                , Datapair 130 124.4
                , Datapair 90 71.1
                , Datapair 110 78.1
                , Datapair 140 88.4
                , Datapair 60 69.8
                , Datapair 40 53.9
                , Datapair 120 81.5
                , Datapair 70 64.2
                , Datapair 50 57.3
                ]

        dataset1 =
            Dataset
                [ Datapair 80 65.8
                , Datapair 80 57.6
                , Datapair 80 77.1
                , Datapair 80 88.4
                , Datapair 80 84.7
                , Datapair 80 70.4
                , Datapair 80 52.5
                , Datapair 190 125
                , Datapair 80 55.6
                , Datapair 80 79.1
                , Datapair 80 68.9
                ]

        -- { data = [ ( 100, 74.6 ), ( 80, 67.7 ) ] }
    in
    -- Browser.sandbox { init = init, update = update, view = view }
    div []
        [ -- , button [ onClick Increment ] [ text "+" ]
          getPlot (List.map getBar) dataset0
        , getPlot (List.map getBar) dataset1
        , getPlot (List.map getCircle) dataset0
        , getPlot (List.map getCircle) dataset1
        , getPlot getLine dataset0
        , getPlot getLine dataset1
        ]


scalePairs : Float -> Float -> List Datapair -> List Datapair
scalePairs xscale yscale pairs =
    let
        xmax =
            List.maximum (List.map .x pairs) |> Maybe.withDefault 1

        ymax =
            List.maximum (List.map .y pairs) |> Maybe.withDefault 1

        scalePair pair =
            Datapair (pair.x * xscale / xmax) (pair.y * yscale / ymax)
    in
    List.map scalePair pairs


getBar : Datapair -> Svg msg
getBar pair =
    let
        xs =
            String.fromFloat pair.x

        ys =
            String.fromFloat pair.y
    in
    rect [ fill "green", x xs, y "0", width "10", height ys, stroke "forestgreen" ] []


getCircle : Datapair -> Svg msg
getCircle pair =
    let
        xs =
            String.fromFloat pair.x

        ys =
            String.fromFloat pair.y
    in
    circle [ fill "lightblue", cx xs, cy ys, r "5", stroke "forestgreen" ] []


getLine : List Datapair -> List (Svg msg)
getLine datapairs =
    let
        pointstr pair =
            String.fromFloat pair.x ++ "," ++ String.fromFloat pair.y

        pairstrings =
            List.map pointstr (List.sortBy .x datapairs)

        datapoints =
            String.join " " pairstrings
    in
    [ polyline [ stroke "black", fill "None", points datapoints ] [] ]


getPlot : (List Datapair -> List (Svg msg)) -> Dataset -> Svg msg
getPlot getShape dataset =
    let
        svgheight =
            150

        svgwidth =
            150

        elemwidth =
            10

        svgwidths =
            String.fromFloat (svgwidth + elemwidth)

        svgheights =
            String.fromFloat (svgheight + 10)

        elemwidths =
            String.fromFloat elemwidth

        pairs =
            scalePairs svgwidth svgheight dataset.data

        shapes =
            getShape pairs
    in
    svg
        [ width svgwidths, height svgheights, transform "translate(0,500)", transform "scale(1, -1)" ]
        ([ rect [ fill "azure", stroke "black", x "0", y "0", width svgwidths, height svgheights ] [] ]
            ++ shapes
        )


makeBarPlot : Dataset -> Svg msg
makeBarPlot dataset =
    let
        svgheight =
            150

        svgwidth =
            150

        svgwidths =
            String.fromFloat svgwidth

        svgheights =
            String.fromFloat svgheight

        bars =
            List.map getBar dataset.data
    in
    svg
        [ width svgwidths, height svgheights, transform "translate(0,500)", transform "scale(1, -1)" ]
        bars
