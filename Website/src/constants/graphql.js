import gql from 'graphql-tag'

export const GET_GAME_INFO_QUERY = gql`
  query GameInfoQuery($game_id: ID!) {
    gameInfo(id: $game_id) {
      id
      players_joined
      game_name
      event_clock
      state
      is_ready
      is_finished
      winner
    }
  }
`
export const FIND_OPTIMAL_MOVE_QUERY = gql`
  query FindOptimalMoveQuery($game_id: ID!) {
    findOptimalMove(id: $game_id)
  }
`

export const CREATE_GAME_MUTATION = gql`
  mutation CreateGameMutation($game_name: String!, $start_state: GameState!) {
    createGame(game_name: $game_name, start_state: $start_state)
  }
`

export const JOIN_GAME_MUTATION = gql`
  mutation JoinGameMutation($game_id: ID!, $player_id: PlayerIdentifier!) {
    joinGame(id: $game_id, pid: $player_id)
  }
`

export const ADD_EVENT_MUTATION = gql`
  mutation AddEventMutation($game_id: ID!, $player_token: PlayerToken!, $event: GameEvent!) {
    addEvent(id: $game_id, token: $player_token, event: $event)
  }
`
export const GAME_SUBSCRIPTION = gql`
  subscription  GameSubscription($game_id: ID!) {
    subcribeGame(id: $game_id) {
      id
      players_joined
      game_name
      event_clock
      state
      is_ready
      is_finished
      winner
    }
  }
`