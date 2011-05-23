/* 
 * File:   WindowContext.h
 * Author: rbuck
 *
 * Created on May 23, 2011, 3:57 PM
 */

#ifndef WINDOWCONTEXT_H
#define	WINDOWCONTEXT_H

#include "Context.h"
#include <sys/types.h>

class WindowContext : public Context {
public:
    WindowContext(u_int32_t iss);
    WindowContext(const WindowContext& orig);
    virtual ~WindowContext();
    
    u_int32_t get_iss() const;
    void set_iss(u_int32_t iss);
    
    u_int32_t get_snd_nxt() const;
    void set_snd_nxt(u_int32_t snd_nxt);
    
    u_int32_t get_snd_una() const;
    void set_snd_una(u_int32_t snd_una);
    
private:
    u_int32_t iss_;
    u_int32_t snd_nxt_;
    u_int32_t snd_una_;
};

#endif	/* WINDOWCONTEXT_H */

