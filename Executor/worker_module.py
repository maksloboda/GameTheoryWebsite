import os

def worker(executable, input_fd, output_fd):
    print(f"Worker {os.getpid()} started")
    os.dup2(input_fd, 0)
    os.dup2(output_fd, 1)
    os.execl(executable, executable)
    os._exit(1) # just in case