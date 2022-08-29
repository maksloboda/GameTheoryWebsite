#!/usr/local/bin/python
import sys
import json
from solver.solver import find_optimal_move, EMPTY_FIELD, GameState

def isStartStateValid(enc_state):
  state = json.loads(enc_state)
  fps = state["FirstPlayerSet"]
  sps = state["SecondPlayerSet"]
  return len(fps) != 0 and len(sps) != 0 and len(fps) == len(sps)

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

# state looks like
# {
#   int[] FirstPlayerSet
#   int[] SecondPlayerSet
#   int FirstScore
#   int SecondScore
#   int LastCard
# }
def processFinished(gameinfo, state):
  fps = state["FirstPlayerSet"]
  sps = state["SecondPlayerSet"]
  is_tiny = state["Version"] == "tiny"

  if len(fps) == 0 and len(sps) == 0:
    if state["FirstScore"] > state["SecondScore"]:
      gameinfo["winner"] = "A" if not is_tiny else "B"
    elif state["FirstScore"] < state["SecondScore"]:
      gameinfo["winner"] = "B" if not is_tiny else "A"
    else:
      gameinfo["winner"] = ""
    gameinfo["is_finished"] = True

# move looks like
# {
#   int Card
# }
def addEvent(gameinfo, pid, move_enc):
  players_joined = gameinfo["players_joined"]
  if len(players_joined) != 2:
    raise Exception("Game is not full")
  state = json.loads(gameinfo["state"])
  if pid != state["CurrentPlayer"]:
    raise Exception("Wrong player")
  move = json.loads(move_enc)

  should_swap = False

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
    winner = ""
    if card < state["LastCard"]:
      should_swap = True
      if pid == "A":
        winner = "SecondScore"
      else:
        winner = "FirstScore"
    else:
      if pid == "A":
        winner = "FirstScore"
      else:
        winner = "SecondScore"
    
    if state["Weights"] is not None and len(state["Weights"]) > 0:
      state[winner] += state["Weights"].pop(0)
    else:
      state[winner] += 1

    processFinished(gameinfo, state)
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
    state["FirstPlayerSet"],
    state["SecondPlayerSet"],
    int(state["CurrentPlayer"] == "B"),
    EMPTY_FIELD if last_card is None else last_card,
    state["Weights"],
    state["Version"]
  )
  
  return {
    "Card": find_optimal_move(solverstate)
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