//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_CONNECTION_H
#define DAMA_SERVER_CONNECTION_H

namespace app {

    /**
     * Class representing individual connections to this server.
     */
    class Connection {
    public: // public attributes
        /// Number of invalid requests that will cause client to be disconnected.
        static constexpr int MAX_ERROR_COUNT = 3;
    private: // private attributes
        int m_socket;
        /// Number of invalid requests sent to the server.
        int m_errorCount = 0;
    public:
        /**
         * Constructs connection instance at given socket and zero error count.
         * @param socket connection socket
         */
        explicit Connection(int socket)
                            : m_socket(socket) {}

        [[nodiscard]] int getSocket() const {
            return m_socket;
        }

        [[nodiscard]] int getErrorCount() const {
            return m_errorCount;
        }

        void incErrCount() {
            m_errorCount++;
        }
    };
}


#endif //DAMA_SERVER_CONNECTION_H
