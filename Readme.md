# Protocol
Delimiter of every parameter is **'|'**. \
Indicator of the end of the message is **'\n'**.\
Prorocol message pattern is `<userId>|<REQUEST_TYPE>|<parameter1>|<parameter2>|...>`\
Request does not end with a delimiter.\
If the request is not according to the basics of the protocol - userId or request type or delimiters missing,
too many or few parameters, response with `450` error code is returned.
## User login
Request to log user into the server with given nickname. Only users, who are not logged in may request to login. Nickname is limited to a-zA-Z0-9 characters and to maximum of 20 characters.
### Request
`<userId>|CONNECT|<nickname>\n`
- `userId = 0` => new connection or reconnect, based on `nickname`
- `userId <> 0` => invalid
### Response
`200|<userId>\n` - new connection ok\
`201|<userId>\n` - reconnection ok\
`400\n` - invalid nickname\
`401\n` - invalid userId
`402\n` - cannot reconnect (not disconnected)
## Logout
Request to logout the player with given user ID. User will not be disconnected. Only existing users in lobby can logout.
### Request
`<userId>|LOGOUT\n`
### Response
`202\n` - logout ok\
`403\n` - user non-existent\
`404\n` - user cannot logout (in a game maybe, has to leave the game first)
## Get games
Request to get all games waiting for second player to join. Only logged in players can request game list.
### Request
`<userId>|GET_GAMES\n`
### Response
`203|<nickname>|<nickname>...\n` - request ok, parameters are nicknames of the players waiting to be joined\
`405\n` - user not logged in, cannot request games
## Create game
Request to create a new game. Requesting player will be set as the first player, waiting for second to join. Only players in lobby may request creating new game.
### Request
`<userId>|CREATE_GAME\n`
### Response
`204\n` - ok, new game created and joined\
`406\n` - cannot create new game, not in lobby\
`407\n` - cannot create new game, not logged in\
\
When new game is created, every client in lobby gets response from server:\
`250|<nickname>\n` - nickname is the nick of player who is in the game, waiting to be joined
## Exit game
### Request
`<userId>|EXIT_GAME\n`
### Response
`205\n` - ok, left the game\
`408\n` - cannot leave game, not logged in\
`409\n` - cannot leave game, not in a game\
\
When player, who created new game, exits the game during waiting for opponent to join, every client in lobby
gets response from server:\
`260|<nickname>\n` - nickname is the nick of player who left the game, waiting to be joined
## Join game
### Request
`<userId>|JOIN_GAME|<nickname>\n` - nickname stands for nickname of opponent to be joined
### Response
`206` - ok, game joined and started
Joining player is always player number two.\
`410\n` - cannot join game, not logged in\
`411\n` - cannot join game, invalid state (only players in lobby may join a game)\
`412\n` - cannot join game, waiting game with player with given nick non existent
## Get game state
### Request
`<userId>|GET_GAME_STATE\n` - request to get the state of the game (token position) we are currently in
### Response
`207|<playerOneNick>,<token1PosX>,<token1PosY>,<token2PosX>,<token2PosY>...|<playerTwoNick>,...\n` -
as a response, player s will receive nickname of player one, positions of all his tokens and the same
respectively for player two.\
Player one is considered to be the player at the bottom part of the table,
player two as the top one. The player, who creates the game, is always player number one.\
`413` - cannot get game state, not logged in
`414` - cannto get game state, not in a running game