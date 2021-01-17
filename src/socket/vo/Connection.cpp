//
// Author: markovd@students.zcu.cz
//

#include "Connection.h"

app::Connection::Connection(int socket)
        : m_socket(socket) {}

void app::Connection::setUserId(const int userId) {
    m_userId = userId;
}

int app::Connection::getUserId() const {
    return m_userId;
}

void app::Connection::incErrCount() {
    m_errorCount++;
}

int app::Connection::getErrorCount() const {
    return m_errorCount;
}

int app::Connection::getSocket() const {
    return m_socket;
}
