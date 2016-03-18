import React from "react"
import Relay from "react-relay"

class CharacterComponent extends React.Component {
  render() {
    let { name } = this.props.character
    let portraitUrl = `img/${name}.png`
    if (this.props.showStats) {
      var wins = <div className="win-stats">15</div>
    }
    if (this.props.showVictory) {
      var victory = <div className="victory-stats">+1</div>
    }
    return (
      <div className="character"
        style={{
          background: `url("${portraitUrl}")`,
          backgroundSize: "cover"
        }}>
        <div className="stats">{ wins }{ victory }</div>
      </div>
    )
  }
}

export let Character = Relay.createContainer(CharacterComponent, {
  fragments: {
    character: () => Relay.QL`
      fragment on Character {
        name
      }
    `,
  },
})
