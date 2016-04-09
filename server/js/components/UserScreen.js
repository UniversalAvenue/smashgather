import React from "react"
import Relay from "react-relay"
import {User} from "./User"

class UserScreenComponent extends React.Component {
  render() {

    let users = this.props.viewer.users.edges.map(edge => {
      return <User key={ edge.node.id } user={ edge.node } showStats="weeklyWins" />
    })

    if (this.props.relay.variables.order == "weeklyWins") {
      var orderPicker = <div className="order-picker">
        <span className="active">WEEKLY</span>
        <span>|</span>
        <a href="#" onClick={ this.setOrder.bind(this, "wins") } >ALL-TIME</a>
      </div>
    } else {
      var orderPicker = <div className="order-picker">
        <a href="#" onClick={ this.setOrder.bind(this, "weeklyWins") } >WEEKLY</a>
        <span>|</span>
        <span className="active">ALL-TIME</span>
      </div>
    }

    return (
      <div className="user-screen">
        <div className="users">
          { users }
        </div>
        { orderPicker }
      </div>
    )
  }

  setOrder(order, evt) {
    evt.preventDefault()
    this.props.relay.setVariables({ order })
  }
}

export let UserScreen = Relay.createContainer(UserScreenComponent, {
  initialVariables: {
    order: "weeklyWins"
  },
  fragments: {
    viewer: () => Relay.QL`
      fragment on Viewer {
        users(first: 10, order: $order) {
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
