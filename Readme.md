# Protocol
Delimiter of every parameter is **'|'**. \
Indicator of the end of the message is **'\n'**.
## User login
### Request
*userId*|CONNECT|*nickname*\n
- userId = 0 => new connection, userId will be generated
- userId > 0 => reconnection
- userId < 0 => invalid
### Response
200|*userId* - new connection ok\
201|*userId* - reconnection ok\
401 - invalid nickname\
402 - invalid userId