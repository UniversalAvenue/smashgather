CREATE TABLE characters (
  id SERIAL PRIMARY KEY,
  name TEXT
);

CREATE TABLE users (
  id SERIAL PRIMARY KEY,
  name TEXT NOT NULL,
  username TEXT NOT NULL,
  character_id INTEGER REFERENCES characters (id)
);

CREATE TABLE games (
  id SERIAL PRIMARY KEY,
  created_at TIMESTAMP WITH TIME ZONE,
  user_id INTEGER REFERENCES users (id),
  character_id INTEGER REFERENCES characters (id),
  verified BOOLEAN
);
