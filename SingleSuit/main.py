#!/usr/local/bin/python
from pickle import PickleError
import sys
import json

from solver.classtypes.game_state import GameState, EMPTY_FIELD
from solver.solvers import find_optimal_move_dfool, find_optimal_move_fool, find_optimal_move_dfool_weighted, find_optimal_move_fool_weighted

def isStartStateValid(state_enc):
  state = json.loads(state_enc)
  fps = state["FirstPlayerSet"]
  sps = state["SecondPlayerSet"]
  if len(fps) != 0 and len(sps) != 0:
    return True
  return False

def joinGame(gameinfo, pid):
  players_joined = gameinfo["players_joined"]
  if pid in players_joined:
    raise Exception("Player already in game")
  elif len(players_joined) == 2:
    raise Exception("Game is full")
  if pid not in ["A", "B"]:
    raise Exception("Unknown player")
  if len(players_joined) == 1:
    gameinfo["is_ready"] = True
  return gameinfo

def processFinished(gameinfo, state, leading, can_draw):
  fps = state["FirstPlayerSet"]
  sps = state["SecondPlayerSet"]

  if state["Weights"] is not None and len(state["Weights"]) > 0:
    weights = state["Weights"]
    first_player_sum = sum([weights[i-1] for i in fps])
    second_player_sum = sum([weights[i-1] for i in sps])

    if len(fps) == 0 and len(sps) == 0:
      if can_draw:
        gameinfo["winner"] = ""
      else:
        gameinfo["winner"] = leading
      gameinfo["is_finished"] = True
    elif len(fps) == 0 and second_player_sum == 0 or len(sps) == 0 and first_player_sum == 0:
      if can_draw:
        gameinfo["winner"] = ""
      else:
        gameinfo["winner"] = "A" if len(fps) == 0 else "B"
      gameinfo["is_finished"] = True
    elif len(fps) == 0 and second_player_sum > 0 or len(sps) == 0 and first_player_sum < 0:
      gameinfo["is_finished"] = True
      gameinfo["winner"] = "A"
    elif len(fps) == 0 and second_player_sum < 0 or len(sps) == 0 and first_player_sum > 0:
      gameinfo["is_finished"] = True
      gameinfo["winner"] = "B"
  else:
    if len(fps) == 0 and len(sps) == 0:
      if can_draw:
        gameinfo["winner"] = ""
      else:
        gameinfo["winner"] = leading
      gameinfo["is_finished"] = True
    elif len(fps) == 0:
      gameinfo["winner"] = "A"
      gameinfo["is_finished"] = True
    elif len(sps) == 0:
      gameinfo["winner"] = "B"
      gameinfo["is_finished"] = True

def addEvent(gameinfo, pid, move_enc):
  players_joined = gameinfo["players_joined"]
  if len(players_joined) != 2:
    raise Exception("Game is not full")
  state = json.loads(gameinfo["state"])
  if pid != state["CurrentPlayer"]:
    raise Exception("Wrong player")
  move = json.loads(move_enc)

  should_swap = False
  can_draw = state["Type"] == "d-singlesuit"

  if move["DoTake"] == True:
    last_card = state["LastCard"]

    if last_card is None:
      raise Exception("Cant take nothing")

    should_swap = True
    if pid == "A":
      state["FirstPlayerSet"].append(last_card)
      processFinished(gameinfo, state, "B", can_draw)
    else:
      state["SecondPlayerSet"].append(last_card)
      processFinished(gameinfo, state, "A", can_draw)
    state["LastCard"] = None
  else:
    arr = None
    if pid == "A":
      arr = state["FirstPlayerSet"]
    else:
      arr = state["SecondPlayerSet"]
    card = move["Card"]
    if card not in arr:
      raise Exception("Player doesnt have that card")
    arr.remove(card)
    if state["LastCard"] == None:
      state["LastCard"] = card
      should_swap = True
    else:
      if card > state["LastCard"]:
        should_swap = False
      else:
        raise Exception("Cannot put a smaller card")
      processFinished(gameinfo, state, "B" if pid == "A" else "A", can_draw)
      state["LastCard"] = None
  if should_swap:
    if pid == "A":
      state["CurrentPlayer"] = "B"
    else:
      state["CurrentPlayer"] = "A"

  gameinfo["state"] = json.dumps(state)
  return gameinfo

def advance(gameinfo):
  gameinfo["is_finished"] = True
  state = json.loads(gameinfo["state"])
  gameinfo["winner"] = "A" if state["CurrentPlayer"] == "B" else "B"
  return gameinfo

def findOptimalMove(gameinfo):
  state = json.loads(gameinfo["state"])
  last_card = state["LastCard"]
  solverstate = GameState(
    int(state["CurrentPlayer"] == "B"),
    state["FirstPlayerSet"],
    state["SecondPlayerSet"],
    EMPTY_FIELD if last_card is None else last_card,
    state["Weights"]
  )

  is_dfool = state["Type"] == "d-singlesuit"
  is_weighted = state["Weights"] is not None and len(state["Weights"]) > 0
  picked_move = None

  if is_weighted and is_dfool:
    picked_move = find_optimal_move_dfool_weighted(solverstate)
  elif is_weighted:
    picked_move = find_optimal_move_fool_weighted(solverstate)
  elif is_dfool:
    picked_move = find_optimal_move_dfool(solverstate)
  else:
    picked_move = find_optimal_move_fool(solverstate)

  return {
    "DoTake": picked_move == last_card,
    "Card": picked_move
  }


def callWrapper(request):
  method = request["method"]
  if method == "isStartStateValid":
    return isStartStateValid(*request["params"])
  elif method == "joinGame":
    return joinGame(*request["params"])
  elif method == "addEvent":
    return addEvent(*request["params"])
  elif method == "advance":
    return advance(*request["params"])
  elif method == "findOptimalMove":
    return findOptimalMove(*request["params"])

  raise Exception("Unknown method")

def main():
  request_json = sys.stdin.read()
  request = json.loads(request_json)
  result = None
  try:
    result = callWrapper(request)
  except Exception as e:
    print(json.dumps(str(e)), file=sys.stderr)
    sys.exit(1)
  print(json.dumps(result))


if __name__ == "__main__":
  main()