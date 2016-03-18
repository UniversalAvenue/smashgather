import {Character} from "./Character"
import moment from "moment"
import React from "react"
import Relay from "react-relay"
import {User} from "./User"

class GameComponent extends React.Component {
  render() {
    let { createdAt, character, user } = this.props.game
    return (
      <div className="game">
        <Character character={ character } showStats={ false } showVictory={ true } />
        <User user={ user } showStats={ false } showVictory={ true } />
        <div>{ moment(new Date(createdAt)).fromNow() }</div>
      </div>
    )
  }
}

export let Game = Relay.createContainer(GameComponent, {
  fragments: {
    game: () => Relay.QL`
      fragment on Game {
        createdAt
        character {
          ${Character.getFragment("character")}
        }
        user {
          ${User.getFragment("user")}
        }
        verified
      }
    `,
  },
})
