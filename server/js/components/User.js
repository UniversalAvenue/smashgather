import {Character} from "./Character"
import React from "react"
import Relay from "react-relay"

class UserComponent extends React.Component {
  render() {
    let { name, wins, character } = this.props.user
    let portraitUrl = `img/${name.toLowerCase()}.png`
    if (this.props.showStats) {
      var mainCharacter = <Character character={ character } showStats={ false } />
      var winStats = <div className="win-stats">{ wins }</div>
    }
    if (this.props.showVictory) {
      var victory = <div className="victory-stats">+1</div>
    }
    return (
      <div className={ "user " + name.toLowerCase() } onClick={ this.props.onClick }
        style={{
          background: `url("${portraitUrl}")`,
          backgroundSize: "cover"
        }}>
        <div className="stats">{ mainCharacter }{ winStats }{ victory }</div>
      </div>
    )
  }
}

export let User = Relay.createContainer(UserComponent, {
  fragments: {
    user: () => Relay.QL`
      fragment on User {
        name
        wins
        character {
          ${Character.getFragment("character")}
        }
      }
    `,
  },
})
