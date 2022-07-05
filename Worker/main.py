import asyncio
import os

function_list = [
  "isStartStateValid",
  "joinGame",
  "addEvent",
  "findOptimalMove",
  "advance",
]

async def run_function(function_name):
  """
  Runs a handler for the provided function
  """
  child_env = os.environ.copy()
  child_env["FUNC_NAME"] = function_name
  proc = await asyncio.create_subprocess_exec("python3", "wrapper.py",
      env=child_env)
  await proc.wait()

async def main():
  """
  Runs all the handlers
  Exists if one of the handlers is terminated
  """
  tasks = []
  for func in function_list:
    task = asyncio.create_task(run_function(func))
    tasks.append(task)
  await asyncio.wait(tasks, return_when=asyncio.FIRST_COMPLETED)
  exit(1)
    

if __name__ == "__main__":
  asyncio.run(main())