import React from "react"
import Relay from "react-relay"
import {User} from "./User"

class UserScreenComponent extends React.Component {
  render() {
    let users = this.props.viewer.users.edges.map((edge) => {
      return <div key={ edge.node.id }><User user={ edge.node } showStats={true} /></div>
    })
    return (
      <div className="user-screen">
        <div className="users">
          { users }
        </div>
      </div>
    )
  }
}

export let UserScreen = Relay.createContainer(UserScreenComponent, {
  fragments: {
    viewer: () => Relay.QL`
      fragment on Viewer {
        users(first: 10) {
          edges {
            node {
              id
              ${User.getFragment("user")}
            },
          },
        },
      }
    `,
  },
})
