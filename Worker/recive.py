import pika
import subprocess
import sys

def callback_valid(ch, method, properties, body):
    result = subprocess.run([sys.executable, '/home/danil/HW/Proj/Game.py'], input=repr(body),capture_output=True, text=True, check=True)   #change the path
    print("stdout:", result.stdout)
def callback_event(ch, method, properties, body):
    result = subprocess.run([sys.executable, '/home/danil/HW/Proj/Game.py'], input=repr(body),capture_output=True, text=True, check=True)   #change the path
    print("stdout:", result.stdout)

connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel_valid = connection.channel()
channel_event = connection.channel()
channel_valid.queue_declare(queue='valid')
channel_event.queue_declare(queue='event')

channel_valid.basic_consume(queue='valid',
                      auto_ack=True,
                      on_message_callback=callback_valid)
channel_event.basic_consume(queue='event',
                      auto_ack=True,
                      on_message_callback=callback_event)

print(' [*] Waiting for messages. To exit press CTRL+C')
channel_valid.start_consuming()
channel_event.start_consuming()

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('Interrupted')
        try:
            sys.exit(0)
        except SystemExit:
            os._exit(0)