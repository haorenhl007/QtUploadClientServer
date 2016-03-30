#pragma once

#include <QObject>
#include <QMutex>

QT_FORWARD_DECLARE_CLASS(QQmlEngine)
QT_FORWARD_DECLARE_CLASS(QJSEngine)

template<class T>
class qmlSingletonPattern
{
public:
    qmlSingletonPattern(T &rObject) {
        Q_ASSERT_X(!s_pInstance, "constructor", "Only one instance of this class is permitted.");
        s_pInstance = &rObject;
    }

    ~qmlSingletonPattern() {
        Q_ASSERT_X(s_pInstance, "destructor", "The singleton instance is invalid.");
        s_pInstance = Q_NULLPTR;
    }

    static QObject *qmlInstance(QQmlEngine *qEng, QJSEngine *jEng) {
        // Use double lock checking
        static QMutex mutex;
        if (!s_pInstance) {
            mutex.lock();

            if (!s_pInstance)
                s_pInstance = new T(qEng, jEng);

            mutex.unlock();
        }

        Q_ASSERT_X(s_pInstance, "instancing", "The singleton has not yet been created.");
        return s_pInstance;
    }

private:
    static T *s_pInstance;

    // Don't implement
    qmlSingletonPattern() Q_DECL_EQ_DELETE;
    Q_DISABLE_COPY(qmlSingletonPattern)
};

template<class T>
T *qmlSingletonPattern<T>::s_pInstance = Q_NULLPTR;
