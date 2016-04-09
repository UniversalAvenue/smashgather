import {Character} from "./Character"
import React from "react"
import Relay from "react-relay"

class UserComponent extends React.Component {
  render() {
    let { user, showStats, showVictory, onClick } = this.props
    if (user) {
      var { name, character } = user
      var portraitUrl = `img/${name.toLowerCase()}.png`
      if (showStats === "wins" || showStats === "weeklyWins") {
        var mainCharacter = <Character character={ character } />
        var winStats = <div className="win-stats">{ user[showStats] }</div>
      }
      if (showVictory) {
        var victory = <div className="victory-stats">+1</div>
      }
    } else {
      var name = "unknown"
      var portraitUrl = "img/unknown.png"
    }
    return (
      <div className={ "user " + name.toLowerCase() } onClick={ onClick }
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
        weeklyWins
        character {
          ${Character.getFragment("character")}
        }
      }
    `,
  },
})
