import React from "react";
import Relay from "react-relay";

class App extends React.Component {
  render() {
    return (
      <div>
        <h1>Games</h1>
        <ul>
          {this.props.viewer.games.edges.map(edge =>
            <li key={edge.node.id}>
              <div>createdAt: { edge.node.createdAt }</div>
              <div>user: { edge.node.user.name }</div>
              <div>character: { edge.node.character.name }</div>
              <div>verified: { edge.node.verified ? "true" : "false" }</div>
            </li>
          )}
        </ul>
      </div>
    );
  }
}

export default Relay.createContainer(App, {
  fragments: {
    viewer: () => Relay.QL`
      fragment on Viewer {
        games(first: 10) {
          edges {
            node {
              id
              createdAt
              user { name }
              character { name }
              verified
            },
          },
        },
      }
    `,
  },
});
