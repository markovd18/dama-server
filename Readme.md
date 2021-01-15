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
`200|<userId>` - new connection ok\
`201|<userId>` - reconnection ok\
`400` - invalid nickname\
`401` - invalid userId
`402` - cannot reconnect (not disconnected)
## Logout
Request to logout the player with given user ID. User will not be disconnected. Only existing users in lobby can logout.
### Request
`<userId>|LOGOUT`
### Response
`202` - logout ok\
`403` - user non-existent\
`404` - user cannot logout (in a game maybe, has to leave the game first)
## Get games
Request to get all games waiting for second player to join. Only logged in players can request game list.
### Request
`<userId>|GET_GAMES`
### Response
`203|<nickname>|<nickname>...` - request ok, parameters are nicknames of the players waiting to be joined\
`405` - user not logged in, cannot request games
## Create game
Request to create a new game. Requesting player will be set as the first player, waiting for second to join. Only players in lobby may request creating new game.
### Request
`<userId>|CREATE_GAME`
### Response
`204` - ok, new game created and joined\
`406` - cannot create new game, not in lobby\
`407` - cannot create new game, not logged in
## Exit game
### Request
`<userId>|EXIT_GAME`
### Response
`205` - ok, left the game\
`408` - cannot leave game, not logged in\
`409` - cannot leave game, not in a game