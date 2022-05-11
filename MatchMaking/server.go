package main

import (
	"log"
	"matchmaking/gamemanager"
	"matchmaking/graph"
	"matchmaking/graph/generated"
	"net/http"
	"os"

	"github.com/99designs/gqlgen/graphql/handler"
	"github.com/99designs/gqlgen/graphql/playground"
	"github.com/go-redis/redis"
)

const defaultPort = "8080"

func NewRedisClient(url string) (*redis.UniversalClient, error) {
	redis_client := redis.NewUniversalClient(&redis.UniversalOptions{
		Addrs: []string{url},
	})

	_, err := redis_client.Ping().Result()

	return &redis_client, err
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

	gamemanager.SetRedisClient(redis_client)
	graph.SetRedisClient(redis_client)

	srv := handler.NewDefaultServer(generated.NewExecutableSchema(generated.Config{Resolvers: &graph.Resolver{}}))

	http.Handle("/", playground.Handler("GraphQL playground", "/query"))
	http.Handle("/query", srv)

	log.Printf("connect to http://localhost:%s/ for GraphQL playground", port)
	log.Fatal(http.ListenAndServe(":"+port, nil))
}
