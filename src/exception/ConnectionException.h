//
// Author: markovd@students.zcu.cz
//

#ifndef DAMA_SERVER_CONNECTIONEXCEPTION_H
#define DAMA_SERVER_CONNECTIONEXCEPTION_H

#include <exception>

/**
 * Exception class describing error when establishing socket connection.
 */
class ConnectionException : public std::exception {
private: //private attributes
    /**
     * Reason why the exception is thrown.
     */
    const char* cause;
public: // public attributes
    /**
     * Creates exception with given cause.
     *
     * @param cause cause of the exception
     */
    explicit ConnectionException(const char *cause) {
        this->cause = cause;
    }

    /**
     * Returns the cause of the exception.
     *
     * @return cause of the exception
     */
    [[nodiscard]] const char* what() const noexcept override {
        return cause;
    }
};


#endif //DAMA_SERVER_CONNECTIONEXCEPTION_H
