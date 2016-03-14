DROP TABLE characters;
CREATE TABLE characters (
  id INTEGER PRIMARY KEY,
  name TEXT
);

DROP TABLE users;
CREATE TABLE users (
  id INTEGER PRIMARY KEY,
  name TEXT,
  username TEXT,
  character_id INTEGER
);

DROP TABLE games;
CREATE TABLE games (
  id INTEGER PRIMARY KEY,
  created_at TIMESTAMP WITH TIME ZONE,
  user_id INTEGER,
  character_id INTEGER,
  verified BOOLEAN
);
