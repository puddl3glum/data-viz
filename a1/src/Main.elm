module Main exposing (main)

import Browser
import Html exposing (Html, br, button, div, h4, i)
import Html.Events exposing (onClick)
import Svg exposing (..)
import Svg.Attributes exposing (..)


type alias Datapair =
    { x : Float, y : Float }


main =
    let
        width =
            200

        height =
            200

        padding =
            20

        -- returns {x = idx, y = x or y}
        getPart : (Datapair -> Float) -> List Datapair -> List Datapair
        getPart f pairs =
            let
                indices =
                    List.map Basics.toFloat (List.range 0 (List.length pairs))

                -- creates list where y is the component being isolated
                newpairs =
                    List.map (\r -> { r | y = f r }) pairs
            in
            List.map2 (\r x0 -> { r | x = x0 }) newpairs indices

        dset0orig =
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

        dset1orig =
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

        dset0 =
            offsetPairs 5 5 (scalePairs height width dset0orig)

        dset1 =
            offsetPairs 5 5 (scalePairs height width dset1orig)

        dset0x =
            offsetPairs 5 5 (scalePairs height width (getPart .x dset0))

        dset0y =
            offsetPairs 5 5 (scalePairs height width (getPart .y dset0))

        dset1x =
            offsetPairs 5 5 (scalePairs height width (getPart .x dset1))

        dset1y =
            offsetPairs 5 5 (scalePairs height width (getPart .y dset1))

        -- { data = [ ( 100, 74.6 ), ( 80, 67.7 ) ] }
    in
    -- Browser.sandbox { init = init, update = update, view = view }
    div []
        [ -- , button [ onClick Increment ] [ text "+" ]
          div [ class "dataset" ]
            [ div
                [ class "plot" ]
                [ h4 []
                    [ text "Dataset 1 "
                    , i [] [ text "x" ]
                    , text "-coordinates"
                    ]
                , getPlot width height padding (getBars dset0x)
                ]
            , div [ class "plot" ]
                [ h4 []
                    [ text "Dataset 1 "
                    , i [] [ text "y" ]
                    , text "-coordinates"
                    ]
                , getPlot width height padding (getBars dset0y)
                ]
            , div [ class "plot" ]
                [ h4 []
                    [ text "Dataset 1 scatter plot"
                    ]
                , getPlot width height padding (List.map getCircle dset0)
                ]

            --, div [ class "plot" ] [ getPlot getLine dataset0 ]
            -- path for x coord (which is retarded and meaningless)
            , div [ class "plot" ]
                [ h4 []
                    [ text "Dataset 1 "
                    , i [] [ text "x" ]
                    , text "-coordinates"
                    ]
                , getPlot width height padding (getPathLine dset0x)
                ]
            , div [ class "plot" ]
                [ h4 []
                    [ text "Dataset 1 "
                    , i [] [ text "y" ]
                    , text "-coordinates"
                    ]
                , getPlot width height padding (getLineSegments dset0y)
                ]
            ]
        , br [] []
        , div
            [ class "dataset" ]
            [ -- div [ class "plot" ] [ getPlot width height padding dataset1 (getBars .x scaled1) ]
              div [ class "plot" ]
                [ h4 []
                    [ text "Dataset 2 "
                    , i [] [ text "x" ]
                    , text "-coordinates"
                    ]
                , getPlot width height padding (getBars dset1x)
                ]
            , div [ class "plot" ]
                [ h4 []
                    [ text "Dataset 2 "
                    , i [] [ text "y" ]
                    , text "-coordinates"
                    ]
                , getPlot width height padding (getBars dset1y)
                ]
            , div [ class "plot" ]
                [ h4 []
                    [ text "Dataset 2 scatter plot" ]
                , getPlot width height padding (List.map getCircle dset1)
                ]
            , div [ class "plot" ]
                [ h4 []
                    [ text "Dataset 2 "
                    , i [] [ text "x" ]
                    , text "-coordinates"
                    ]
                , getPlot width height padding (getPathLine dset1x)
                ]
            , div [ class "plot" ]
                [ h4 []
                    [ text "Dataset 2 "
                    , i [] [ text "y" ]
                    , text "-coordinates"
                    ]
                , getPlot width height padding (getLineSegments dset1y)
                ]

            --, div [ class "plot" ] [ getPlot getLine dataset1 ]
            ]
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


offsetPairs : Float -> Float -> List Datapair -> List Datapair
offsetPairs xoffset yoffset pairs =
    List.map (\r -> Datapair (r.x + xoffset) (r.y + yoffset)) pairs


getPlot : Float -> Float -> Float -> List (Svg msg) -> Svg msg
getPlot pwidth pheight padding shapes =
    let
        svgwidths =
            String.fromFloat (pwidth + padding)

        svgheights =
            String.fromFloat (pheight + padding)
    in
    svg
        [ width svgwidths, height svgheights, transform "scale(1, -1)" ]
        ([ rect [ class "background", x "0", y "0", width svgwidths, height svgheights ] [] ]
            ++ shapes
        )


getBars : List Datapair -> List (Svg msg)
getBars pairs =
    let
        getRect : Datapair -> Svg msg
        getRect pair =
            let
                xs =
                    String.fromFloat pair.x

                ys =
                    String.fromFloat pair.y
            in
            rect [ class "bar", x xs, y "5", width "10", height ys ] []
    in
    List.map getRect pairs


getCircle : Datapair -> Svg msg
getCircle pair =
    let
        xs =
            String.fromFloat pair.x

        ys =
            String.fromFloat pair.y
    in
    circle [ r "3", cx xs, cy ys, class "scatter" ] []


getPathLine : List Datapair -> List (Svg msg)
getPathLine pairs =
    let
        path =
            let
                pointstrings =
                    List.map (\r -> String.fromFloat r.x ++ " " ++ String.fromFloat r.y) pairs

                start =
                    List.head pointstrings |> Maybe.withDefault "0 0"

                lstring =
                    String.join " " (List.map (\s -> "L" ++ s) pointstrings)
            in
            "M" ++ start ++ lstring
    in
    List.singleton (Svg.path [ class "path-line", d path ] [])


getLineSegments : List Datapair -> List (Svg msg)
getLineSegments pairs =
    let
        pairtuples =
            zip pairs (List.tail pairs |> Maybe.withDefault [])

        getSegment : ( Datapair, Datapair ) -> Svg msg
        getSegment tuple =
            let
                toString =
                    String.fromFloat

                first =
                    Tuple.first tuple

                second =
                    Tuple.second tuple
            in
            line
                [ class "line"
                , x1 (toString first.x)
                , y1 (toString first.y)
                , x2 (toString second.x)
                , y2 (toString second.y)
                ]
                []
    in
    List.map getSegment pairtuples


zip : List a -> List b -> List ( a, b )
zip list1 list2 =
    case ( list1, list2 ) of
        ( _, [] ) ->
            []

        ( [], _ ) ->
            []

        ( x :: xs, y :: ys ) ->
            ( x, y ) :: zip xs ys


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
    [ polyline [ points datapoints, class "line" ] [] ]
