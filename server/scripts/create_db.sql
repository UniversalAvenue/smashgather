DROP TABLE characters;
CREATE TABLE characters (
  id INTEGER PRIMARY KEY,
  name TEXT
);

DROP TABLE users;
CREATE TABLE users (
  id INTEGER PRIMARY KEY,
  name TEXT NOT NULL,
  username TEXT NOT NULL,
  character_id INTEGER REFERENCES characters (id)
);

DROP TABLE games;
CREATE TABLE games (
  id INTEGER PRIMARY KEY,
  created_at TIMESTAMP WITH TIME ZONE,
  user_id INTEGER REFERENCES users (id),
  character_id INTEGER REFERENCES characters (id),
  verified BOOLEAN
);
