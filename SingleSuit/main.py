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
  players_joined.append(pid)
  if len(players_joined) == 2:
    gameinfo["is_ready"] = True
  return gameinfo

def addEvent(gameinfo, pid, move_enc):
  players_joined = gameinfo["players_joined"]
  if len(players_joined) != 2:
    raise Exception("Game is not full")
  state = json.loads(gameinfo["state"])
  if pid != state["CurrentPlayer"]:
    raise Exception("Wrong player")
  move = json.loads(move_enc)

  should_swap = False

  if move["DoTake"] == True:
    last_card = state["LastCard"]
    should_swap = True
    if pid == "A":
      state["FirstPlayerSet"].append(last_card)
    else:
      state["SecondPlayerSet"].append(last_card)
    state["LastCard"] = None
  else:
    arr = None
    if pid == "A":
      arr = state["FirstPlayerSet"]
    else:
      arr = state["SecondPlayerSet"]
    card = move["Card"]
    arr.remove(card)
    if state["LastCard"] == None:
      state["LastCard"] = card
      should_swap = True
    else:
      if card < state["LastCard"]:
        should_swap = True
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
    print(json.dumps(str(e)))
    sys.exit(1)
  print(json.dumps(result))
  

if __name__ == "__main__":
  main()