import pika
import subprocess
import sys
import os
import json

def getEnvOrThrow(name):
    result = os.getenv(name, None)
    if result is None:
        raise AttributeError("%s not found in evironment" % name)
    return result

def pika_respond(request_id, pika_connection, response):
    tmp = pika_connection.channel()
    tmp.basic_publish(exchange='',
                      routing_key='resp_%s' % request_id,
                      body=json.dumps(response))

def handle_func(function_name, pika_connection, executable_path, message_body):
    error_cause = ""
    request_id = ""
    result = None
    try:
        parsed = json.loads(message_body)
        params = parsed.get("params")
        request_id = parsed.get("id")
        if params is None or not isinstance(params, list):
            raise AttributeError("Invalid params")
        if request_id is None or not isinstance(request_id, str):
            raise AttributeError("Invalid id")
        enc_req = json.dumps({
            "method": function_name,
            "params": params
            }, indent=1)
        result = subprocess.run([executable_path],
            input=enc_req, capture_output=True, text=True, check=False)
        if result.returncode == 0:
            pika_respond(request_id, pika_connection, {
            "result": result.stdout
            })
        else:
            pika_respond(request_id, pika_connection, {
            "error": result.stderr
            })
        return
    except json.JSONDecodeError:
        error_cause = "Malformed json"
    except AttributeError:
        error_cause = "Missing fields"
    pika_respond(request_id, pika_connection, {
        "error": error_cause
    })

def main():
    executable_path = getEnvOrThrow("EXEC_PATH")
    listen_path = os.getenv("RABBITMQ_URL", "localhost")
    listen_function = getEnvOrThrow("FUNC_NAME")
    game_name = getEnvOrThrow("GAME_NAME")

    connection = pika.BlockingConnection(pika.ConnectionParameters(listen_path))
    channel_function = connection.channel()
    queue_name = 'func_%s_%s' % (game_name, listen_function)
    channel_function.queue_declare(queue=queue_name, auto_delete=True)

    def callback_func(ch, method, properties, body):
        handle_func(listen_function, connection, executable_path, body)

    channel_function.basic_consume(queue=queue_name,
                        auto_ack=True,
                        on_message_callback=callback_func)

    print(' [*] Waiting for messages. To exit press CTRL+C')
    channel_function.start_consuming()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('Interrupted')
        sys.exit(0)