import {Game} from "./Game"
import React from "react"
import Relay from "react-relay"

class GameScreenComponent extends React.Component {
  render() {
    let games = this.props.viewer.games.edges.map((edge) => <Game key={ edge.node.id } game={ edge.node } viewer={ this.props.viewer }/>)

    if (this.props.viewer.games.pageInfo.hasNextPage) {
      var loadMore = <div className="load-more">
        <a href="#" onClick={ this.loadMore.bind(this) } >LOAD MORE</a>
      </div>
    }
    return (
      <div className="game-screen">
        <div className="games">
          { games }
        </div>
        { loadMore }
      </div>
    )
  }

  loadMore(evt) {
    evt.preventDefault();
    let { count } = this.props.relay.variables
    this.props.relay.setVariables({ count: count + 10 })
  }
}

export let GameScreen = Relay.createContainer(GameScreenComponent, {
  initialVariables: {
    count: 10
  },
  fragments: {
    viewer: () => Relay.QL`
      fragment on Viewer {
        games(first: $count) {
          edges {
            node {
              id
              ${Game.getFragment("game")}
            },
          },
          pageInfo {
           hasNextPage
          }
        },

        ${Game.getFragment("viewer")}
      }
    `,
  },
});
