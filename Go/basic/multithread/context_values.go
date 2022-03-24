package multithread

import (
	"context"
	"fmt"
	"math/rand"
	"time"
)

func ContextValuesExample() {
	rand.Seed(time.Now().UnixNano())
	ctx := context.Background()
	ctx = context.WithValue(ctx, "id", rand.Int())
	operation1(ctx)
}

func operation1(ctx context.Context) {
	fmt.Println("op1 id=", ctx.Value("id"))
	operation2(ctx)
}

func operation2(ctx context.Context) {
	fmt.Println("op2 id=", ctx.Value("id"))
}