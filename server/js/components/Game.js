import {Character} from "./Character"
import moment from "moment"
import React from "react"
import Relay from "react-relay"
import {User} from "./User"
import {UserPicker} from "./UserPicker"
import {UpdateGame} from "../mutations/UpdateGame"

class GameComponent extends React.Component {

  state = {
    editing: false
  }

  render() {
    let { createdAt, character, user } = this.props.game

    if (this.state.editing) {
      var editor = <UserPicker
        current={ user }
        onSelect={ this.updateUser.bind(this) }
        viewer={ this.props.viewer } />
    }

    return (
      <div className="game">
        <Character character={ character } showStats={ false } showVictory={ true } />
        <div className="winning-user">
          <User user={ user } showStats={ false } showVictory={ true } onClick={ this.toggleEditing.bind(this) } />
          { editor }
        </div>
        <div>{ moment(new Date(createdAt)).fromNow() }</div>
      </div>
    )
  }

  toggleEditing(evt) {
    this.setState({editing: !this.state.editing})
  }

  updateUser(user) {
    Relay.Store.commitUpdate(new UpdateGame({
      gameId: this.props.game.id,
      userId: user.id
    }))
    this.setState({editing: false})
  }
}

export let Game = Relay.createContainer(GameComponent, {
  fragments: {
    game: () => Relay.QL`
      fragment on Game {
        id
        createdAt
        character {
          ${Character.getFragment("character")}
        }
        user {
          id
          ${User.getFragment("user")}
        }
        verified
      }
    `,
    viewer: () => Relay.QL`
      fragment on Viewer {
        ${ UserPicker.getFragment("viewer") }
      }
    `
  },
})
