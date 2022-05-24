package main

import (
	"log"
	"matchmaking/gamemanager"
	"matchmaking/graph"
	"matchmaking/graph/generated"
	"net/http"
	"os"
	"time"

	"github.com/99designs/gqlgen/graphql/handler"
	"github.com/99designs/gqlgen/graphql/handler/extension"
	"github.com/99designs/gqlgen/graphql/handler/transport"
	"github.com/99designs/gqlgen/graphql/playground"
	"github.com/go-redis/redis"
	"github.com/gorilla/websocket"
	"github.com/streadway/amqp"
)

const defaultPort = "8080"

func NewRedisClient(url string) (*redis.UniversalClient, error) {
	redis_client := redis.NewUniversalClient(&redis.UniversalOptions{
		Addrs: []string{url},
	})

	_, err := redis_client.Ping().Result()

	return &redis_client, err
}

type CorsServer struct {
	Handler http.Handler
}

func (s CorsServer) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Access-Control-Allow-Methods", "*")
	w.Header().Set("Access-Control-Allow-Headers", "*")
	s.Handler.ServeHTTP(w, r)
}

func main() {
	port := os.Getenv("PORT")
	if port == "" {
		port = defaultPort
	}

	redis_url := os.Getenv("REDIS_URL")
	redis_client, err := NewRedisClient(redis_url)
	if err != nil {
		panic(err)
	}

	rabbitmq_url := os.Getenv("RABBITMQ_URL")
	rabbitmq_client, err := amqp.Dial(rabbitmq_url)
	if err != nil {
		panic(err)
	}

	gamemanager.SetAMQPConnection(rabbitmq_client)
	gamemanager.SetRedisClient(redis_client)
	graph.SetRedisClient(redis_client)

	srv := handler.New(generated.NewExecutableSchema(generated.Config{Resolvers: &graph.Resolver{}}))

	srv.AddTransport(transport.Websocket{
		KeepAlivePingInterval: 10 * time.Second,
		Upgrader: websocket.Upgrader{
			CheckOrigin: func(r *http.Request) bool {
				return true
			},
		},
	})
	srv.AddTransport(transport.Options{})
	srv.AddTransport(transport.GET{})
	srv.AddTransport(transport.POST{})
	srv.Use(extension.Introspection{})

	http.Handle("/", playground.Handler("GraphQL playground", "/query"))

	http.Handle("/query", CorsServer{Handler: srv})

	log.Printf("connect to http://localhost:%s/ for GraphQL playground", port)
	log.Fatal(http.ListenAndServe(":"+port, nil))
}
