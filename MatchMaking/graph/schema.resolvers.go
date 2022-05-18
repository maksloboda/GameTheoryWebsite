package graph

// This file will be automatically regenerated based on the schema, any resolver implementations
// will be copied through when generating and any unknown code will be moved to the end.

import (
	"context"
	"errors"
	"matchmaking/gamemanager"
	"matchmaking/graph/generated"
	"matchmaking/graph/model"

	"github.com/go-redis/redis"
)

func (r *mutationResolver) CreateGame(ctx context.Context, gameName string, startState string) (*string, error) {
	id, err := gamemanager.CreateGame(gameName, startState)
	return &id, err
}

func (r *mutationResolver) JoinGame(ctx context.Context, id string, pid string) (string, error) {
	token, err := gamemanager.AddPlayer(id, pid)
	return token, err
}

func (r *mutationResolver) AddEvent(ctx context.Context, id string, token string, event string) (*bool, error) {
	err := gamemanager.AddEvent(id, token, event)
	is_error := err == nil
	return &is_error, err
}

func (r *queryResolver) GameInfo(ctx context.Context, id string) (*model.GameInfo, error) {
	game_info, err := gamemanager.GetGameInfo(id)
	return game_info, err
}

func (r *subscriptionResolver) SubcribeGame(ctx context.Context, id string) (<-chan *model.GameInfo, error) {
	return gamemanager.SubscribeGame(id)
}

// Mutation returns generated.MutationResolver implementation.
func (r *Resolver) Mutation() generated.MutationResolver { return &mutationResolver{r} }

// Query returns generated.QueryResolver implementation.
func (r *Resolver) Query() generated.QueryResolver { return &queryResolver{r} }

// Subscription returns generated.SubscriptionResolver implementation.
func (r *Resolver) Subscription() generated.SubscriptionResolver { return &subscriptionResolver{r} }

type mutationResolver struct{ *Resolver }
type queryResolver struct{ *Resolver }
type subscriptionResolver struct{ *Resolver }

// !!! WARNING !!!
// The code below was going to be deleted when updating resolvers. It has been copied here so you have
// one last chance to move it out of harms way if you want. There are two reasons this happens:
//  - When renaming or deleting a resolver the old code will be put in here. You can safely delete
//    it when you're done.
//  - You have helper methods in this file. Move them out to keep these resolver files clean.
var redis_client *redis.UniversalClient

func SetRedisClient(client *redis.UniversalClient) {
	redis_client = client
}
func NewRedisClient(url string) (*redis.Client, error) {
	client := redis.NewClient(&redis.Options{
		Addr: url,
	})

	_, err := client.Ping().Result()
	if !errors.Is(err, nil) {
		return nil, err
	}

	return client, nil
}
