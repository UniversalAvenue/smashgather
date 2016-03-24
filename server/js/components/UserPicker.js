import * as React from "react"
import * as Relay from "react-relay"
import {User} from "./User"

export class UserPickerComponent extends React.Component {
  render() {

    let first = <User user={ this.props.current } />
    let rest = this.props.viewer.users.edges
      .map(edge => edge.node)
      .filter(user => user.id !== this.props.current.id)
      .map(user => <User key={ user.id } user={ user } onClick={ this.selectUser.bind(this, user) } />)

    return <div className="user-picker">
      { first }
      { rest }
    </div>
  }

  selectUser(user) {
    if (this.props.onSelect) {
      this.props.onSelect(user)
    }
  }
}

export let UserPicker = Relay.createContainer(UserPickerComponent, {
  fragments: {
    viewer: () => Relay.QL`
      fragment on Viewer {
        users(first: 1000) {
          edges {
            node {
              id
              name

              ${ User.getFragment("user") }
            }
          }
        }
      }
    `
  },
})
