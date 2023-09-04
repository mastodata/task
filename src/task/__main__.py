import os

import zmq

if __name__ == '__main__':
    ctx = zmq.Context()
    task_address = os.environ["TASK_ADDRESS"]
    sink_address = os.environ["SINK_ADDRESS"]

    print(f"Starting with {task_address =}, {sink_address =}")

    # To pull from vent
    pull = ctx.socket(zmq.PULL)
    pull.bind(task_address)

    # To push to sink
    push = ctx.socket(zmq.PUSH)
    push.connect(sink_address)

    while True:
        status = pull.recv_json()
