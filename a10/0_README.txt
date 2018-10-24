SERVER:
has a signal handler to receive SIGCHLD to reap zombies
starts on a random port
repeat:
	awaits 2 incoming connections
	The 1st connection is player 1
	The 2nd connection is player 2
	Spawns a child process to manage that game

SERVER CHILD
sends an 'x' or an 'o' to each player as one byte
repeat:
	sends the game state to both players
	awaits a move from player who's turn it is
	receives move from player as a char of one byte [0-8]
	plays move
until:
	someone disconnects OR
	game over
then: disconnects both players


CLIENT:
connects to server (port # is first argument, hostname is optionally 2nd)
receives 'x' or 'o'
repeat:
	receives state from server
	if my turn:
		repeat:
			getmove from player: [1-9,q]
		until legal or quit
		send move as one byte char: [0-8]
until:
	disconnect OR
	game over OR
	quit
then: exit
