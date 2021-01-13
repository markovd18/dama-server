# Protocol
Delimiter of every parameter is **'|'**. \
Indicator of the end of the message is **'\n'**.\
Prorocol message pattern is `<userId>|<REQUEST_TYPE>|<parameter1>|<parameter2>|...>`\
Request does not end with a delimiter.\
If the request is not according to the basics of the protocol - userId or request type or delimiters missing, response with `450` error code is returned.
## User login
### Request
`<userId>|CONNECT|<nickname>\n`
- `userId = 0` => new connection, userId will be generated
- `userId > 0` => reconnection
- `userId < 0` => invalid
### Response
`200|<userId>` - new connection ok\
`201|<userId>` - reconnection ok\
`401` - invalid nickname\
`402` - invalid userId