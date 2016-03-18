import * as React from "react"
import * as Relay from "react-relay"

// Pass in gameId and userId or characterId or verified flah

export class UpdateGame extends Relay.Mutation {

  getMutation() {
    return Relay.QL`mutation { updateGame }`
  }

  getVariables() {
    return this.props
  }

  getFatQuery() {
    return Relay.QL`
      fragment on UpdateGamePayload {
        game
      }
    `
  }

  getConfigs(): Relay.MutationConfig[] {
    return [{
      type: "FIELDS_CHANGE",
      fieldIDs: {
        game: this.props.gameId
      }
    }]
  }
}
