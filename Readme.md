# Protocol
Delimiter of every parameter is **'|'**. \
Indicator of the end of the message is **'\n'**.\
Prorocol message pattern is `<userId>|<REQUEST_TYPE>|<parameter1>|<parameter2>|...>`\
Request does not end with a delimiter.\
If the request is not according to the basics of the protocol - userId or request type or delimiters missing,
too many or few parameters, response with `450` error code is returned.
## User login
### Request
`<userId>|CONNECT|<nickname>\n`
- `userId = 0` => new connection or reconnect, based on `nickname`
- `userId < 0` => invalid
### Response
`200|<userId>` - new connection ok\
`201|<userId>` - reconnection ok\
`400` - invalid nickname\
`401` - invalid userId
`402` - cannot reconnect (not disconnected)
## Logout
### Request
`<userId>|LOGOUT`
### Response
`202` - logout ok\
`403` - user non-existent\
`404` - user cannot logout (in a game maybe, has to exit the game first)