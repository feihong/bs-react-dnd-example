// Return copy of given array, switching the elements at the given positions
let reorder = (arr, index1, index2) => {
  let arr = Array.copy(arr);
  switch (arr |> Js.Array.spliceInPlace(~pos=index1, ~remove=1, ~add=[||])) {
  | [|removed|] =>
    arr
    |> Js.Array.spliceInPlace(~pos=index2, ~remove=0, ~add=[|removed|])
    |> ignore
  | _ => ()
  };
  arr;
};

type action =
  | MoveCard(int, int)
  | DropCard(int, int);

type state = {cards: array(T.card)};

let component = ReasonReact.reducerComponent(__MODULE__);

let make = _children => {
  ...component,
  initialState: () => {
    cards:
      Array.makeBy(30, i =>
        {T.id: i, text: "Item " ++ string_of_int(i + 1)}
      ),
  },
  reducer: (action, state) =>
    switch (action) {
    | MoveCard(dragId, hoverId) =>
      let cards = state.cards;
      let dragIndex = cards |> Js.Array.findIndex(card => card.T.id == dragId);
      let hoverIndex =
        cards |> Js.Array.findIndex(card => card.T.id == hoverId);

      Js.log3("MoveCard:", dragIndex, hoverIndex);

      ReasonReact.Update({
        cards: state.cards->reorder(dragIndex, hoverIndex),
      });
    | DropCard(id, startIndex) =>
      ReasonReact.SideEffects(
        _ => {
          let currentIndex =
            state.cards |> Js.Array.findIndex(card => card.T.id == id);
          if (startIndex == currentIndex) {
            Js.log("Card was dropped into old position!");
          } else {
            state.cards
            ->Array.get(currentIndex)
            ->Option.map(card =>
                Js.log2("Card was dropped into new position:", card)
              )
            ->ignore;
          };
        },
      )
    },
  render: ({state, send}) => {
    <BsReactDnd.DragDropContextProvider backend=BsReactDnd.Backend.html5>
      <div style={ReactDOMRe.Style.make(~width="400", ())}>
        {state.cards
         ->Array.mapWithIndex((index, card) =>
             <Card
               key={string_of_int(card.id)}
               id={card.id}
               text={card.text}
               index
               moveCard={(dragIndex, hoverIndex) =>
                 send(MoveCard(dragIndex, hoverIndex))
               }
               dropCard={(id, startIndex) => send(DropCard(id, startIndex))}
             />
           )
         ->ReasonReact.array}
      </div>
    </BsReactDnd.DragDropContextProvider>;
  },
};
