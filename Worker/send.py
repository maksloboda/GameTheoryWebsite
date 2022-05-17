import pika

connection = pika.BlockingConnection(pika.ConnectionParameters('localhost'))
channel_valid = connection.channel()
channel_event = connection.channel()
channel_valid.queue_declare(queue='valid')
channel_event.queue_declare(queue='event')
channel_valid.basic_publish(exchange='',
                      routing_key='valid',
                      body='Hello Valid!')
channel_event.basic_publish(exchange='',
                      routing_key='event',
                      body='Hello Event!')
print(" [x] Sent 'Hello World!'")