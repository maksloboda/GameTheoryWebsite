import gql from 'graphql-tag'

export const GET_GAME_INFO_QUERY = gql`
  query GameInfoQuery($game_id: ID!) {
    gameInfo(id: $game_id) {
      id
      players_joined
      game_name
      event_clock
      state
    }
  }
`

export const HELLO_WORLD_QUERY = gql`
  query HelloWorldQuery($game_id: ID!) {
    hello
  }
`