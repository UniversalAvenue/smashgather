import {Character} from "./Character"
import React from "react"
import Relay from "react-relay"

class CharacterScreenComponent extends React.Component {
  render() {
    let characters = this.props.viewer.characters.edges.map((edge) => {
      return <div key={ edge.node.id }><Character character={ edge.node } showStats={true} /></div>
    })
    return (
      <div className="character-screen">
        <div className="characters">
          { characters }
        </div>
      </div>
    )
  }
}

export let CharacterScreen = Relay.createContainer(CharacterScreenComponent, {
  fragments: {
    viewer: () => Relay.QL`
      fragment on Viewer {
        characters(first: 12) {
          edges {
            node {
              id
              ${Character.getFragment("character")}
            },
          },
        },
      }
    `,
  },
})
