import {Game} from "./Game"
import React from "react"
import Relay from "react-relay"

class GameScreenComponent extends React.Component {
  render() {
    let games = this.props.viewer.games.edges.map((edge) => <Game key={ edge.node.id } game={ edge.node } viewer={ this.props.viewer }/>)
    return (
      <div className="game-screen">
        <div className="games">
          { games }
        </div>
      </div>
    )
  }
}

export let GameScreen = Relay.createContainer(GameScreenComponent, {
  fragments: {
    viewer: () => Relay.QL`
      fragment on Viewer {
        games(first: 10) {
          edges {
            node {
              id
              ${Game.getFragment("game")}
            },
          },
        },

        ${Game.getFragment("viewer")}
      }
    `,
  },
});
