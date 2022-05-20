import gql from 'graphql-tag'

export const GET_GAME_INFO_QUERY = gql`
  query GameInfoQuery($id: ID!) {
    id
    players_joined {
      player_identifier
    }
    game_name
    event_clock
    state
  }
`