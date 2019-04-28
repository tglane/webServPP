//
// Created by timog on 23.04.19.
//

#ifndef CPPWEBSERVER_CRYPTOTHREADSETUP_HPP
#define CPPWEBSERVER_CRYPTOTHREADSETUP_HPP

#include <openssl/crypto.h>

class CryptoThreadSetup {

public:

    static void lock_callback(int mode, int type, char *file, int line)
    {
        (void)file;
        (void)line;
        if (mode & CRYPTO_LOCK) {
            pthread_mutex_lock(&(lockarray[type]));
        }
        else {
            pthread_mutex_unlock(&(lockarray[type]));
        }
    }

    static unsigned long thread_id(void)
    {
        unsigned long ret;

        ret=(unsigned long)pthread_self();
        return(ret);
    }

    static void init_locks(void)
    {
        int i;

        lockarray=(pthread_mutex_t *)OPENSSL_malloc(CRYPTO_num_locks() *
                                                    sizeof(pthread_mutex_t));
        for (i=0; i<CRYPTO_num_locks(); i++) {
            pthread_mutex_init(&(lockarray[i]),NULL);
        }

        CRYPTO_set_id_callback((unsigned long (*)())thread_id);
        CRYPTO_set_locking_callback((void (*)(int, int, const char*, int))lock_callback);
    }

    static void kill_locks(void)
    {
        int i;

        CRYPTO_set_locking_callback(NULL);
        for (i=0; i<CRYPTO_num_locks(); i++)
            pthread_mutex_destroy(&(lockarray[i]));

        OPENSSL_free(lockarray);
    }

private:

    static pthread_mutex_t *lockarray;

};


#endif //CPPWEBSERVER_CRYPTOTHREADSETUP_HPP
