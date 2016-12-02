# distributedPacman
Academic project, *distributed* implementation of the famous 'Pac-Man' game. It was developed for the signature 'Distributed Systems".

It was tested on Debian (Jessie), weird behavior can be experimented when playing in other distributions.

In this version a the players are allowed to control the ghost that chase Pacman, the assignment of characters is done in order of connections. The first one to connect will be Pacman and the last player connecting to the server will have the slowest ghost.

##How to play
- Ensure that you have a g++ compiler updated and the ncurses library installed
- Run the command ```make```, in case of error go back to the previous step
- Execute the ```server``` with an optional parameter (number of players that will play)
- The player running the server must check his **IP address** and share it with the other players so they can use it to connect
- The other players must execute the ```client``` adding the IP address of the host as parameter, when no IP address is added the program assumes that the server is localhost.
- Once everyone is connected, each player will be able to use the arrow keys to move and play like the classic game


