package main

import (
    "fmt"
    "log"
    "net/http"
		"os"
		"sync"
		"strconv"
)

var state = map[string]int{}
var next_state_id = 1
var state_lock sync.Mutex

func main() {

		port, is_port_specified := os.LookupEnv("HTTP_PORT")
		if !is_port_specified {
			port = "8008"
		}
    http.HandleFunc("/add_task", AddTask)
    fmt.Println("Server started at port " + port)
    log.Fatal(http.ListenAndServe(":" + port, nil))
}

func AddTask(w http.ResponseWriter, r *http.Request) {
	state_lock.Lock()
	current_state := next_state_id;
	next_state_id += 1
	id := strconv.Itoa(current_state);
	state[id] = 0
	state_lock.Unlock()
	fmt.Fprintf(w, id)
}