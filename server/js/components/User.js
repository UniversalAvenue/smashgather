import {Character} from "./Character"
import React from "react"
import Relay from "react-relay"

class UserComponent extends React.Component {
  render() {
    let { name, character } = this.props.user
    let portraitUrl = `img/${name}.png`
    if (this.props.showStats) {
      var mainCharacter = <Character character={ character } showStats={ false } />
      var wins = <div className="win-stats">15</div>
    }
    if (this.props.showVictory) {
      var victory = <div className="victory-stats">+1</div>
    }
    return (
      <div className="user"
        style={{
          background: `url("${portraitUrl}")`,
          backgroundSize: "cover"
        }}>
        <div className="stats">{ mainCharacter }{ wins }{ victory }</div>
      </div>
    )
  }
}

export let User = Relay.createContainer(UserComponent, {
  fragments: {
    user: () => Relay.QL`
      fragment on User {
        name
        character {
          ${Character.getFragment("character")}
        }
      }
    `,
  },
})
