import {CharacterScreen} from "./CharacterScreen"
import {GameScreen} from "./GameScreen"
import React from "react"
import Relay from "react-relay"
import {UserScreen} from "./UserScreen"
import ViewerRoute from "../routes/ViewerRoute"

export class Smashgather extends React.Component {
  state = {
    selectedScreen: "GAMES",
  }

  renderSelectedScreen() {
    let screen = {
      "GAMES": GameScreen,
      "USERS": UserScreen,
      "CHARACTERS": CharacterScreen,
    }[this.state.selectedScreen]

    return <Relay.RootContainer
      Component={screen}
      route={new ViewerRoute()}
    />
  }

  selectScreen(selectedScreen) {
    this.setState({ selectedScreen })
  }

  render() {
    let screen = this.renderSelectedScreen()
    return (
      <div className="app-container">
        <div className="tab-bar">
          <Tab selected={ this.state.selectedScreen } onTabClick={(name) => this.selectScreen(name)} name="GAMES" />
          <Tab selected={ this.state.selectedScreen } onTabClick={(name) => this.selectScreen(name)} name="USERS" />
          <Tab selected={ this.state.selectedScreen } onTabClick={(name) => this.selectScreen(name)} name="CHARACTERS" />
        </div>
        <div className="screen">{ screen }</div>
      </div>
    )
    return <CharacterScreen viewer={ this.props.viewer } />
  }
}

const Tab = ({ name, selected, onTabClick }) => {
  if (name == selected) {
    return <span className="tab active">{ name }</span>
  } else {
    return <a href="#" className="tab" onClick={onTabClick.bind(this, name)}>{ name }</a>
  }
}
