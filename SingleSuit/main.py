import sys
import json


def isStartStateValid(enc_state):
  return True

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
  

def callWrapper(request):
  method = request["method"]
  if method == "isStartStateValid":
    return isStartStateValid(*request["params"])
  elif method == "joinGame":
    return joinGame(*request["params"])
  elif method == "addEvent":
    return addEvent(*request["params"])

  raise Exception("Unknown method")

# def 

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