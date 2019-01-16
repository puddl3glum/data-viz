import Browser
import Html exposing (Html, button, div, text)
import Html.Events exposing (onClick)

import Plot
import Models exposing (Model)

main =
  Browser.sandbox { init = init, update = update, view = view }


-- MODEL

init : Model
init =
  {data = []}

-- UPDATE
 
type Msg = Bar

update : Msg -> Model -> Model
update msg model =
  case msg of
    Bar ->
      model


-- VIEW

view : Model -> Html Msg
view model =
  div []
    [ button [ onClick Bar ] [ text "-" ]
    , div [] [ text "Hello" ]
    -- , button [ onClick Increment ] [ text "+" ]
    , Plot.makeBarPlot model
    ]