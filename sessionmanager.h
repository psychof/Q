#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

class SessionManager
{
public:
    static SessionManager& instance() {
        static SessionManager instance;
        return instance;
    }

    void setCurrentUserId(int userId) {
        m_currentUserId = userId;
    }

    int getCurrentUserId() const {
        return m_currentUserId;
    }

    void clearSession(){
      m_currentUserId = -1;
    }

private:
    SessionManager() : m_currentUserId(-1) {}  // -1 означает, что пользователь не авторизован
    int m_currentUserId;

    // Убираем возможность копирования и присваивания
    SessionManager(const SessionManager&) = delete;
    SessionManager& operator = (const SessionManager&) = delete;
};

#endif // SESSIONMANAGER_H
