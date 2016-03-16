# Smashgather Server

This is an (overly complicated) Relay server and corresponding React app to store and render the Smashgather data. It's based on [relay-starter-kit](https://github.com/relayjs/relay-starter-kit)

## Getting Started
It's a node project, so it's pretty easy:
`npm install`

Make sure you have Postgres installed locally, then run this command to create the database:
`npm run create-db`

## Running
`npm run start-dev`: start the webpack dev server; open http://localhost:3000 to open the app
`npm run update-schema`: rebuild the GraphQL schema for the Relay app (needs to be done after every schema change)
`npm run build`: compile the React app (to `public/assets`) and production server to (`dist/production.js`)
`npm run clean`: delete all build artifacts
`npm start`: run the production server from `dist/production.js`
