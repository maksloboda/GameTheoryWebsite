import multiprocessing as mp
from worker_module import worker
import sys
import os

''' 
    takes path to executable (string)
    returns Process object
'''

def init_worker(executable, pipe_read, pipe_write):
    p = mp.Process(target=worker, args=(executable, pipe_read, pipe_write))
    return p


''' 
    takes path to the executable and number of workers
    returns array of Process objects and two arrays of file descriptors
'''

def init_workers(path_to_executable, number): 
    workers = []
    input_fds = []
    output_fds = []
    for i in range(0, number): 
        r_to_worker, w_to_worker = os.pipe()
        r_from_worker, w_from_worker = os.pipe()
        worker_process = init_worker(path_to_executable, r_to_worker, w_from_worker)

        workers.append(worker_process)
        input_fds.append(w_to_worker)
        output_fds.append(r_from_worker)

    return workers, input_fds, output_fds


if __name__ == "__main__":
    if (len(sys.argv) < 3):
        print("Incorrect arguments number")

    path_to_exec = sys.argv[1]
    workers_num = int(sys.argv[2])

    workers, input_fds, output_fds = init_workers(path_to_exec, workers_num)

    for i in range(0, workers_num):
        print(input_fds[i], output_fds[i])

    for p in workers:
        p.start()      

    for fd in input_fds:
        for i in range(0, 10):
            os.write(fd, (str(i) + "\n").encode('utf-8'))
            
    for p in workers:
        p.join()

    for fd in input_fds:
        os.close(fd)

    for fd in output_fds:
        os.close(fd)